#include <chrono>
#include <thread>
#include <memory>

#include <unistd.h>

#include "curses/input.hpp"
#include "game_action.hpp"
#include "game_state.hpp"
#include "movement_system.hpp"
#include "turn_system.hpp"
#include "components/point.hpp"
#include "components/mech.hpp"
#include "curses/screen.hpp"
#include "curses/window_group.hpp"
#include "curses/battlefield_window_group.hpp"
#include "curses/battlefield_help_window_group.hpp"
#include "curses/confirmation_window_group.hpp"

void create_unit(
    entt::registry& registry,
    Point point,
    Mech mech
) {
    const auto unit = registry.create();
    registry.emplace<Point>(unit, point);
    registry.emplace<Mech>(unit, mech);
}

class GameLoopHandler {
    std::vector<std::unique_ptr<curses::WindowGroup>> _window_group_stack;
    GameState _game_state;
    MovementSystem _movement_system;
    TurnSystem _turn_system;
    bool _resize_needed;
    bool _should_quit;

    void _nudge_cursor(const Vector& direction) {
        _movement_system.move_cursor(
            _game_state,
            _game_state.grid_pos() + direction
        );
    }

    void _handle_game_action(GameAction action) {
        auto& current_group = _window_group_stack.back();

        switch(action.tag) {
            case GameActionTag::battlefield_ask_quit:
                _window_group_stack.emplace_back(
                    std::make_unique<curses::ConfirmationWindowGroup>(
                        current_group->main_window(),
                        "Quit game?",
                        "Quit",
                        GameActionTag::battlefield_quit
                    )
                );
                break;
            case GameActionTag::battlefield_quit:
                _should_quit = true;
                // Stop processing further updates.
                return;
            case GameActionTag::battlefield_help:
                _window_group_stack.emplace_back(
                    std::make_unique<curses::BattlefieldHelpWindowGroup>()
                );
                break;
            case GameActionTag::battlefield_move_up:
                // Adjusting x by even/odd y coordinates makes us
                // go up and down the screen instead of diagonally.
                _nudge_cursor({abs(_game_state.grid_pos().y) % 2 == 0, -1});
                break;
            case GameActionTag::battlefield_move_down:
                _nudge_cursor({-abs(_game_state.grid_pos().y) % 2, 1});
                break;
            case GameActionTag::battlefield_move_left:
                _nudge_cursor({-1, 0});
                break;
            case GameActionTag::battlefield_move_right:
                _nudge_cursor({1, 0});
                break;
            case GameActionTag::battlefield_select:
                _movement_system.make_selection(
                    _game_state,
                    _game_state.grid_pos()
                );
                break;
            case GameActionTag::battlefield_select_point:
                _movement_system.make_selection(
                    _game_state,
                    action.point()
                );
                break;
            case GameActionTag::battlefield_ask_end_turn:
                _window_group_stack.emplace_back(
                    std::make_unique<curses::ConfirmationWindowGroup>(
                        current_group->main_window(),
                        "End turn?",
                        "End Turn",
                        GameActionTag::battlefield_end_turn
                    )
                );
                break;
            case GameActionTag::battlefield_end_turn:
                _turn_system.end_turn(_game_state);
                break;
            case GameActionTag::close_window:
                _window_group_stack.pop_back();
                break;
            case GameActionTag::none:
                break;
        }
    }

public:
    GameLoopHandler():
        _window_group_stack(),
        _game_state(),
        _movement_system(),
        _turn_system(),
        _resize_needed(false),
        _should_quit(false)
    {}

    bool should_quit() const noexcept {
        return _should_quit;
    }

