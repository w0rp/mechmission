#ifndef __MECHMISSION_GAME_ACTION_H_
#define __MECHMISSION_GAME_ACTION_H_

#include "components/point.hpp"
#include <exception>
#include <variant>
#include <array>

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

// An exception for when we create a GameAction with the wrong data.
class InvalidGameActionTagDataException final : public std::exception {
public:
    InvalidGameActionTagDataException();

    const char* what() const noexcept final;
};

class GameAction {
public:
    GameAction();
    GameActionTag tag() const;

    GameAction(GameActionTag new_tag);
    GameAction(GameActionTag new_tag, const Point& point);

    const Point& point() const;
private:
    GameActionTag _tag;
    std::variant<Point> _data;
};

static constexpr int MAX_GAME_ACTIONS = 4;
using GameActionArray = std::array<GameAction, MAX_GAME_ACTIONS>;

#endif
