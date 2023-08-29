#ifndef __MECHMISSION_CURSES_BATTLEFIELD_WINDOW_H_
#define __MECHMISSION_CURSES_BATTLEFIELD_WINDOW_H_

#include "window.hpp"

#include "../grid.hpp"

namespace curses {
    enum class BattlefieldWindowAction {
        none,
        quit
    };

    class BattlefieldWindow {
        Window _hud;
        Window _field;
        int _field_x;
        int _field_y;

        void _draw_hexes(const Grid& grid, int x, int y);
        void _resize_windows();
        void _move_up();
        void _move_down();
    public:
        BattlefieldWindow();
        BattlefieldWindowAction step(const Grid& grid);
    };
}

#endif
