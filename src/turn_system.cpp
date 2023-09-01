#include "turn_system.hpp"

#include "components/control.hpp"
#include "components/mech.hpp"
#include "grid.hpp"

// TODO: Add ability to end turn for different players.
void TurnSystem::end_turn(
    entt::registry& registry,
    Grid& grid
) {
    auto view = registry.view<Mech, PlayerControlled>();

    for(auto [entity, mech]: view.each()) {
        // Restore energy to all mechs.
        mech.energy = mech.max_energy;
    }

    // TODO: Increment turn number after all players end turns.
    grid.set_turn_number(grid.turn_number() + 1);
}
