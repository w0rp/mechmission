#ifndef __MECHMISSION_CURSES_SCREEN_H_
#define __MECHMISSION_CURSES_SCREEN_H_

namespace curses {
    enum Input {
        unknown,
        resize,
        quit,
        up,
        down,
        left,
        right,
    };

    void start_ui();
    void stop_ui();
    void clear_ui();
    void refresh_ui();
    int screen_width();
    int screen_height();
    Input get_input();
}

#endif
