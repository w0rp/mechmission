#ifndef __MECHMISSION_CURSES_BATTLEFIELD_HELP_WINDOW_GROUP_H_
#define __MECHMISSION_CURSES_BATTLEFIELD_HELP_WINDOW_GROUP_H_

#include "window_group.hpp"

namespace curses {
    class BattlefieldHelpWindowGroup: public WindowGroup {
        Window _help_window;
    public:
        BattlefieldHelpWindowGroup();

        const Window& main_window() const override;
        void resize() override;
        const GameActionArray handle_input(
            const GameState& game_state,
            curses::Input input
        ) override;
        void render(const GameState& game_state) override;
    };
}

#endif
