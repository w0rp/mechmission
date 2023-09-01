#ifndef __MECHMISSION_TURN_SYSTEM_H_
#define __MECHMISSION_TURN_SYSTEM_H_

#include <entt/entity/registry.hpp>

#include "grid.hpp"

class TurnSystem {
public:
    int number() const;
    void end_turn(entt::registry& registry, Grid& grid);
};

#endif
