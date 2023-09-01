#ifndef __MECHMISSION_MOVEMENT_SYSTEM_H_
#define __MECHMISSION_MOVEMENT_SYSTEM_H_

#include <vector>

#include <entt/entity/registry.hpp>

#include "components/point.hpp"
#include "grid.hpp"
#include "entt/entity/fwd.hpp"

class MovementSystem {
    std::vector<Point> _movement_path;
    entt::entity _entity_to_move;

public:
    MovementSystem();

    void make_selection(entt::registry& registry, const Grid& grid, Point point);
    bool step(entt::registry& registry, const Grid& grid);
};

#endif
