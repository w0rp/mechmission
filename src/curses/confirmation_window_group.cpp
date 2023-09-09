#include "confirmation_window_group.hpp"
#include "input.hpp"
#include "window_group.hpp"
#include "screen.hpp"

#include <cstring>

namespace curses {
    const int button_row = 3;
    const char* const cancel_button = "[Cancel]";

// Implementation details.
struct ConfirmationWindowGroup::Impl {
    static GameActionArray _get_mouse_selection_action(
        ConfirmationWindowGroup& group
    ) {
        auto [x, y] = group._popup.get_mouse_position();

        if (x >= 0 && y == button_row) {
            // Check if a mouse click happened inside the cancel button.
            if (x >= 2 && x < 2 + int(strlen(cancel_button))) {
                return {GameActionTag::close_window};
            }

            // Check if a mouse click happened inside the "ok" button.
            if (
                x >= group._popup.width() - int(strlen(group._ok_text)) - 4
                && x <= group._popup.width() - 3
            ) {
                return {GameActionTag::close_window, group._ok_action};
            }
        }

        return {};
    }
};

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

    const GameActionArray ConfirmationWindowGroup::handle_input(
        const GameState& game_state,
        curses::Input input
    ) {
        switch (input) {
            case curses::Input::quit:
                return {GameActionTag::close_window};
            case curses::Input::left:
            case curses::Input::right:
                _ok = !_ok;
                return {};
            case curses::Input::enter:
                if (_ok) {
                    return {GameActionTag::close_window, _ok_action};
                }

                return {GameActionTag::close_window};
            case curses::Input::mouse_left_click:
                return Impl::_get_mouse_selection_action(*this);
            default:
                return {};
        }
    }

    void ConfirmationWindowGroup::render(const GameState& game_state) {
        curses::hide_cursor();
        _popup.clear();
        _popup.draw_borders();
        _popup.draw(_popup.width() / 2 - int(strlen(_message)) / 2 - 1, 1, _message);

        if (!_ok) _popup.color_on(curses::Color::menu_selection);
        _popup.draw(2, button_row, cancel_button);
        if (!_ok) _popup.color_off(curses::Color::menu_selection);
        if (_ok) _popup.color_on(curses::Color::menu_selection);
        _popup.draw(_popup.width() - int(strlen(_ok_text)) - 4, button_row, '[');
        _popup.draw(_popup.width() - int(strlen(_ok_text)) - 3, button_row, _ok_text);
        _popup.draw(_popup.width() - 3, button_row, ']');
        if (_ok) _popup.color_off(curses::Color::menu_selection);

        _popup.refresh();
    }
}
