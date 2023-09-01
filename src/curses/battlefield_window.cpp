#include "battlefield_window.hpp"

#include <cstring>
#include <algorithm>
#include <entt/entity/registry.hpp>
#include <cmath>

#include "../grid.hpp"
#include "../components/point.hpp"
#include "../components/mech.hpp"
#include "../components/control.hpp"
#include "../pathing.hpp"
#include "color.hpp"
#include "input.hpp"
#include "screen.hpp"

namespace curses {
    const int HUD_WIDTH = 20;

    BattlefieldWindow::BattlefieldWindow(const Point& field_pos) noexcept:
        _hud(0, 0, HUD_WIDTH, curses::screen_height()),
        _field(HUD_WIDTH, 0, curses::screen_width() - HUD_WIDTH, curses::screen_height()),
        _field_pos(field_pos),
        _movement_path(),
        _movement_spaces()
    {
    }

    // Check if there is a Mech at a given point.
    bool mech_exists_at_point(const entt::registry& registry, Point point) {
        auto view = registry.view<const Point, const Mech>();

        for(auto [entity, mechPoint, mech]: view.each()) {
            if (mechPoint == point) {
                return true;
            }
        }

        return false;
    }

// Implementation details.
struct BattlefieldWindow::Impl {
    static void _update_paths_to_draw(
        BattlefieldWindow& window,
        const entt::registry& registry,
        const Grid& grid
    ) {
        auto selected = registry.view<const Point, const Mech, PlayerSelected>();

        for(auto [entity, point, mech]: selected.each()) {
            // Track the selected entity in the window and re-compute
            // reachable spaces only as-needed.
            if (window._selected_entity != entity) {
                window._selected_entity = entity;
                bfs_reachable_spaces(
                    window._movement_spaces,
                    registry,
                    grid,
                    mech.energy,
                    point
                );
            }

            a_star_movement(
                window._movement_path,
                registry,
                grid,
                mech.energy,
                point,
                window._field_pos
            );

            return;
        }

        // Clear selection and spaces if needed.
        window._selected_entity = entt::null;
        window._movement_path.clear();
        window._movement_spaces.clear();
    }

    // Draw hexes like so:
    //
    //  . .
    // . . .
    //  . .
    static void _draw_hexes(
        BattlefieldWindow& window,
        const entt::registry& registry,
        const Grid& grid,
        const Point point
    ) {
        window._field.clear();
        const int width = window._field.width();
        const int height = window._field.height();
        const int half_height = height / 2;
        const int offset_x = int(floor((width - height) / 4.0));

        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                if (i % 2 == j % 2) {
                    const Point draw_point(
                        point.x + (i - j) / 2 - offset_x,
                        point.y + j - half_height
                    );

                    if (grid.has(draw_point)) {
                        if (mech_exists_at_point(registry, draw_point)) {
                            window._field.draw(i, j, '%');
                        } else if (
                            // Search the path vector for this draw point.
                            std::find(
                                window._movement_path.begin(),
                                window._movement_path.end(),
                                draw_point
                            ) != window._movement_path.end()
                        ) {
                            window._field.color_on(curses::Color::terrain);
                            window._field.draw(i, j, '_');
                            window._field.color_off(curses::Color::terrain);
                        } else if (
                            window._movement_spaces.contains(draw_point)
                        ) {
                            window._field.color_on(curses::Color::selectable);
                            window._field.draw(i, j, '.');
                            window._field.color_off(curses::Color::selectable);
                        } else {
                            window._field.color_on(curses::Color::terrain);
                            window._field.draw(i, j, '.');
                            window._field.color_off(curses::Color::terrain);
                        }
                    }
                }
            }
        }

