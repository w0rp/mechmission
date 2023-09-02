#ifndef __MECHMISSION_CURSES_WINDOW_GROUP_H_
#define __MECHMISSION_CURSES_WINDOW_GROUP_H_

#include <vector>

#include "../game_action.hpp"
#include "../game_state.hpp"
#include "input.hpp"
#include "window.hpp"

namespace curses {
    class WindowGroup {
    public:
        WindowGroup() {}
        virtual ~WindowGroup() {}

        virtual const Window& main_window() const = 0;
        virtual void resize() = 0;
        virtual const std::vector<GameAction> handle_input(
            const GameState& game_state,
            curses::Input input
        ) = 0;
        virtual void render(const GameState& game_state) = 0;
    };
}

#endif
