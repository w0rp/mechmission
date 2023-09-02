#ifndef __MECHMISSION_GAME_STATE_H_
#define __MECHMISSION_GAME_STATE_H_

#include <entt/entity/registry.hpp>

#include "grid.hpp"

class GameState {
    entt::registry _registry;
    Grid _grid;
    Point _grid_pos;
    int _turn_number;
public:
    // Disable copying and assignment.
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

    GameState();

    void open_map(
        const Grid& grid,
        const Point& grid_pos,
        int turn_number
    );
    entt::registry& registry();
    const entt::registry& registry() const;
    Grid& grid();
    const Grid& grid() const;
    // Get the current position for moving around the grid.
    const Point& grid_pos() const;
    // Set the position in the grid to move to.
    void set_grid_pos(const Point& grid_pos);
    // Get the current turn number.
    int turn_number() const;
    // Update the current turn number.
    void set_turn_number(int turn_number);
};

#endif
