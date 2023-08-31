#include "grid.hpp"
#include "components.hpp"
#include "curses/screen.hpp"
#include "curses/battlefield_window.hpp"

#include <entt/entity/registry.hpp>

#include <ncurses.h>


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

void make_selection(entt::registry& registry, Point point) {
    auto view = registry.view<Point, PlayerControlled>();

    // Select a unit if one is selected.
    for(auto [entity, unit_point]: view.each()) {
        if (unit_point == point) {
            registry.clear<PlayerSelected>();
            registry.emplace<PlayerSelected>(entity);
        }
    }

    // Prevent movement if the selected space has an object on it.
    for(auto [entity, object_point]: registry.view<Point>().each()) {
        if (object_point == point) {
            return;
        }
    }

    auto selected = registry.view<PlayerSelected>();

    // Move a unit to a point if one is selected.
    for(auto [entity, unit_point]: view.each()) {
        if (selected.contains(entity)) {
            registry.clear<PlayerSelected>();
            unit_point = point;
        }
    }
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

    curses::start_ui();
    curses::BattlefieldWindow window;

    bool done = false;

    while (!done) {
        switch (window.step(registry, g)) {
            case curses::BattlefieldWindowAction::quit:
                done = true;
                break;
            case curses::BattlefieldWindowAction::select:
                make_selection(
                    registry,
                    window.cursor()
                );

                break;
            case curses::BattlefieldWindowAction::none:
                break;
        }
    }

    curses::stop_ui();

    return 0;
}
