#ifndef __MECHMISSION_CURSES_INPUT_H_
#define __MECHMISSION_CURSES_INPUT_H_

namespace curses {
    enum class Input {
        unknown,
        no_more_input,
        resize,
        quit,
        up,
        down,
        left,
        right,
        space,
        question_mark,
        enter,
    };

    // Check for a window resize input or otherwise return unknown input.
    // This should be called after sleeping to resize windows.
    Input check_for_resize() noexcept;
    Input get_input() noexcept;
}

#endif
