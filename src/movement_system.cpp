#include "movement_system.hpp"

#include "components/control.hpp"
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
    auto view = registry.view<Point, PlayerControlled>();

    // Select a unit if one is selected.
    for(auto [entity, unit_point]: view.each()) {
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
    for(auto [entity, unit_point]: view.each()) {
        if (selected.contains(entity)) {
            registry.clear<PlayerSelected>();

            _movement_path = a_star_movement(registry, grid, unit_point, point);
            _entity_to_move = entity;
        }
    }
}

bool MovementSystem::step(entt::registry& registry, const Grid& grid) {
    bool busy = false;

    if (registry.valid(_entity_to_move)) {
        if (!_movement_path.empty()) {
            // Pop a point in the path to move to and put the entity there.
            auto point = *_movement_path.begin();
            _movement_path.erase(_movement_path.begin());
            registry.replace<Point>(_entity_to_move, point);

            // Tell UIs we're busy.
            busy = true;
        } else {
            // If we run out of points to move, clear the entity to move to.
            _movement_path = std::vector<Point>();
            _entity_to_move = entt::null;
        }
    }

    return busy;
}
