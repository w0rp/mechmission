#ifndef __MECHMISSION_CURSES_COLOR_H_
#define __MECHMISSION_CURSES_COLOR_H_

namespace curses {
    enum class Color {
        normal,
        terrain,
        selectable,
    };

    void setup_colors() noexcept;
}

#endif
