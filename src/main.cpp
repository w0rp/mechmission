#include "grid.hpp"
#include "curses.hpp"

#include <ncurses.h>

int main(int argc, char** argv) {
    Grid g{100};

    g.get(0, 0).color = 1;
    g.get(1, 1).color = 2;
    g.get(2, 2).color = 3;
    g.get(3, 3).color = 4;
    g.get(4, 4).color = 5;
    g.get(4, 4).color = 6;
    g.get(3, 4).color = 'a';
    g.get(4, 3).color = 'b';
    g.get(5, 3).color = 'c';
    g.get(5, 4).color = 'd';
    g.get(4, 5).color = 'e';
    g.get(3, 5).color = 'f';

    curses::start_ui();
    curses::BattlefieldWindow window;

    int x = 0;
    int y = 0;

    auto run = true;

    while (run) {
        window.draw_hexes(g, x, y);

        switch (window.get_input()) {
            case curses::Input::quit:
                run = false;
                break;
            case curses::Input::up:
                if (--y < 0) {
                    y = 0;
                }

                break;
            case curses::Input::down:
                if (++y > g.size()) {
                    y = g.size() - 1;
                }

                break;
            case curses::Input::left:
                if (--x < 0) {
                    x = 0;
                }

                break;
            case curses::Input::right:
                if (++x > g.size()) {
                    x = g.size() - 1;
                }

                break;
            default:
                break;
        }
    }

    curses::stop_ui();

    return 0;
}