        // Search for first hex cell the cursor could lie in.
        int cursor_i = 0;
        int cursor_j = 0;

        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                if (i % 2 == j % 2) {
                    const int x = (i - j) / 2 - offset_x;
                    const int y = j - half_height;

                    if (x >= 0 && y >= 0) {
                        cursor_i = i;
                        cursor_j = j;
                        goto cursor_found;
                    }
                }
            }
        }
        cursor_found:

        window._field.position_cursor(cursor_i, cursor_j);
    }

    static void _draw_hud(
        BattlefieldWindow& window,
        const entt::registry& registry,
        const Grid& grid
    ) {
        // Draw the coordinates atop the HUD.
        window._hud.clear();
        window._hud.draw_borders();
        window._hud.drawf(3, 0, "x:%+04d,y:%+04d",
            window._field_pos.x, window._field_pos.y);
        window._hud.drawf(1, window._hud.height() - 2,
            "Turn: %d", grid.turn_number());
        window._hud.draw(6, window._hud.height() - 1, "? - help");

        _draw_unit_data(window, registry);
    }

    static void _draw_windows(
        BattlefieldWindow& window,
        const entt::registry& registry,
        const Grid& grid
    ) {
        _update_paths_to_draw(window, registry, grid);
        _draw_hexes(window, registry, grid, window._field_pos);
        _draw_hud(window, registry, grid);
        // The UI and windows need to be refreshed constantly.
        curses::refresh_ui();
        window._hud.refresh();
        window._field.refresh();
    }

    // Redraw window borders and such when the terminal is resized.
    static void _resize_windows(BattlefieldWindow& window) {
        auto width = curses::screen_width();
        auto height = curses::screen_height();

        window._hud.resize(0, 0, HUD_WIDTH, height);
        window._field.resize(HUD_WIDTH, 0, width - 20, height);

        curses::clear_ui();
        window._hud.clear();
        window._field.clear();
        window._hud.draw_borders();
    }

    static void _draw_unit_data(
        BattlefieldWindow& window,
        const entt::registry& registry
    ) {
        auto view = registry.view<const Point, const Mech>();

        for(auto [entity, mechPoint, mech]: view.each()) {
            if (mechPoint == window._field_pos) {
                window._hud.drawf(1, 1, "Unit %02d", mech.number);
                window._hud.drawf(1, 2, "HP - %03d", mech.health);
                window._hud.drawf(1, 3, "EP - %03d/%03d",
                    mech.energy, mech.max_energy);

                break;
            }
        }
    }

    static void _draw_confirm_popup(
        Window& popup,
        const bool ok,
        const char* message,
        const char* ok_text
    ) {
        popup.clear();
        popup.draw_borders();
        popup.draw(popup.width() / 2 - strlen(message) / 2 - 1, 1, message);

        if (!ok) popup.color_on(curses::Color::menu_selection);
        popup.draw(2, 3, "[Cancel]");
        if (!ok) popup.color_off(curses::Color::menu_selection);
        if (ok) popup.color_on(curses::Color::menu_selection);
        popup.draw(popup.width() - strlen(ok_text) - 4, 3, '[');
        popup.draw(popup.width() - strlen(ok_text) - 3, 3, ok_text);
        popup.draw(popup.width() - 3, 3, ']');
        if (ok) popup.color_off(curses::Color::menu_selection);

        popup.refresh();
    }

    static bool _confirm_action(
        BattlefieldWindow& window,
        const entt::registry& registry,
        const Grid& grid,
        const char* message,
        const char* ok_text
    ) {
        bool ok = false;

        hide_cursor();

        const int popup_width = 30;
        const int popup_height = 5;
        Window popup{
            window._field,
            window._field.width() / 2 - popup_width / 2,
            window._field.height() / 2 - popup_height / 2,
            popup_width,
            popup_height,
        };

        bool done = false;

        while (!done) {
            _draw_confirm_popup(popup, ok, message, ok_text);

            switch (curses::get_input()) {
                case curses::Input::resize:
                    // Resize the main windows and popup.
                    _resize_windows(window);
                    popup.resize(
                        window._field.width() / 2 - popup_width / 2,
                        window._field.height() / 2 - popup_height / 2,
                        popup_width,
                        popup_height
                    );
                    // Redraw the main windows first.
                    _draw_windows(window, registry, grid);
                    // We'll redraw the popup in the next iteration.
                    break;
                case curses::Input::quit:
                    ok = false;
                    done = true;
                    break;
                case curses::Input::left:
                case curses::Input::right:
                    ok = !ok;
                    break;
                case curses::Input::enter:
                    done = true;
                    break;
                default:
                    break;
            }
        }

        show_cursor();

        return ok;
    }
};

    Point BattlefieldWindow::cursor() const noexcept {
        return _field_pos;
    }

    bool BattlefieldWindow::set_cursor(const Grid& grid, const Point p) noexcept {
        if (grid.has(p)) {
            _field_pos = p;
            return true;
        }

        return false;
    }

    BattlefieldWindowAction BattlefieldWindow::step(
        const entt::registry& registry,
        const Grid& grid,
        const bool input_locked
    ) {
        Impl::_draw_windows(*this, registry, grid);

        curses::Input input;

        // Prevent inputs if input is locked.
        if (input_locked) {
            // Wait 10 frames of 60FPS, thereabouts.
            curses::sleep(16 * 10);
            input = curses::check_for_resize();
        } else {
            input = curses::get_input();
        }

        auto action = BattlefieldWindowAction::none;

        switch (input) {
            case curses::Input::resize:
                Impl::_resize_windows(*this);
                break;
            case curses::Input::up:
                set_cursor(
                    grid,
                    Point(
                        _field_pos.x + (abs(_field_pos.y) % 2 == 0),
                        _field_pos.y - 1
                    )
                );
                break;
            case curses::Input::down:
                set_cursor(
                    grid,
                    Point(
                        _field_pos.x - (abs(_field_pos.y) % 2),
                        _field_pos.y + 1
                    )
                );
                break;
            case curses::Input::left:
                set_cursor(
                    grid,
                    Point(_field_pos.x - 1, _field_pos.y)
                );
                break;
            case curses::Input::right:
                set_cursor(
                    grid,
                    Point(_field_pos.x + 1, _field_pos.y)
                );
                break;
            case curses::Input::space:
                action = BattlefieldWindowAction::select;
                break;
            case curses::Input::enter:
                if (
                    Impl::_confirm_action(
                        *this,
                        registry,
                        grid,
                        "End turn?",
                        "End Turn"
                    )
                ) {
                    action = BattlefieldWindowAction::end_turn;
                }
                break;
            case curses::Input::quit:
                if (
                    Impl::_confirm_action(
                        *this,
                        registry,
                        grid,
                        "Quit game?",
                        "Quit"
                    )
                ) {
                    action = BattlefieldWindowAction::quit;
                }
                break;
            case curses::Input::question_mark:
                action = BattlefieldWindowAction::help;
                break;
            case curses::Input::unknown:
                break;
        }

        return action;
    }
}
