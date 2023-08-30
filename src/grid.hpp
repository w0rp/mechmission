#ifndef __MECHMISSION_GRID_H_
#define __MECHMISSION_GRID_H_

#include <vector>
#include <ostream>

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
    // Return `true` if coordinates exist inside of the grid.
    bool has(int x, int y) const;
    // Get a reference to a space in the grid.
    Space& get(int x, int y);
    // Get const a reference to a space in the grid.
    const Space& get(int x, int y) const;
};

#endif
