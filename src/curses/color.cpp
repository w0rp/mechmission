#include "color.hpp"

#include <ncurses.h>

namespace curses {
    void setup_colors() noexcept {
        start_color();
        init_pair(int(Color::normal), COLOR_WHITE, COLOR_BLACK);
        init_pair(int(Color::menu_selection), COLOR_BLACK, COLOR_WHITE);
        init_pair(int(Color::terrain), COLOR_CYAN, COLOR_BLACK);
        init_pair(int(Color::selectable), COLOR_BLUE, COLOR_BLACK);
    }
}
