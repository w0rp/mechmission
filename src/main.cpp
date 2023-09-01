#include <entt/entity/registry.hpp>
#include <tuple>

#include "grid.hpp"
#include "movement_system.hpp"
#include "components/point.hpp"
#include "components/mech.hpp"
#include "components/control.hpp"
#include "curses/screen.hpp"
#include "curses/battlefield_window.hpp"
#include "curses/battlefield_help_window.hpp"

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

enum class ActiveScreen {
    none,
    battlefield,
    battlefield_help,
};

std::tuple<ActiveScreen, Point> run_battlefield_window(
    entt::registry& registry,
    Grid& grid,
    MovementSystem& movement_system,
    const Point& initial_cursor
) {
    curses::show_cursor();
    curses::BattlefieldWindow window{initial_cursor};

    while (true) {
        bool input_locked = movement_system.step(registry, grid);

        switch (window.step(registry, grid, input_locked)) {
            case curses::BattlefieldWindowAction::quit:
                return std::make_tuple(
                    ActiveScreen::none,
                    window.cursor()
                );
            case curses::BattlefieldWindowAction::help:
                return std::make_tuple(
                    ActiveScreen::battlefield_help,
                    window.cursor()
                );
            case curses::BattlefieldWindowAction::select:
                movement_system.make_selection(registry, grid, window.cursor());

                break;
            case curses::BattlefieldWindowAction::none:
                break;
        }
    }
}

ActiveScreen run_battlefield_help_window() {
    curses::hide_cursor();
    curses::BattlefieldHelpWindow window;

    while(true) {
        switch (window.step()) {
            case curses::BattlefieldHelpWindowAction::quit:
                return ActiveScreen::battlefield;
            case curses::BattlefieldHelpWindowAction::none:
                break;
        }
    }
}

void run_battlefield() {
    entt::registry registry;
    create_player_unit(
        registry,
        Point(1, 1),
        Mech{
            .number=1,
            .health=50,
            .energy=25,
            .max_energy=25,
        }
    );
    create_player_unit(
        registry,
        Point(2, 3),
        Mech{
            .number=2,
            .health=75,
            .energy=25,
            .max_energy=25,
        }
    );

    Grid grid{5};

    // A system for handling movement in a battlefield.
    MovementSystem movement_system;

    auto active_screen = ActiveScreen::battlefield;
    Point cursor{0, 0};

    while(active_screen != ActiveScreen::none) {
        switch(active_screen) {
            case ActiveScreen::battlefield:
                std::tie(active_screen, cursor) = run_battlefield_window(
                    registry,
                    grid,
                    movement_system,
                    cursor
                );
                break;
            case ActiveScreen::battlefield_help:
                active_screen = run_battlefield_help_window();
                break;
            default:
                active_screen = ActiveScreen::none;
        }
    }
}

int main(int argc, char** argv) {
    curses::start_ui();

    run_battlefield();

    curses::stop_ui();

    return 0;
}
