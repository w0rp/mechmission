#include "grid.hpp"
#include "components.hpp"
#include "curses/screen.hpp"
#include "curses/battlefield_window.hpp"

#include <entt/entity/registry.hpp>

#include <ncurses.h>

int main(int argc, char** argv) {
    entt::registry registry;

    const auto unit1 = registry.create();
    registry.emplace<Point>(unit1, Point(1, 1));
    registry.emplace<Mech>(unit1, Mech(1, 50));

    const auto unit2 = registry.create();
    registry.emplace<Point>(unit2, Point(2, 3));
    registry.emplace<Mech>(unit2, Mech(2, 75));

    Grid g{5};

    curses::start_ui();
    curses::BattlefieldWindow window;

    while (window.step(registry, g) != curses::BattlefieldWindowAction::quit) {
    }

    curses::stop_ui();

    return 0;
}
