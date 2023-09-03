#ifndef __MECHMISSION_CURSES_CONFIRMATION_WINDOW_GROUP_H_
#define __MECHMISSION_CURSES_CONFIRMATION_WINDOW_GROUP_H_

#include "window_group.hpp"

namespace curses {
    class ConfirmationWindowGroup: public WindowGroup {
        // NOTE: The lifetime of the parent window is assumed to be greater
        // than the confirmation window. The reference will be dangling if
        // the parent window is destroyed before the confirmation window.
        const Window& _parent_window;
        Window _popup;
        const char* _message;
        const char* _ok_text;
        bool _ok;
        GameAction _ok_action;

        // A forward declaration to private implementation details.
        struct Impl;
    public:
        ConfirmationWindowGroup(
            const Window& parent_window,
            const char* message,
            const char* ok_text,
            GameAction ok_action
        );

        const Window& main_window() const override;
        void resize() override;
        const std::vector<GameAction> handle_input(
            const GameState& game_state,
            curses::Input input
        ) override;
        void render(const GameState& game_state) override;
    };
}

#endif
