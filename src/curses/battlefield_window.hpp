#ifndef __MECHMISSION_CURSES_BATTLEFIELD_WINDOW_H_
#define __MECHMISSION_CURSES_BATTLEFIELD_WINDOW_H_

#include "entt/entity/fwd.hpp"
#include "window.hpp"

#include "../grid.hpp"
#include "../components/point.hpp"

#include <entt/entity/registry.hpp>

namespace curses {
    enum class BattlefieldWindowAction {
        none,
        quit,
        select
    };

    class BattlefieldWindow {
        Window _hud;
        Window _field;
        Point _field_pos;

        // A composed struct for forward-declaring implementation details.
        struct Impl;
    public:
        BattlefieldWindow();
        Point cursor() const;
        bool set_cursor(const Grid& grid, const Point point);
        BattlefieldWindowAction step(
            const entt::registry& registry,
            const Grid& grid,
            const bool input_locked
        );
    };
}

#endif
