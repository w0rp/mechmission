#ifndef __MECHMISSION_PATHING_H_
#define __MECHMISSION_PATHING_H_

#include <vector>
#include <set>

#include <entt/entity/registry.hpp>

#include "grid.hpp"
#include "components/point.hpp"

int compute_energy_cost(
    const Grid& grid,
    const Point& current,
    const Point& next
);

// Search for the most efficient path to a space and return it.
// An empty vector is returned if no path can be found.
void a_star_movement(
    std::vector<Point>& path,
    const entt::registry& registry,
    const Grid& grid,
    const int maximum_energy_cost,
    Point current,
    const Point& goal
);

// Search for spaces we that can be reached from a start point.
// The results are stored in a supplied set reference, so memory
// can be re-used for the set.
void bfs_reachable_spaces(
    std::set<Point>& spaces,
    const entt::registry& registry,
    const Grid& grid,
    const int maximum_energy_cost,
    Point current
);

#endif
