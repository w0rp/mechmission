#include "game_action.hpp"

#include <cassert>

inline void assert_has_point(GameActionTag tag) {
    assert(tag == GameActionTag::battlefield_select_point);
}

inline void assert_has_no_point(GameActionTag tag) {
    assert(tag != GameActionTag::battlefield_select_point);
}

// Init the action with a tag and the largest data for the union.
GameAction::GameAction(GameActionTag new_tag): tag(new_tag), _point{} {
    // Do not allow tags requiring data to be set without the data.
    assert_has_no_point(new_tag);
}

GameAction::GameAction(GameActionTag new_tag, const Point& point):
    tag(new_tag),
    _point{point}
{
    assert_has_point(new_tag);
}

const Point& GameAction::point() const {
    assert_has_point(tag);

    return _point;
}
