#ifndef __MECHMISSION_CURSES_BATTLEFIELD_HELP_WINDOW_H_
#define __MECHMISSION_CURSES_BATTLEFIELD_HELP_WINDOW_H_

#include "window.hpp"

namespace curses {
    enum class BattlefieldHelpWindowAction {
        none,
        quit,
    };

    class BattlefieldHelpWindow {
        Window _help;
    public:
        BattlefieldHelpWindow() noexcept;
        BattlefieldHelpWindowAction step() noexcept;
    };
}

#endif
