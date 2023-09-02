#ifndef __MECHMISSION_MOVEMENT_SYSTEM_H_
#define __MECHMISSION_MOVEMENT_SYSTEM_H_

#include <vector>

#include "components/point.hpp"
#include "game_state.hpp"

class MovementSystem {
    std::vector<Point> _movement_path;
    entt::entity _entity_to_move;
    double _remaining_movement_t;

public:
    MovementSystem();

    void move_cursor(GameState& game_state, const Point& new_pos);
    void make_selection(GameState& game_state, const Point& point);
    bool update(GameState& game_state, double dt);
};

#endif
