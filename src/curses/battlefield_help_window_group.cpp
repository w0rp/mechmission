#include "battlefield_help_window_group.hpp"

#include "screen.hpp"

namespace curses {
    BattlefieldHelpWindowGroup::BattlefieldHelpWindowGroup():
        _help_window(0, 0, curses::screen_width(), curses::screen_height())
    {
    }

    const Window& BattlefieldHelpWindowGroup::main_window() const {
        return _help_window;
    }

    void BattlefieldHelpWindowGroup::resize() {
        _help_window.resize(0, 0, curses::screen_width(), curses::screen_height());
        _help_window.clear();
    }

    const std::vector<GameAction> BattlefieldHelpWindowGroup::handle_input(
        const GameState& game_state,
        curses::Input input
    ) {
        if (input == Input::quit) {
            return {GameActionTag::close_window};
        }

        return {};
    }

    void BattlefieldHelpWindowGroup::render(const GameState& game_state) {
        curses::hide_cursor();
        _help_window.draw_borders();

        std::array help_lines{
            "Move     - arrows,hjkl",
            "Select   - Space",
            "End Turn - Enter",
            "Quit     - q",
            "Help     - ?",
            "",
            "Try clicking your mouse.",
        };

        int y = 1;

        for (auto&& line: help_lines) {
            _help_window.draw(1, y, line);
            ++y;
        }
        _help_window.refresh();
    }
}
