#include <entt/entity/registry.hpp>

#include "grid.hpp"
#include "components.hpp"
#include "curses/screen.hpp"
#include "curses/battlefield_window.hpp"

#include "movement_system.hpp"
void create_player_unit(
    entt::registry& registry,
    Point point,
    Mech mech
) {
    const auto unit = registry.create();
    registry.emplace<Point>(unit, point);
    registry.emplace<Mech>(unit, mech);
    registry.emplace<PlayerControlled>(unit);
}

int main(int argc, char** argv) {
    entt::registry registry;

    create_player_unit(
        registry,
        Point(1, 1),
        Mech{
            .number=1,
            .health=50,
            .max_move_spaces=4,
        }
    );
    create_player_unit(
        registry,
        Point(2, 3),
        Mech{
            .number=2,
            .health=75,
            .max_move_spaces=4,
        }
    );

    Grid g{5};

    // A system for handling movement in a battlefield.
    MovementSystem movement_system;

    curses::start_ui();
    curses::BattlefieldWindow window;

    bool done = false;

    while (!done) {
        bool input_locked = movement_system.step(registry, g);

        switch (window.step(registry, g, input_locked)) {
            case curses::BattlefieldWindowAction::quit:
                done = true;
                break;
            case curses::BattlefieldWindowAction::select:
                movement_system.make_selection(registry, g, window.cursor());

                break;
            case curses::BattlefieldWindowAction::none:
                break;
        }
    }

    curses::stop_ui();

    return 0;
}
