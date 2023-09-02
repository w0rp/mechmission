#include "grid.hpp"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const Space& s) {
    os << "Space{}";

    return os;
}

// Compute the index for storing a space in the _spaces vector.
// Credit to /u/GavrielBA on Reddit.
int grid_index(int radius, int rhombus_size, int x, int y) {
    // The z-coordinate for cube hex coordinates.
    int z = -x - y;

    if (x >= 0 && y < 0) {
        return x * radius - y;
    }

    if (y >= 0 && z < 0) {
        return rhombus_size + y * radius - z;
    }

    if (z >= 0 && x < 0) {
        return rhombus_size * 2 + z * radius - x;
    }

    return 0;
}

Grid::Grid():
    _radius(0),
    _rhombus_size(0),
    _spaces(1)
    {}

Grid::Grid(int radius):
    _radius(radius),
    _rhombus_size(radius * (radius + 1)),
    _spaces(1 + _rhombus_size * 3)
    {}

int Grid::radius() const {
    return _radius;
}

bool Grid::contains(const Point& p) const noexcept {
    int z = -p.x - p.y;

    return abs(p.x) <= _radius && abs(p.y) <= _radius && abs(z) <= _radius;
}

Space& Grid::at(const Point& p) {
    return _spaces.at(grid_index(_radius, _rhombus_size, p.x, p.y));
}

const Space& Grid::at(const Point& p) const {
    return _spaces.at(grid_index(_radius, _rhombus_size, p.x, p.y));
}
