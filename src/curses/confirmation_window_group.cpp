#include "confirmation_window_group.hpp"
#include "input.hpp"
#include "window_group.hpp"
#include "screen.hpp"

#include <cstring>

namespace curses {
    ConfirmationWindowGroup::ConfirmationWindowGroup(
        const Window& parent_window,
        const char* message,
        const char* ok_text,
        GameAction ok_action
    ):
        WindowGroup(),
        _parent_window(parent_window),
        _popup{
            _parent_window,
            _parent_window.width() / 2 - 30 / 2,
            _parent_window.height() / 2 - 5 / 2,
            30,
            5,
        },
        _message(message),
        _ok_text(ok_text),
        _ok(false),
        _ok_action(ok_action)
    {}

    const Window& ConfirmationWindowGroup::main_window() const {
        return _popup;
    }

    void ConfirmationWindowGroup::resize() {
        _popup.resize(
            _parent_window.width() / 2 - _popup.width() / 2,
            _parent_window.height() / 2 - _popup.height() / 2,
            _popup.width(),
            _popup.height()
        );
    }

    const std::vector<GameAction> ConfirmationWindowGroup::handle_input(
        const GameState& game_state,
        curses::Input input
    ) {
        switch (input) {
            case curses::Input::quit:
                return {GameAction::close_window};
            case curses::Input::left:
            case curses::Input::right:
                _ok = !_ok;
                return {};
            case curses::Input::enter:
                if (_ok) {
                    return {GameAction::close_window, _ok_action};
                }

                return {GameAction::close_window};
            default:
                return {};
        }
    }

    void ConfirmationWindowGroup::render(const GameState& game_state) {
        curses::hide_cursor();
        _popup.clear();
        _popup.draw_borders();
        _popup.draw(_popup.width() / 2 - strlen(_message) / 2 - 1, 1, _message);

        if (!_ok) _popup.color_on(curses::Color::menu_selection);
        _popup.draw(2, 3, "[Cancel]");
        if (!_ok) _popup.color_off(curses::Color::menu_selection);
        if (_ok) _popup.color_on(curses::Color::menu_selection);
        _popup.draw(_popup.width() - strlen(_ok_text) - 4, 3, '[');
        _popup.draw(_popup.width() - strlen(_ok_text) - 3, 3, _ok_text);
        _popup.draw(_popup.width() - 3, 3, ']');
        if (_ok) _popup.color_off(curses::Color::menu_selection);

        _popup.refresh();
    }
}
