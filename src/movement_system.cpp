#include "movement_system.hpp"

#include "components/control.hpp"
#include "components/mech.hpp"
#include "components/point.hpp"
#include "pathing.hpp"

// Required seconds to pass for each step of movement.
const double required_movement_t = 0.125;

MovementSystem::MovementSystem():
    _movement_path(),
    _entity_to_move(entt::null),
    _remaining_movement_t(required_movement_t)
{
}

void MovementSystem::move_cursor(GameState& game_state, const Point& new_pos) {
    if (game_state.grid().contains(new_pos)) {
        game_state.set_grid_pos(new_pos);
    }
}

void MovementSystem::make_selection(GameState& game_state, const Point& point) {
    // Always move the cursor to the selection point.
    // There may or may not be something to select there.
    move_cursor(game_state, point);

    auto& registry = game_state.registry();
    auto view = registry.view<Point, const Mech>();

    // Select a unit if one is selected.
    for(auto [entity, unit_point, mech]: view.each()) {
        if (
            mech.player_number == game_state.player_number()
            && unit_point == point
        ) {
            registry.clear<ActingUnit>();
            registry.emplace<ActingUnit>(entity);
        }
    }

    // Prevent movement if the selected space has an object on it.
    for(auto [entity, object_point]: registry.view<Point>().each()) {
        if (object_point == point) {
            return;
        }
    }

    auto selected = registry.view<ActingUnit>();

    // Indicate that we'd like to move an entity to a desired point.
    for(auto [entity, unit_point, mech]: view.each()) {
        if (selected.contains(entity)) {
            registry.clear<ActingUnit>();

            a_star_movement(
                _movement_path,
                game_state.registry(),
                game_state.grid(),
                mech.energy,
                unit_point,
                point
            );
            _entity_to_move = entity;
        }
    }
}

bool MovementSystem::update(GameState& game_state, double dt) {
    auto& registry = game_state.registry();

    bool busy = false;

    if (registry.valid(_entity_to_move)) {
        if (!_movement_path.empty()) {
            // Tell UIs we're busy.
            busy = true;

            // Only advance movement when enough time has gone by.
            _remaining_movement_t -= dt;

            if (_remaining_movement_t <= 0) {
                _remaining_movement_t = required_movement_t;

                auto& mech = registry.get<Mech>(_entity_to_move);
                auto current = registry.get<Point>(_entity_to_move);

                // Pop a point in the path to move to and put the entity there.
                auto next = *_movement_path.begin();
                _movement_path.erase(_movement_path.begin());
                registry.replace<Point>(_entity_to_move, next);

                // Decrease mech energy by cost of movement.
                mech.energy -= compute_energy_cost(
                    game_state.grid(),
                    current,
                    next
                );
            }
        } else {
            // If we run out of points to move, clear the entity to move to.
            _entity_to_move = entt::null;
        }
    }

    return busy;
}
