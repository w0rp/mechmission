#include "window.hpp"

#include <ncurses.h>

namespace curses {
    Window::Window(int x, int y, int width, int height) noexcept:
        _window{newwin(height, width, y, x)}
    {
    }

    Window::~Window() {
        delwin((WINDOW*)_window);
    }

    void Window::resize(int x, int y, int width, int height) noexcept {
        wresize((WINDOW*)_window, height, width);
        mvwin((WINDOW*)_window, y, x);
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

    void Window::draw(int x, int y, const char* str) noexcept {
        mvwprintw((WINDOW*)_window, y, x, "%s", str);
    }

    void Window::drawf(int x, int y, const char* fmt, ...) noexcept {
        wmove((WINDOW*)_window, y, x);
        va_list args;
        va_start(args, fmt);
        vw_printw((WINDOW*)_window, fmt, args);
        va_end(args);
    }

    void Window::draw_borders() noexcept {
        box((WINDOW*)_window, 0, 0);
    }

    void Window::clear() noexcept {
        wclear((WINDOW*)_window);
    }

    void Window::refresh() noexcept {
        wrefresh((WINDOW*)_window);
    }
}
