#include "game_action.hpp"

#include <cassert>

InvalidGameActionTagDataException::InvalidGameActionTagDataException() {}

const char* InvalidGameActionTagDataException::what() const noexcept {
    return "Invalid GameActionTag value: call another constructor";
}

inline void assert_has_point(GameActionTag tag) {
    assert(tag == GameActionTag::battlefield_select_point);
}

inline void assert_has_no_point(GameActionTag tag) {
    assert(tag != GameActionTag::battlefield_select_point);
}

// Zero-initialize with no action by default.
GameAction::GameAction(): _tag(GameActionTag::none), _data{} {}

// Init the action with a tag and the largest data for the union.
GameAction::GameAction(GameActionTag new_tag): _tag(new_tag), _data{} {
    // Ensure we call this constructor with the correct tags only.
    switch (new_tag) {
    case GameActionTag::battlefield_select_point:
        throw InvalidGameActionTagDataException();
    default:
        break;
    }
}

GameAction::GameAction(GameActionTag new_tag, const Point& point):
    _tag(new_tag),
    _data{point}
{
    // Ensure we call this constructor with the correct tags only.
    switch (new_tag) {
    case GameActionTag::battlefield_select_point:
        break;
    default:
        throw InvalidGameActionTagDataException();
    }
}

GameActionTag GameAction::tag() const {
    return _tag;
}

const Point& GameAction::point() const {
    return std::get<Point>(_data);
}
