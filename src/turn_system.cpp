#include "turn_system.hpp"

#include "components/control.hpp"
#include "components/mech.hpp"

// TODO: Add ability to end turn for different players.
void TurnSystem::end_turn(
    GameState& game_state
) {
    auto& registry = game_state.registry();

    auto view = registry.view<Mech, PlayerControlled>();

    for(auto [entity, mech]: view.each()) {
        // Restore energy to all mechs.
        mech.energy = mech.max_energy;
    }

    // TODO: Increment turn number after all players end turns.
    game_state.set_turn_number(game_state.turn_number() + 1);
}
