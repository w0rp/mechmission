#include "window.hpp"
#include "input.hpp"

#include <ncurses.h>

namespace curses {
    Window::Window(int x, int y, int width, int height) noexcept:
        _parent_window(nullptr),
        _window{newwin(height, width, y, x)}
    {
    }

    Window::Window(const Window& parent, int x, int y, int width, int height) noexcept:
        _parent_window(parent._window),
        _window{derwin((WINDOW*)_parent_window, height, width, y, x)}
    {
    }

    Window::~Window() {
        delwin((WINDOW*)_window);
    }

    void Window::resize(int x, int y, int width, int height) noexcept {
        wresize((WINDOW*)_window, height, width);

        if (_parent_window) {
            // mvderwin is supposed to move a window inside of a parent
            // correctly, but it doesn't appear to work properly, so we can do
            // it ourselves.
            //
            // This doesn't account for grandchild windows.
            mvwin(
                (WINDOW*)_window,
                y + getbegy((WINDOW*)_parent_window),
                x + getbegx((WINDOW*)_parent_window)
            );
        } else {
            mvwin((WINDOW*)_window, y, x);
        }
    }

    int Window::x() const noexcept {
       return getbegx((WINDOW*)_window);
    }

    int Window::y() const noexcept {
       return getbegy((WINDOW*)_window);
    }

    int Window::width() const noexcept {
       return getmaxx((WINDOW*)_window);
    }

    int Window::height() const noexcept {
       return getmaxy((WINDOW*)_window);
    }

    void Window::position_cursor(int x, int y) noexcept {
        wmove((WINDOW*)_window, y, x);
    }

    void Window::fill(curses::Color color) noexcept {
        // FIXME: We can't set other colours after we use this?
        wbkgd((WINDOW*)_window, COLOR_PAIR(int(color)));
    }

    void Window::color_on(curses::Color color) noexcept {
        wattron((WINDOW*)_window, COLOR_PAIR(int(color)));
    }

    void Window::color_off(curses::Color color) noexcept {
        wattroff((WINDOW*)_window, COLOR_PAIR(int(color)));
    }

    void Window::draw(int x, int y, char chr) noexcept {
        wmove((WINDOW*)_window, y, x);
        waddch((WINDOW*)_window, chr);
    }

    void Window::draw(curses::Color color, int x, int y, char chr) noexcept {
        color_on(color);
        draw(x, y, chr);
        color_off(color);
    }

    void Window::draw(int x, int y, const char* str) noexcept {
        mvwprintw((WINDOW*)_window, y, x, "%s", str);
    }

    void Window::draw(curses::Color color, int x, int y, const char* str) noexcept {
        color_on(color);
        draw(x, y, str);
        color_off(color);
    }

    void Window::drawf(int x, int y, const char* fmt, ...) noexcept {
        wmove((WINDOW*)_window, y, x);
        va_list args;
        va_start(args, fmt);
        vw_printw((WINDOW*)_window, fmt, args);
        va_end(args);
    }

    void Window::drawf(curses::Color color, int x, int y, const char* fmt, ...) noexcept {
        color_on(color);
        wmove((WINDOW*)_window, y, x);
        va_list args;
        va_start(args, fmt);
        vw_printw((WINDOW*)_window, fmt, args);
        va_end(args);
        color_off(color);
    }

    void Window::draw_borders() noexcept {
        box((WINDOW*)_window, 0, 0);
    }

    void Window::clear() noexcept {
        wclear((WINDOW*)_window);
    }

    void Window::refresh() noexcept {
        if (_parent_window) {
            touchwin((WINDOW*)_parent_window);
        }

        wrefresh((WINDOW*)_window);
    }

    MousePosition Window::get_mouse_position() noexcept {
        auto pos = get_last_mouse_position();

        // TODO: Are coordinates wrong for popups inside of windows?
        //
        // Translate coordinates inside of the window.
        pos.x -= x();
        pos.y -= y();

        // If the coordinates aren't inside the bounds of the window, then
        // set them to -1.
        if (
            pos.x < 0
            || pos.y < 0
            || pos.x >= width()
            || pos.y >= height()
        ) {
            pos.x = -1;
            pos.y = -1;
        }

        return pos;
    }
}
