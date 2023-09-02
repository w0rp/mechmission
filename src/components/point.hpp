#ifndef __MECHMISSION_COMPONENTS_POINT_H_
#define __MECHMISSION_COMPONENTS_POINT_H_

#include <compare>

struct Vector {
    int x;
    int y;

    std::strong_ordering operator<=> (const Vector&) const = default;
};

struct Point {
    int x;
    int y;

    std::strong_ordering operator<=> (const Point&) const = default;
};

// Add a Vector to a Point.
Point operator+(const Point& p, const Vector& v);
// Add a Point to a Vector.
Point operator+(const Vector& v, const Point& p);
// Add a Vector to a Vector.
Vector operator+(const Vector& v1, const Vector& v2);

// Subtract a Vector from a Vector.
Vector operator-(const Vector& v1, const Vector& v2);
// Subtract a Vector from a Point.
Point operator-(const Point& p, const Vector& v);

#endif
