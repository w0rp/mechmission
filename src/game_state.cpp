#include "game_state.hpp"

GameState::GameState():
    _registry(),
    _grid(),
    _turn_number(1),
    _player_number(1),
    _players(1)
{}

void GameState::open_map(const GameStateMapParameters& params) {
    _grid = params.grid;
    _grid_pos = params.grid_pos;
    _turn_number = params.turn_number;
    _player_number = params.player_number;
    _players = params.players;
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

int GameState::player_number() const {
    return _player_number;
}

void GameState::set_player_number(int player_number) {
    _player_number = player_number;
}

int GameState::players() const {
    return _players;
}
