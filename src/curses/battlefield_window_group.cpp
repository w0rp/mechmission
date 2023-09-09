#include "battlefield_window_group.hpp"

#include <algorithm>
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

    // Check if there is a Mech at a given point.
    // The player number of the mech will be returned.
    int mech_exists_at_point(const entt::registry& registry, Point point) {
        auto view = registry.view<const Point, const Mech>();

        for(auto [entity, mechPoint, mech]: view.each()) {
            if (mechPoint == point) {
                return mech.player_number;
            }
        }

        return 0;
    }

    BattlefieldWindowGroup::BattlefieldWindowGroup(const Point& field_pos) noexcept:
        _hud(0, 0, HUD_WIDTH, curses::screen_height()),
        _field(HUD_WIDTH, 0, curses::screen_width() - HUD_WIDTH, curses::screen_height()),
        _movement_path(),
        _movement_spaces(),
        _selected_entity(entt::null)
    {
    }

// Implementation details.
struct BattlefieldWindowGroup::Impl {
    static void _update_paths_to_draw(
        BattlefieldWindowGroup& group,
        const GameState& game_state
    ) {
        auto selected = game_state.registry()
            .view<const Point, const Mech, ActingUnit>();

        for(auto [entity, point, mech]: selected.each()) {
            // Track the selected entity in the window and re-compute
            // reachable spaces only as-needed.
            if (group._selected_entity != entity) {
                group._selected_entity = entity;
                bfs_reachable_spaces(
                    group._movement_spaces,
                    game_state.registry(),
                    game_state.grid(),
                    mech.energy,
                    point
                );
            }

            a_star_movement(
                group._movement_path,
                game_state.registry(),
                game_state.grid(),
                mech.energy,
                point,
                game_state.grid_pos()
            );

            return;
        }

        // Clear selection and spaces if needed.
        group._selected_entity = entt::null;
        group._movement_path.clear();
        group._movement_spaces.clear();
    }

    // Draw hexes like so:
    //
    //  . .
    // . . .
    //  . .
    static void _draw_hexes(
        BattlefieldWindowGroup& group,
        const GameState& game_state
    ) {
        auto& registry = game_state.registry();
        auto& grid = game_state.grid();
        auto& point = game_state.grid_pos();

        group._field.clear();
        const int width = group._field.width();
        const int height = group._field.height();
        const int half_height = height / 2;
        const int offset_x = int(floor((width - height) / 4.0));

        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                if (i % 2 == j % 2) {
                    const Point draw_point(
                        point.x + (i - j) / 2 - offset_x,
                        point.y + j - half_height
                    );

                    if (grid.contains(draw_point)) {
                        auto color = curses::Color::terrain;
                        char chr = '.';

                        if (int mech_player = mech_exists_at_point(registry, draw_point); mech_player) {
                            chr = '&';
                            color = mech_player == game_state.player_number()
                                ? curses::Color::normal
                                : curses::Color::enemy_unit;
                        } else if (
                            std::ranges::any_of(
                                group._movement_path,
                                [&](auto&& p){ return p == draw_point; }
                            )
                        ) {
                            chr = '_';
                        } else if (
                            group._movement_spaces.contains(draw_point)
                        ) {
                            color = curses::Color::selectable;
                        }

                        group._field.draw(color, i, j, chr);
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

        group._field.position_cursor(cursor_i, cursor_j);
    }

    static void _draw_unit_data(
        BattlefieldWindowGroup& group,
        const GameState& game_state
    ) {
        auto view = game_state.registry().view<const Point, const Mech>();

        for(auto [entity, mechPoint, mech]: view.each()) {
            if (mechPoint == game_state.grid_pos()) {
                group._hud.drawf(1, 1, "Player %d, Unit %02d",
                    mech.player_number, mech.number);
                group._hud.drawf(1, 2, "HP - %03d", mech.armor.health);
                group._hud.drawf(1, 3, "EP - %03d/%03d",
                    mech.energy, mech.battery.max_energy);

                break;
            }
        }
    }

    static void _draw_hud(
        BattlefieldWindowGroup& group,
        const GameState& game_state
    ) {
        // Draw the coordinates atop the HUD.
        group._hud.clear();
        group._hud.draw_borders();
        group._hud.drawf(3, 0, "x:%+04d,y:%+04d",
            game_state.grid_pos().x, game_state.grid_pos().y);
        group._hud.drawf(1, group._hud.height() - 2,
            "Turn: %d", game_state.turn_number());
        group._hud.draw(6, group._hud.height() - 1, "? - help");

        _draw_unit_data(group, game_state);
    }

    static void _draw_windows(
        BattlefieldWindowGroup& group,
        const GameState& game_state
    ) {
        _update_paths_to_draw(group, game_state);
        _draw_hexes(group, game_state);
        curses::show_cursor();
        _draw_hud(group, game_state);
        group._hud.refresh();
        group._field.refresh();
    }

    static GameActionArray _get_mouse_selection_action(
        BattlefieldWindowGroup& group,
        const GameState& game_state
    ) {
        // The position clicked on the overall screen.
        auto [i, j] = group._field.get_mouse_position();

        // Check if the mouse position points at something inside of the grid.
        //
        // See _draw_hexes for origin of code for translating window
        // coordinates to a point on the grid.
        if (i >= 0 && j >= 0 && i % 2 == j % 2) {
            auto& grid = game_state.grid();
            auto& point = game_state.grid_pos();
            const int width = group._field.width();
            const int height = group._field.height();
            const int half_height = height / 2;
            const int offset_x = int(floor((width - height) / 4.0));

            const Point draw_point(
                point.x + (i - j) / 2 - offset_x,
                point.y + j - half_height
            );

            if (grid.contains(draw_point)) {
                return {
                    GameAction(GameActionTag::battlefield_select_point, draw_point),
                };
            }
        }

        return {};
    }
};

    const Window& BattlefieldWindowGroup::main_window() const {
        return _field;
    }

    void BattlefieldWindowGroup::resize() {
        auto width = curses::screen_width();
        auto height = curses::screen_height();

        _hud.resize(0, 0, HUD_WIDTH, height);
        _field.resize(HUD_WIDTH, 0, width - 20, height);

        _hud.clear();
        _field.clear();
        _hud.draw_borders();
    }

    const GameActionArray BattlefieldWindowGroup::handle_input(
        const GameState& game_state,
        curses::Input input
    ) {
        switch (input) {
            case curses::Input::up:
                return {GameActionTag::battlefield_move_up};
            case curses::Input::down:
                return {GameActionTag::battlefield_move_down};
            case curses::Input::left:
                return {GameActionTag::battlefield_move_left};
            case curses::Input::right:
                return {GameActionTag::battlefield_move_right};
            case curses::Input::mouse_left_click:
                return Impl::_get_mouse_selection_action(*this, game_state);
            case curses::Input::space:
                return {GameActionTag::battlefield_select};
            case curses::Input::enter:
                return {GameActionTag::battlefield_ask_end_turn};
            case curses::Input::quit:
                return {GameActionTag::battlefield_ask_quit};
            case curses::Input::question_mark:
                return {GameActionTag::battlefield_help};
            default:
                return {};
        }
    }

    void BattlefieldWindowGroup::render(const GameState& game_state) {
        Impl::_draw_windows(*this, game_state);
    }
}
