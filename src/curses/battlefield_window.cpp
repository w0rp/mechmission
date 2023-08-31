#include "battlefield_window.hpp"

#include <entt/entity/registry.hpp>
#include <cmath>

#include "../grid.hpp"
#include "../components.hpp"
#include "../pathing.hpp"
#include "screen.hpp"

namespace curses {
    const int HUD_WIDTH = 20;

    BattlefieldWindow::BattlefieldWindow():
        _hud(0, 0, HUD_WIDTH, curses::screen_height()),
        _field(HUD_WIDTH, 0, curses::screen_width() - HUD_WIDTH, curses::screen_height()),
        _field_pos(0, 0)
    {
        // Draw initial borders when window is created.
        _hud.draw_borders();
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
    static std::set<Point> _get_path_to_draw(
        BattlefieldWindow& window,
        const entt::registry& registry,
        const Grid& grid
    ) {
        auto selected = registry.view<const Point, PlayerSelected>();

        for(auto [entity, point]: selected.each()) {
            return a_star_movement(registry, grid, point, window._field_pos);
        }

        return std::set<Point>();
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
        const std::set<Point>& path,
        const Point point
    ) {
        window._field.clear();
        const int width = window._field.width();
        const int height = window._field.height();
        const int half_height = height / 2;
        const int offset_x = floor((width - height) / 4.0);

        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                if (i % 2 == j % 2) {
                    const Point draw_point(
                        point.x + (i - j) / 2 - offset_x,
                        point.y + j - half_height
                    );

                    if (grid.has(draw_point.x, draw_point.y)) {
                        if (mech_exists_at_point(registry, draw_point)) {
                            window._field.draw(i, j, '%');
                        } else if (path.contains(draw_point)) {
                            window._field.draw(i, j, 'X');
                        } else {
                            window._field.draw(i, j, '.');
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
        auto selected = registry.view<const Mech, PlayerSelected>();
        auto view = registry.view<const Point, const Mech>();

        for(auto [entity, mech]: selected.each()) {
            window._hud.drawf(1, 2, "SELECTED: %d", mech.number);
        }

        for(auto [entity, mechPoint, mech]: view.each()) {
            if (mechPoint == window._field_pos) {
                window._hud.drawf(1, 1, "Unit %02d - %03d HP",
                    mech.number, mech.health);

                break;
            }
        }
    }
};

    Point BattlefieldWindow::cursor() const {
        return _field_pos;
    }

    bool BattlefieldWindow::set_cursor(const Grid& grid, const Point p) {
        if (grid.has(p.x, p.y)) {
            _field_pos = p;
            return true;
        }

        return false;
    }

    BattlefieldWindowAction BattlefieldWindow::step(
        const entt::registry& registry,
        const Grid& grid
    ) {
        Impl::_draw_hexes(
            *this,
            registry,
            grid,
            Impl::_get_path_to_draw(*this, registry, grid),
            _field_pos
        );

        // Draw the coordinates atop the HUD.
        _hud.clear();
        _hud.draw_borders();
        _hud.drawf(3, 0, "x:%+04d,y:%+04d", _field_pos.x, _field_pos.y);
        Impl::_draw_unit_data(*this, registry);

        // The UI and windows need to be refreshed constantly.
        curses::refresh_ui();
        _hud.refresh();
        _field.refresh();

        auto action = BattlefieldWindowAction::none;

        switch (curses::get_input()) {
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
            case curses::Input::quit:
                action = BattlefieldWindowAction::quit;
                break;
            case curses::Input::unknown:
                break;
        }

        return action;
    }
}
