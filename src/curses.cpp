#include "curses.hpp"

#include "grid.hpp"

namespace curses {
    const int HUD_WIDTH = 20;

    BattlefieldWindow::BattlefieldWindow():
        _hud(0, 0, HUD_WIDTH, curses::screen_height()),
        _field(HUD_WIDTH, 0, curses::screen_width() - HUD_WIDTH, curses::screen_height())
    {
        // Draw initial borders when window is created.
        _hud.draw_borders();
    }

    void BattlefieldWindow::draw_hexes(const Grid& grid, int grid_x, int grid_y) {
        _field.clear();
        const int width = _field.width();
        const int height = _field.height();
        const int half_width = width / 2;
        const int half_height = height / 2;

        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                if (i % 2 == j % 2) {
                    const int x_offset = (i - j) / 2;
                    const int y_offset = j - half_height;
                    const int x = grid_x + x_offset;
                    const int y = grid_y + y_offset;

                    if (
                        x >= 0
                        && x < grid.size()
                        && y >= 0
                        && y < grid.size()
                    ) {
                        auto& space = grid.get(x, y);

                        if (space.color >= 1 && space.color <= 9) {
                            _field.draw(i, j, '0' + space.color);
                        } else if (space.color >= 'a' && space.color <= 'z') {
                            _field.draw(i, j, 'a' + (space.color - 'a'));
                        } else {
                            _field.draw(i, j, '.');
                        }
                    }
                }
            }
        }

        _field.position_cursor(half_width, half_height);
    }

    Input BattlefieldWindow::get_input() {
        //  . .
        // . . .
        //  . .

        // The UI and windows need to be refreshed constantly.
        curses::refresh_ui();
        _hud.refresh();
        _field.refresh();

        auto input = curses::get_input();

        if (input == Input::resize) {
            // Redraw window borders and such when the terminal is resized.
            auto width = curses::screen_width();
            auto height = curses::screen_height();

            _hud.resize(0, 0, HUD_WIDTH, height);
            _field.resize(HUD_WIDTH, 0, width - 20, height);

            curses::clear_ui();
            _hud.clear();
            _field.clear();
            _hud.draw_borders();
        }

        return input;
    }
}
