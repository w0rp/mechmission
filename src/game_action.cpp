#include "game_action.hpp"

#include <cassert>

inline void assert_has_point(GameActionTag tag) {
    assert(tag == GameActionTag::battlefield_select_point);
}

inline void assert_has_no_point(GameActionTag tag) {
    assert(tag != GameActionTag::battlefield_select_point);
}

// Zero-initialize with no action by default.
GameAction::GameAction(): _tag(GameActionTag::none), _point{} {}

// Init the action with a tag and the largest data for the union.
GameAction::GameAction(GameActionTag new_tag): _tag(new_tag), _point{} {
    // Do not allow tags requiring data to be set without the data.
    assert_has_no_point(new_tag);
}

GameAction::GameAction(GameActionTag new_tag, const Point& point):
    _tag(new_tag),
    _point{point}
{
    assert_has_point(new_tag);
}

GameActionTag GameAction::tag() const {
    return _tag;
}

const Point& GameAction::point() const {
    assert_has_point(_tag);

    return _point;
}
