#ifndef __MECHMISSION_GRID_H_
#define __MECHMISSION_GRID_H_

#include <vector>
#include <ostream>

struct Space {
    int color;
};

// Implements outputting a space to a stream.
std::ostream& operator<<(std::ostream& os, const Space& s);

class Grid {
    std::vector<Space> _spaces;
    int _size;
public:
    Grid(int size);

    Space& get(int row, int col);
    int size() const;
    const Space& get(int row, int col) const;
};

#endif
