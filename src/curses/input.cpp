#include "input.hpp"

#include <ncurses.h>

namespace curses {
    Input check_for_resize() noexcept {
        auto input = Input::unknown;
        int ch;
        // Stop blocking for getch calls.
        nodelay(stdscr, TRUE);

        while((ch = getch()) != ERR) {
            if (ch == KEY_RESIZE) {
                input = Input::resize;
            }
        }

        // Start blocking forever again.
        wtimeout(stdscr, -1);

        return input;
    }

    Input get_input() noexcept {
        switch(getch()) {
            case ERR:
                return Input::no_more_input;
            case 'q':
            case 27: // Escape
                return Input::quit;
            case KEY_RESIZE:
                return Input::resize;
            case KEY_UP:
            case 'k':
                return Input::up;
            case KEY_DOWN:
            case 'j':
                return Input::down;
            case KEY_LEFT:
            case 'h':
                return Input::left;
            case KEY_RIGHT:
            case 'l':
                return Input::right;
            case ' ':
                return Input::space;
            case '?':
                return Input::question_mark;
            case '\n':
                return Input::enter;
            default:
                return Input::unknown;
        }
    }
}
