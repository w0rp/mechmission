#include "battlefield_help_window.hpp"

#include "screen.hpp"

namespace curses {
    void draw_help(Window& window) noexcept {
        window.draw_borders();
        window.draw(1, 1, "Move   - arrows,hjkl");
        window.draw(1, 2, "Select - Space");
        window.draw(1, 3, "Quit   - q");
        window.draw(1, 4, "Help   - ?");
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
