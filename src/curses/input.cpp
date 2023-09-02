#include "input.hpp"

#include <ncurses.h>

namespace curses {
    MousePosition last_mouse_position;

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
            case KEY_MOUSE:
                if (MEVENT event; getmouse(&event) == OK) {
                    if (event.bstate & BUTTON1_CLICKED) {
                        last_mouse_position.x = event.x;
                        last_mouse_position.y = event.y;

                        return Input::mouse_left_click;
                    }

                    if (event.bstate & BUTTON2_PRESSED) {
                        last_mouse_position.x = event.x;
                        last_mouse_position.y = event.y;

                        return Input::mouse_right_click;
                    }
                }

                return Input::unknown;
            default:
                return Input::unknown;
        }
    }

    MousePosition get_last_mouse_position() noexcept {
        return last_mouse_position;
    }
}
