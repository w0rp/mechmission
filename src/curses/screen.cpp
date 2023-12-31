#include "screen.hpp"

#include "color.hpp"

#include <ncurses.h>

namespace curses {
    void start_ui() noexcept {
        initscr();
        curses::setup_colors();
        // Pass through characters immediately without line buffering.
        // pass other controls through verbatim.
        raw();
        // Enable non-blocking input.
        timeout(0);
        // Enable the keypad.
        keypad(stdscr, TRUE);
        // Enable mouse events.
        mousemask(ALL_MOUSE_EVENTS, NULL);
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