    void setup() {
        curses::start_ui();
        _game_state.open_map({
            .grid=Grid(5),
            .grid_pos=Point(0, 0),
            .turn_number=1,
            .player_number=1,
            .players=2,
        });

        Armor basic_armor{
            {
                "Basic Armor", // name
                1, // rank
                100, // health
                100, // max_health
                10, // weight
            },
            10, // protection
        };
        Legs basic_legs{
            {
                "Basic Legs", // name
                1, // rank
                100, // health
                100, // max_health
                10, // weight
            },
            2, // protection
            1.0, // speed_multiplier
        };
        Engine basic_engine{
            {
                "Basic Engine", // name
                1, // rank
                100, // health
                100, // max_health
                10, // weight
            },
            1.0, // speed_multiplier
        };
        Battery battery{
            {
                "Basic Battery", // name
                1, // rank
                100, // health
                100, // max_health
                10, // weight
            },
            100, // energy
            100, // max_energy
        };
        ShieldGenerator basic_shield_generator{
            {
                "Basic Shield Generator", // name
                1, // rank
                100, // health
                100, // max_health
                10, // weight
            },
            20, // energy_cost
            200, // max_strength
        };
        Sensor basic_sensor{
            {
                "Basic Sensor", // name
                1, // rank
                100, // health
                100, // max_health
                10, // weight
            },
            5, // energy_cost
            10, // sensor_range
        };
        Weapon laser_weapon{
            {
                "Basic Laser", // name
                1, // rank
                100, // health
                100, // max_health
                10, // weight
            },
            WeaponType::energy,
            -1, // ammo
            -1, // max_ammo
            50, // energy_cost
            10, // maximum_range
            1.0, // base_accuracy
            10, // min_physical_damage
            15, // max_physical_damage
            60, // min_shields_damage
            80, // max_shields_damage
            0, // splash_range
            0.0, // splash_damage
        };

        create_unit(
            _game_state.registry(),
            Point(1, 1),
            Mech{
                1, // mech_spec_id
                1, // player_number
                1, // number
                "Jools", // name
                50, // energy
                200, // shield_strength
                50, // frame_weight
                basic_armor,
                basic_legs,
                basic_engine,
                battery,
                basic_shield_generator,
                basic_sensor,
                // weapons
                {laser_weapon},
                // utils
                {},
            }
        );
        create_unit(
            _game_state.registry(),
            Point(2, 3),
            Mech{
                1, // mech_spec_id
                1, // player_number
                2, // number
                "Jops", // name
                50, // energy
                200, // shield_strength
                50, // frame_weight
                basic_armor,
                basic_legs,
                basic_engine,
                battery,
                basic_shield_generator,
                basic_sensor,
                // weapons
                {laser_weapon},
                // utils
                {},
            }
        );
        create_unit(
            _game_state.registry(),
            Point(-2, -1),
            Mech{
                1, // mech_spec_id
                2, // player_number
                1, // number
                "Hercules", // name
                50, // energy
                200, // shield_strength
                50, // frame_weight
                basic_armor,
                basic_legs,
                basic_engine,
                battery,
                basic_shield_generator,
                basic_sensor,
                // weapons
                {laser_weapon},
                // utils
                {},
            }
        );

        // Set up the battlefield window group to start.
        _window_group_stack.emplace_back(
            std::make_unique<curses::BattlefieldWindowGroup>(
                Point(0, 0)
            )
        );
    }

    void teardown() {
        curses::stop_ui();
    }

    void update(double dt) {
        curses::Input input;

        bool movement_busy = _movement_system.update(_game_state, dt);

        while ((input = curses::get_input()) != curses::Input::no_more_input) {
            // If we receive at least one resize event, mark that we need to
            // resize all active window groups.
            if (input == curses::Input::resize) {
                _resize_needed = true;
                continue;
            }

            if (movement_busy) {
                // Don't send input to window groups if movement system is
                // busy.
                continue;
            }

            // Send input only to the window group at the end of the stack.
            for (
                auto action: _window_group_stack.back()
                    ->handle_input(_game_state, input)
            ) {
                _handle_game_action(action);
            }
        }
    }

    void render(double interpolation_amount) {
        // Resize all window groups in order, if needed.
        if (_resize_needed) {
            curses::clear_ui();

            for (auto& window_group: _window_group_stack) {
                window_group->resize();
            }

            _resize_needed = false;
        }

        curses::refresh_ui();

        // Render all window groups in order.
        for (auto& window_group: _window_group_stack) {
            window_group->render(_game_state);
        }
    }
};

int main(int argc, char** argv) {
    const double fps = 60;
    const double dt = 1.0 / fps;
    auto current_time = std::chrono::steady_clock::now();
    double accumulator = 0.0;

    GameLoopHandler loop_handler;

    loop_handler.setup();

    while (!loop_handler.should_quit()) {
        // If we needed to recompute dt for vsync, we would do it at
        // the start of the loop.
        const auto new_time = std::chrono::steady_clock::now();
        double frame_time =
            std::chrono::duration<double>(new_time - current_time)
            .count();
        current_time = new_time;

        // Ensure we don't introduce a lag of game renders by limiting
        // the amount of time we spend computing game world logic.
        if (frame_time > 0.25) {
            frame_time = 0.25;
        }

        accumulator += frame_time;

        while (accumulator >= dt) {
            // If we had to interpolate state, we'd do it here.
            loop_handler.update(dt);
            accumulator -= dt;
        }

        if (accumulator < 0.0) accumulator = 0.0;

        loop_handler.render(accumulator / dt);

        // If a single iteration takes less time than a frame, sleep for a
        // little while so we don't hammer the CPU.
        double iteration_time =
            std::chrono::duration<double>(
                std::chrono::steady_clock::now() - new_time
            )
            .count();

        if (iteration_time < dt) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(int((dt - iteration_time) * 1000))
            );
        }
    }

    loop_handler.teardown();

    return 0;
}
