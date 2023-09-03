#ifndef __MECHMISSION_GAME_ACTION_H_
#define __MECHMISSION_GAME_ACTION_H_

#include "components/point.hpp"

enum class GameActionTag {
    none,
    close_window,
    battlefield_ask_quit,
    battlefield_quit,
    battlefield_select,
    battlefield_select_point,
    battlefield_ask_end_turn,
    battlefield_end_turn,
    battlefield_move_up,
    battlefield_move_down,
    battlefield_move_left,
    battlefield_move_right,
    battlefield_help,
};

class GameAction {
public:
    const GameActionTag tag;
private:
    union {
        Point _point;
    };
public:
    // You should not be able to default construct a tag.
    GameAction() = delete;

    GameAction(GameActionTag new_tag);
    GameAction(GameActionTag new_tag, const Point& point);

    const Point& point() const;
};

#endif
