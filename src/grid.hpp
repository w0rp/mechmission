#ifndef __MECHMISSION_GRID_H_
#define __MECHMISSION_GRID_H_

#include <vector>
#include <ostream>

#include "components/point.hpp"

struct Space {
};

// Implements outputting a space to a stream.
std::ostream& operator<<(std::ostream& os, const Space& s);

class Grid {
    int _radius;
    int _rhombus_size;
    std::vector<Space> _spaces;
public:
    Grid(int radius);

    int radius() const;
    // Return `true` if a point exists inside of the grid.
    bool has(const Point& p) const noexcept;
    // Get a reference to a space in the grid.
    Space& get(const Point& p);
    // Get const a reference to a space in the grid.
    const Space& get(const Point& p) const;
};

#endif
