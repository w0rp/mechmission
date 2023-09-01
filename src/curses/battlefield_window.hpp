#ifndef __MECHMISSION_CURSES_BATTLEFIELD_WINDOW_H_
#define __MECHMISSION_CURSES_BATTLEFIELD_WINDOW_H_

#include <set>

#include <entt/entity/registry.hpp>
#include "entt/entity/fwd.hpp"
#include "window.hpp"

#include "../grid.hpp"
#include "../components/point.hpp"

namespace curses {
    enum class BattlefieldWindowAction {
        none,
        select,
        end_turn,
        quit,
        help,
    };

    class BattlefieldWindow {
        Window _hud;
        Window _field;
        Point _field_pos;
        std::vector<Point> _movement_path;
        std::set<Point> _movement_spaces;
        entt::entity _selected_entity;

        // A composed struct for forward-declaring implementation details.
        struct Impl;
    public:
        BattlefieldWindow() = delete;
        BattlefieldWindow(const Point& field_pos) noexcept;
        Point cursor() const noexcept;
        bool set_cursor(const Grid& grid, const Point point) noexcept;
        BattlefieldWindowAction step(
            const entt::registry& registry,
            const Grid& grid,
            const bool input_locked
        );
    };
}

#endif
