#ifndef __MECHMISSION_COMPONENTS_H_
#define __MECHMISSION_COMPONENTS_H_

#include <compare>

struct Point {
    int x;
    int y;

    std::strong_ordering operator<=> (const Point&) const = default;
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

#endif
