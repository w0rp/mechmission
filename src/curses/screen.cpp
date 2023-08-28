#include "screen.hpp"

#include <ncurses.h>

namespace curses {
    void start_ui() {
        initscr();
        raw();
        keypad(stdscr, TRUE);
        // curs_set(0);
        noecho();
    }

    void stop_ui() {
        endwin();
    }

    void clear_ui() {
        wclear(stdscr);
    }

    void refresh_ui() {
        refresh();
    }

    int screen_width() {
       return getmaxx(stdscr);
    }

    int screen_height() {
       return getmaxy(stdscr);
    }

    Input get_input() {
        switch(getch()) {
            case 'q':
            case 27: // Escape
                return Input::quit;
            case KEY_RESIZE:
                return Input::resize;
            case KEY_UP:
                return Input::up;
            case KEY_DOWN:
                return Input::down;
            case KEY_LEFT:
                return Input::left;
            case KEY_RIGHT:
                return Input::right;
            default:
                return Input::unknown;
        }
    }
}
