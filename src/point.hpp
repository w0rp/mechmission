#ifndef __MECHMISSION_POINT_H_
#define __MECHMISSION_POINT_H_

#include <ostream>

struct Point {
    int x;
    int y;
};

// Compute the distance between two points.
double distance(const Point& p1, const Point& p2);

// Implements outputting a point to a stream.
std::ostream& operator<<(std::ostream& os, const Point& p);

#endif
