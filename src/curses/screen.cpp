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
}
