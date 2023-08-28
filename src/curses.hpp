#ifndef __MECHMISSION_CURSES_H_
#define __MECHMISSION_CURSES_H_

#include "curses/screen.hpp"
#include "curses/window.hpp"

#include "grid.hpp"

namespace curses {
    class BattlefieldWindow {
        Window _hud;
        Window _field;
    public:
        BattlefieldWindow();

        void draw_hexes(const Grid& grid, int x, int y);
        Input get_input();
    };
}

#endif
