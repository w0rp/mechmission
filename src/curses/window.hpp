#ifndef __MECHMISSION_CURSES_WINDOW_H_
#define __MECHMISSION_CURSES_WINDOW_H_

// A copy of the PRINTFLIKE
#ifndef GCC_PRINTFLIKE
#ifndef printf
#define GCC_PRINTFLIKE(fmt,var) __attribute__((format(printf,fmt,var)))
#else
#define GCC_PRINTFLIKE(fmt,var) /*nothing*/
#endif
#endif

#include "color.hpp"

namespace curses {
    class Window {
        void* _parent_window;
        void* _window;
    public:
        // Create a new window with coordinates on the screen.
        Window(int x, int y, int width, int height) noexcept;
        // Create a new child window with coordinates on the screen.
        Window(const Window& parent, int x, int y, int width, int height) noexcept;
        ~Window();

        void resize(int x, int y, int width, int height) noexcept;
        int width() const noexcept;
        int height() const noexcept;
        void position_cursor(int x, int y) noexcept;
        // Fill a window with a given color.
        void fill(curses::Color color) noexcept;
        void color_on(curses::Color color) noexcept;
        void color_off(curses::Color color) noexcept;
        void draw(int x, int y, char chr) noexcept;
        void draw(int x, int y, const char* str) noexcept;
        // Draw characters with a format string.
        void drawf(int x, int y, const char* fmt, ...) noexcept GCC_PRINTFLIKE(4,5);
        void draw_borders() noexcept;
        void clear() noexcept;
        void refresh() noexcept;
    };
}

#endif
