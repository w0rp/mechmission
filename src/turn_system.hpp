#ifndef __MECHMISSION_TURN_SYSTEM_H_
#define __MECHMISSION_TURN_SYSTEM_H_

#include "game_state.hpp"

class TurnSystem {
public:
    int number() const;
    void end_turn(GameState& game_state);
};

#endif
