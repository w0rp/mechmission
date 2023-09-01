#include "screen.hpp"

#include "color.hpp"

#include <ncurses.h>

namespace curses {
    void start_ui() noexcept {
        initscr();
        curses::setup_colors();
        raw();
        keypad(stdscr, TRUE);
        noecho();
    }

    void stop_ui() noexcept {
        endwin();
    }

    void clear_ui() noexcept {
        wclear(stdscr);
    }

    void refresh_ui() noexcept {
        refresh();
    }

    void hide_cursor() noexcept {
        curs_set(0);
    }

    void show_cursor() noexcept {
        curs_set(1);
    }

    int screen_width() noexcept {
       return getmaxx(stdscr);
    }

    int screen_height() noexcept {
       return getmaxy(stdscr);
    }

    void sleep(int ms) noexcept {
        napms(ms);
    }

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
            default:
                return Input::unknown;
        }
    }
}
