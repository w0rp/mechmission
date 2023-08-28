#include "grid.hpp"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const Space& s) {
    os << "Space{color: " << s.color << "}";

    return os;
}

Grid::Grid(int size):
    _spaces(int(size*size)),
    _size(size)
    {}

Space& Grid::get(int row, int col) {
    return _spaces[row * _size + col];
}

int Grid::size() const {
    return _size;
}

const Space& Grid::get(int row, int col) const {
    return _spaces[row * _size + col];
}
