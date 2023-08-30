#ifndef __MECHMISSION_COMPONENTS_H_
#define __MECHMISSION_COMPONENTS_H_

struct Point {
    int x;
    int y;
};

struct Mech {
    int number;
    int health;
    int max_move_spaces;
};

// A component tracking units a player can control.
struct PlayerControlled {};

// A component tracking units a player has selected.
struct PlayerSelected {};

// Compute if two points are equal.
bool operator==(const Point& p1, const Point& p2);

// Compute the distance between two points.
double distance(const Point& p1, const Point& p2);

#endif
