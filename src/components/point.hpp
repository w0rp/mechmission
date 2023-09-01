#ifndef __MECHMISSION_COMPONENTS_POINT_H_
#define __MECHMISSION_COMPONENTS_POINT_H_

#include <compare>

struct Point {
    int x;
    int y;

    std::strong_ordering operator<=> (const Point&) const = default;
};

#endif
