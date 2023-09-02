#include "game_state.hpp"

GameState::GameState():
    _registry(),
    _grid(),
    _turn_number(1)
{}

void GameState::open_map(
    const Grid& grid,
    const Point& grid_pos,
    int turn_number
) {
    _grid = grid;
    _grid_pos = grid_pos;
    _turn_number = turn_number;
}

entt::registry& GameState::registry() {
    return _registry;
}

const entt::registry& GameState::registry() const {
    return _registry;
}

Grid& GameState::grid() {
    return _grid;
}

const Grid& GameState::grid() const {
    return _grid;
}

const Point& GameState::grid_pos() const {
    return _grid_pos;
}

void GameState::set_grid_pos(const Point& grid_pos) {
    _grid_pos = grid_pos;
}

int GameState::turn_number() const {
    return _turn_number;
}

void GameState::set_turn_number(int turn_number) {
    _turn_number = turn_number;
}
