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
public:
    Grid();
    Grid(int radius);

    int radius() const;
    // Return `true` if a point exists inside of the grid.
    bool contains(const Point& p) const noexcept;
    // Get a reference to a space in the grid.
    //
    // throws std::out_of_range if the point is not in the grid.
    Space& at(const Point& p);
    // Get const a reference to a space in the grid.
    //
    // throws std::out_of_range if the point is not in the grid.
    const Space& at(const Point& p) const;
private:
    int _radius;
    int _rhombus_size;
    std::vector<Space> _spaces;
};

#endif
