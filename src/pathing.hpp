#ifndef __MECHMISSION_PATHING_H_
#define __MECHMISSION_PATHING_H_

#include <vector>

#include <entt/entity/registry.hpp>

#include "grid.hpp"
#include "components.hpp"

std::vector<Point> a_star_movement(
    const entt::registry& registry,
    const Grid& grid,
    Point start,
    Point goal
);

#endif
