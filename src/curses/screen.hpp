#ifndef __MECHMISSION_CURSES_SCREEN_H_
#define __MECHMISSION_CURSES_SCREEN_H_

namespace curses {
    enum class Input {
        unknown,
        resize,
        quit,
        up,
        down,
        left,
        right,
        space,
        question_mark,
    };

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
    // Check for a window resize input or otherwise return unknown input.
    // This should be called after sleeping to resize windows.
    Input check_for_resize() noexcept;
    Input get_input() noexcept;
}

#endif
