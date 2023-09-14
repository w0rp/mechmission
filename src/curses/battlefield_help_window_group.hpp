#ifndef __MECHMISSION_CURSES_BATTLEFIELD_HELP_WINDOW_GROUP_H_
#define __MECHMISSION_CURSES_BATTLEFIELD_HELP_WINDOW_GROUP_H_

#include "window.hpp"
#include "input.hpp"
#include "../game_state.hpp"
#include "../game_action.hpp"

namespace curses {
    class BattlefieldHelpWindowGroup {
    public:
        BattlefieldHelpWindowGroup();

        const Window& main_window() const;
        void resize();
        const GameActionArray handle_input(
            const GameState& game_state,
            curses::Input input
        );
        void render(const GameState& game_state);
    private:
        Window _help_window;
    };
}

#endif
