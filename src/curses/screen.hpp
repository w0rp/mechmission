#ifndef __MECHMISSION_CURSES_SCREEN_H_
#define __MECHMISSION_CURSES_SCREEN_H_

namespace curses {
    void start_ui() noexcept;
    void stop_ui() noexcept;
    void clear_ui() noexcept;
    void refresh_ui() noexcept;
    void hide_cursor() noexcept;
    void show_cursor() noexcept;
    int screen_width() noexcept;
    int screen_height() noexcept;
    // Sleep for a number of milliseconds.
    void sleep(int ms) noexcept;
}

#endif
