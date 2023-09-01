#include <algorithm>
#include <cmath>
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
inline double compute_cost(
    const Grid& grid,
    const Point& current,
    const Point& next
) {
    // TODO: Factor in terrain height.
    return 1.0;
}

// Determine if a point can be stepped on or not.
inline bool is_valid_point(
    const entt::registry& registry,
    const Grid& grid,
    const Point& point
) {
    // We can't step on points outside of the grid.
    if (!grid.has(point.x, point.y)) {
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

std::vector<Point> get_computed_path(
    const std::unordered_map<Point, Point>& reverse_direction_map,
    Point last
) {
    // Put the items in a vector and reverse it.
    std::vector<Point> path;
    path.reserve(reverse_direction_map.size());

    while (reverse_direction_map.contains(last)) {
        path.push_back(last);
        last = reverse_direction_map.at(last);
    }

    // Reverse the path back to forwards order again.
    std::reverse(path.begin(), path.end());

    return path;
}

std::vector<Point> a_star_movement(
    const entt::registry& registry,
    const Grid& grid,
    Point start,
    Point goal
) {
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

    std::unordered_map<Point, Point> reverse_direction_map;
    auto last = start;

    while (!que.empty()) {
        const auto [current, current_cost] = que.top();
        last = current;
        que.pop();

        if (current == goal) {
            break;
        }

        for (auto next: neighbors(current)) {
            // Skip points we cannot move to.
            if (!is_valid_point(registry, grid, next)) {
                continue;
            }

            const auto cost = current_cost + compute_cost(grid, current, next);

            if (!costs.contains(next) || cost < costs.at(next)) {
                costs[next] = cost;
                que.emplace(next, cost + heuristic(grid, next, goal));
                reverse_direction_map[next] = current;
            }
        }
    }

    if (last != goal) {
        // Return an empty set if we can't reach the goal.
        return std::vector<Point>();
    }

    return get_computed_path(reverse_direction_map, last);
}
