#ifndef __MECHMISSION_CURSES_BATTLEFIELD_WINDOW_GROUP_H_
#define __MECHMISSION_CURSES_BATTLEFIELD_WINDOW_GROUP_H_

#include <set>
#include <vector>

#include "window.hpp"
#include "input.hpp"
#include "../components/point.hpp"
#include "../game_state.hpp"
#include "../game_action.hpp"

namespace curses {
    class BattlefieldWindowGroup {
    public:
        BattlefieldWindowGroup(const Point& field_pos) noexcept;

        const Window& main_window() const;
        void resize();
        const GameActionArray handle_input(
            const GameState& game_state,
            curses::Input input
        );
        void render(const GameState& game_state);
    private:
        // A forward declaration to private implementation details.
        struct Impl;

        Window _hud;
        Window _field;
        std::vector<Point> _movement_path;
        std::set<Point> _movement_spaces;
        entt::entity _selected_entity;
    };
}

#endif
