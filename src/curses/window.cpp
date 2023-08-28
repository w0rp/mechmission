#include "window.hpp"

#include <ncurses.h>

namespace curses {
    Window::Window(int x, int y, int width, int height):
        _window{newwin(height, width, y, x)}
    {
    }

    Window::~Window() {
        delwin((WINDOW*)_window);
    }

    void Window::resize(int x, int y, int width, int height) {
        wresize((WINDOW*)_window, height, width);
        mvwin((WINDOW*)_window, y, x);
    }

    int Window::width() {
       return getmaxx((WINDOW*)_window);
    }

    int Window::height() {
       return getmaxy((WINDOW*)_window);
    }

    void Window::position_cursor(int x, int y) {
        wmove((WINDOW*)_window, y, x);
    }

    void Window::draw(int x, int y, char chr) {
        wmove((WINDOW*)_window, y, x);
        waddch((WINDOW*)_window, chr);
    }

    void Window::draw(int x, int y, const char* str) {
        mvwprintw((WINDOW*)_window, y, x, "%s", str);
    }

    void Window::draw_borders() {
        box((WINDOW*)_window, 0, 0);
    }

    void Window::clear() {
        wclear((WINDOW*)_window);
    }

    void Window::refresh() {
        wrefresh((WINDOW*)_window);
    }
}
