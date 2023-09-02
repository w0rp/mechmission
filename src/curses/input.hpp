#ifndef __MECHMISSION_CURSES_INPUT_H_
#define __MECHMISSION_CURSES_INPUT_H_

#include "mouse_position.hpp"

namespace curses {
    enum class Input {
        unknown,
        no_more_input,
        resize,
        quit,
        mouse_left_click,
        mouse_right_click,
        up,
        down,
        left,
        right,
        space,
        question_mark,
        enter,
    };

    Input get_input() noexcept;
    MousePosition get_last_mouse_position() noexcept;
}

#endif
