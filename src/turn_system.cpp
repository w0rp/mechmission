#include "turn_system.hpp"

#include "components/mech.hpp"

// TODO: Add ability to end turn for different players.
void TurnSystem::end_turn(
    GameState& game_state
) {
    auto& registry = game_state.registry();

    auto view = registry.view<Mech>();

    for(auto [entity, mech]: view.each()) {
        if (mech.player_number == game_state.player_number()) {
            // Restore energy to all mechs.
            mech.energy = mech.max_energy;
        }
    }

    if (game_state.player_number() < game_state.players()) {
        // Go to the next player if there is one.
        game_state.set_player_number(game_state.player_number() + 1);
    } else {
        // If the last player ended a turn, go to the next turn.
        game_state.set_player_number(1);
        game_state.set_turn_number(game_state.turn_number() + 1);
    }
}
