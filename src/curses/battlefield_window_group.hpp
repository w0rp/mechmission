#ifndef __MECHMISSION_CURSES_BATTLEFIELD_WINDOW_GROUP_H_
#define __MECHMISSION_CURSES_BATTLEFIELD_WINDOW_GROUP_H_

#include <set>
#include <vector>

#include "../components/point.hpp"

#include "window_group.hpp"

namespace curses {
    class BattlefieldWindowGroup final: public WindowGroup {
        Window _hud;
        Window _field;
        std::vector<Point> _movement_path;
        std::set<Point> _movement_spaces;
        entt::entity _selected_entity;

        // A forward declaration to private implementation details.
        struct Impl;
    public:
        BattlefieldWindowGroup(const Point& field_pos) noexcept;

        const Window& main_window() const override;
        void resize() override;
        const GameActionArray handle_input(
            const GameState& game_state,
            curses::Input input
        ) override;
        void render(const GameState& game_state) override;
    };
}

#endif
