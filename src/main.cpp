#include "grid.hpp"
#include "curses/screen.hpp"
#include "curses/battlefield_window.hpp"

#include <ncurses.h>

int main(int argc, char** argv) {
    Grid g{100};

    curses::start_ui();
    curses::BattlefieldWindow window;

    while (window.step(g) != curses::BattlefieldWindowAction::quit) {
    }

    curses::stop_ui();

    return 0;
}
