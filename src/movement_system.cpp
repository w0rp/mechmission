#include "movement_system.hpp"

#include "components/control.hpp"
#include "components/mech.hpp"
#include "components/point.hpp"
#include "pathing.hpp"

MovementSystem::MovementSystem():
    _movement_path(),
    _entity_to_move(entt::null)
{
}

void MovementSystem::make_selection(
    entt::registry& registry,
    const Grid& grid,
    Point point
) {
    auto view = registry.view<Point, const Mech, PlayerControlled>();

    // Select a unit if one is selected.
    for(auto [entity, unit_point, mech]: view.each()) {
        if (unit_point == point) {
            registry.clear<PlayerSelected>();
            registry.emplace<PlayerSelected>(entity);
        }
    }

    // Prevent movement if the selected space has an object on it.
    for(auto [entity, object_point]: registry.view<Point>().each()) {
        if (object_point == point) {
            return;
        }
    }

    auto selected = registry.view<PlayerSelected>();

    // Indicate that we'd like to move an entity to a desired point.
    for(auto [entity, unit_point, mech]: view.each()) {
        if (selected.contains(entity)) {
            registry.clear<PlayerSelected>();

            a_star_movement(
                _movement_path,
                registry,
                grid,
                mech.energy,
                unit_point,
                point
            );
            _entity_to_move = entity;
        }
    }
}

bool MovementSystem::step(entt::registry& registry, const Grid& grid) {
    bool busy = false;

    if (registry.valid(_entity_to_move)) {
        if (!_movement_path.empty()) {
            auto& mech = registry.get<Mech>(_entity_to_move);
            auto current = registry.get<Point>(_entity_to_move);

            // Pop a point in the path to move to and put the entity there.
            auto next = *_movement_path.begin();
            _movement_path.erase(_movement_path.begin());
            registry.replace<Point>(_entity_to_move, next);

            // Decrease mech energy by cost of movement.
            mech.energy -= compute_energy_cost(grid, current, next);

            // Tell UIs we're busy.
            busy = true;
        } else {
            // If we run out of points to move, clear the entity to move to.
            _entity_to_move = entt::null;
        }
    }

    return busy;
}
