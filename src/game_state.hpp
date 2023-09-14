#ifndef __MECHMISSION_GAME_STATE_H_
#define __MECHMISSION_GAME_STATE_H_

#include <entt/entity/registry.hpp>

#include "grid.hpp"

class GameState {
public:
    // Disable copying and assignment.
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;
    // Explicitly use default moves.
    GameState(GameState&&) = default;
    GameState& operator=(GameState&&) = default;

    GameState();
    ~GameState() = default;

    void open_map(
        const Grid& grid,
        const Point& grid_pos,
        int turn_number,
        int player_number,
        int players
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
    // Get the current player number.
    int player_number() const;
    // Update the current player number.
    void set_player_number(int player_number);
    // Get the number of players.
    int players() const;
private:
    entt::registry _registry;
    Grid _grid;
    Point _grid_pos;
    int _turn_number = 1;
    int _player_number = 1;
    int _players = 1;
};

#endif
