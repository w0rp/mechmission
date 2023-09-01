#include "battlefield_help_window.hpp"

#include <array>

#include "input.hpp"
#include "screen.hpp"

namespace curses {
    void draw_help(Window& window) noexcept {
        window.draw_borders();

        std::array help_lines{
            "Move     - arrows,hjkl",
            "Select   - Space",
            "End Turn - Enter",
            "Quit     - q",
            "Help     - ?",
        };

        int y = 1;

        for (auto&& line: help_lines) {
            window.draw(1, y, line);
            ++y;
        }
    }

    BattlefieldHelpWindow::BattlefieldHelpWindow() noexcept:
        _help(0, 0, curses::screen_width(), curses::screen_height())
    {
    }

    BattlefieldHelpWindowAction BattlefieldHelpWindow::step() noexcept {
        draw_help(_help);
        curses::refresh_ui();
        _help.refresh();

        switch(curses::get_input()) {
            case Input::quit:
                return BattlefieldHelpWindowAction::quit;
            case Input::resize:
                _help.resize(0, 0, curses::screen_width(), curses::screen_height());
                curses::clear_ui();
                _help.clear();

                return BattlefieldHelpWindowAction::none;
            default:
                return BattlefieldHelpWindowAction::none;
        }
    }
}
