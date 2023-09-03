#include <algorithm>
#include <cmath>
#include <set>
#include <queue>
#include <array>
#include <unordered_map>

#include <entt/entity/registry.hpp>

#include "grid.hpp"
#include "components/point.hpp"

// Compute neighbors around a hexagonal point.
inline std::array<Point, 6> neighbors(Point p) {
    return {
        Point(p.x + 1, p.y), Point(p.x + 1, p.y - 1), Point(p.x, p.y - 1),
        Point(p.x - 1, p.y), Point(p.x - 1, p.y + 1), Point(p.x, p.y + 1),
    };
};

// A point and a cost for searching.
struct PointCost {
    Point point;
    double cost;
};

// Define how to hash a Point for maps from points.
template<>
struct std::hash<Point>
{
    std::size_t operator()(const Point& p) const noexcept
    {
        std::size_t h1 = std::hash<int>{}(p.x);
        std::size_t h2 = std::hash<int>{}(p.y);

        return h1 ^ (h2 << 1);
    }
};

// Compute the cost between two points assumed to be adjacent.
int compute_energy_cost(
    const Grid& grid,
    const Point& current,
    const Point& next
) {
    // TODO: Factor in terrain height.
    return 1 * 10;
}

// Determine if a point can be stepped on or not.
inline bool is_valid_point(
    const entt::registry& registry,
    const Grid& grid,
    const Point& point
) {
    // We can't step on points outside of the grid.
    if (!grid.contains(point)) {
        return false;
    }

    // We can't step on points occupied by something else.
    auto physical_objects = registry.view<const Point>();

    for(auto [entity, other_point]: physical_objects.each()) {
        if (point == other_point) {
            return false;
        }
    }

    return true;
}

inline double distance(const Point& p1, const Point& p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

inline double heuristic(
    const Grid& grid,
    const Point& current,
    const Point& next
) {
    return distance(current, next);
}

// Compare a point and cost by the cost for queues.
struct PointCostCompare {
    bool operator()(const PointCost& a, const PointCost& b) {
        return a.cost > b.cost;
    }
};

void update_a_star_path(
    std::vector<Point>& path,
    const std::unordered_map<Point, Point>& reverse_direction_map,
    Point last
) {
    // Put the items in a vector and reverse it.
    path.reserve(reverse_direction_map.size());

    while (reverse_direction_map.contains(last)) {
        path.push_back(last);
        last = reverse_direction_map.at(last);
    }

    // Reverse the path back to forwards order again.
    std::reverse(path.begin(), path.end());
}

void a_star_movement(
    std::vector<Point>& path,
    const entt::registry& registry,
    const Grid& grid,
    const int maximum_energy_cost,
    const Point& start,
    const Point& goal
) {
    // Reset the path vector.
    path.clear();

    // A priority queue for points with costs.
    std::priority_queue<
        PointCost,
        std::vector<PointCost>,
        PointCostCompare
    > que;
    que.emplace(start, 0.0);
    // Keep track of costs for each point in a map.
    std::unordered_map<Point, double> costs;
    costs[start] = 0.0;
    // Separately track energy costs for moves.
    std::unordered_map<Point, int> energy_costs;
    energy_costs[start] = 0;

    std::unordered_map<Point, Point> reverse_direction_map;
    auto last = start;

    while (!que.empty()) {
        const auto [current, current_cost] = que.top();
        const auto current_energy_cost = energy_costs.at(current);
        last = current;
        que.pop();

        if (current == goal) {
            break;
        }

        for (auto& next: neighbors(current)) {
            // Skip points we cannot move to.
            if (!is_valid_point(registry, grid, next)) {
                continue;
            }

            const int energy_cost = compute_energy_cost(grid, current, next);
            const int total_energy_cost = current_energy_cost + energy_cost;

            if (total_energy_cost > maximum_energy_cost) {
                // Don't search this path if it will cost too much.
                continue;
            }

            const double cost = current_cost + double(energy_cost);

            if (!costs.contains(next) || cost < costs.at(next)) {
                costs[next] = cost;
                energy_costs[next] = total_energy_cost;
                que.emplace(next, cost + heuristic(grid, next, goal));
                reverse_direction_map[next] = current;
            }
        }
    }

    if (last == goal) {
        update_a_star_path(path, reverse_direction_map, last);
    }
}

void bfs_reachable_spaces(
    std::set<Point>& spaces,
    const entt::registry& registry,
    const Grid& grid,
    const int maximum_energy_cost,
    const Point& start
) {
    // Reset available spaces to include where the unit is.
    spaces.clear();
    spaces.emplace(start);

    // Put the current space in a queue and fan outwards.
    std::queue<Point> que;
    que.emplace(start);
    // Track energy costs for moves.
    std::unordered_map<Point, int> energy_costs;
    energy_costs[start] = 0;

    while (!que.empty()) {
        const auto current = que.front();
        que.pop();
        const auto current_energy_cost = energy_costs.at(current);

        for (auto& next: neighbors(current)) {
            if (
                is_valid_point(registry, grid, next)
                && !spaces.contains(next)
            ) {
                const int energy_cost = compute_energy_cost(grid, current, next);
                const int total_energy_cost = current_energy_cost + energy_cost;

                if (total_energy_cost > maximum_energy_cost) {
                    // Don't include this space if it will cost too much.
                    continue;
                }

                energy_costs[next] = total_energy_cost;
                que.emplace(next);
                spaces.emplace(next);
            }
        }
    }
}
