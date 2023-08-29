#include "battlefield_window.hpp"

#include <cmath>

#include "../grid.hpp"
#include "screen.hpp"

namespace curses {
    const int HUD_WIDTH = 20;

    BattlefieldWindow::BattlefieldWindow():
        _hud(0, 0, HUD_WIDTH, curses::screen_height()),
        _field(HUD_WIDTH, 0, curses::screen_width() - HUD_WIDTH, curses::screen_height()),
        _field_x(0),
        _field_y(0)
    {
        // Draw initial borders when window is created.
        _hud.draw_borders();
    }

    // Draw hexes like so:
    //
    //  . .
    // . . .
    //  . .
    void BattlefieldWindow::_draw_hexes(const Grid& grid, int grid_x, int grid_y) {
        _field.clear();
        const int width = _field.width();
        const int height = _field.height();
        const int half_height = height / 2;
        const int offset_x = floor((width - height) / 4.0);

        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                if (i % 2 == j % 2) {
                    const int x = grid_x + (i - j) / 2 - offset_x;
                    const int y = grid_y + j - half_height;

                    if (
                        x >= 0
                        && x < grid.size()
                        && y >= 0
                        && y < grid.size()
                    ) {
                        _field.draw(i, j, '.');
                    }
                }
            }
        }

        // Search for first hex cell the cursor could lie in.
        int cursor_i = 0;
        int cursor_j = 0;

        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                if (i % 2 == j % 2) {
                    const int x = (i - j) / 2 - offset_x;
                    const int y = j - half_height;

                    if (x >= 0 && y >= 0) {
                        cursor_i = i;
                        cursor_j = j;
                        goto cursor_found;
                    }
                }
            }
        }
        cursor_found:

        _field.position_cursor(cursor_i, cursor_j);
    }

    // Redraw window borders and such when the terminal is resized.
    void BattlefieldWindow::_resize_windows() {
        auto width = curses::screen_width();
        auto height = curses::screen_height();

        _hud.resize(0, 0, HUD_WIDTH, height);
        _field.resize(HUD_WIDTH, 0, width - 20, height);

        curses::clear_ui();
        _hud.clear();
        _field.clear();
        _hud.draw_borders();
    }

    inline void _keep_coordinates_inside_grid(int& x, int& y, const Grid& grid) {
        if (y < 0) {
            y = 0;
        } else if (y > grid.size() - 1) {
            y = grid.size() - 1;
        }

        if (x < 0) {
            x = 0;
        } else if (x > grid.size() - 1) {
            x = grid.size() - 1;
        }
    }

    BattlefieldWindowAction BattlefieldWindow::step(const Grid& grid) {
        _keep_coordinates_inside_grid(_field_x, _field_y, grid);
        _draw_hexes(grid, _field_x, _field_y);

        // Draw the coordinates atop the HUD.
        _hud.drawf(3, 0, "x:%04d,y:%04d", _field_x, _field_y);

        // The UI and windows need to be refreshed constantly.
        curses::refresh_ui();
        _hud.refresh();
        _field.refresh();

        auto action = BattlefieldWindowAction::none;

        switch (curses::get_input()) {
            case curses::Input::resize:
                _resize_windows();
                break;
            case curses::Input::up:
                --_field_y;
                break;
            case curses::Input::down:
                ++_field_y;
                break;
            case curses::Input::left:
                --_field_x;
                break;
            case curses::Input::right:
                ++_field_x;
                break;
            case curses::Input::quit:
                action = BattlefieldWindowAction::quit;
                break;
            case curses::Input::unknown:
                break;
        }

        return action;
    }
}
