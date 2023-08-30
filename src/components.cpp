#include "components.hpp"

#include <cmath>

double distance(const Point& p1, const Point& p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

bool operator==(const Point& p1, const Point& p2) {
    return (p1.x == p2.x && p1.y == p2.y);
}
