#include "point.hpp"

#include <cmath>

double distance(const Point& p1, const Point& p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

std::ostream& operator<<(
    std::ostream& os,
    const Point& p
) {
    os << "Point{" << p.x << ", " << p.y << "}";

    return os;
}
