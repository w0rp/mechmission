#include "point.hpp"

Point operator+(const Point& p, const Vector& v) {
    return Point{p.x + v.x, p.y + v.y};
}

Point operator+(const Vector& v, const Point& p) {
    return Point{v.x + p.x, v.y + p.y};
}

Vector operator+(const Vector& v1, const Vector& v2) {
    return Vector{v1.x + v2.x, v1.y + v2.y};
}

Vector operator-(const Vector& v1, const Vector& v2) {
    return Vector{v1.x - v2.x, v1.y - v2.y};
}

Point operator-(const Point& p, const Vector& v) {
    return Point{p.x - v.x, p.y - v.y};
}
