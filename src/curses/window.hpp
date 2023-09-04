#ifndef __MECHMISSION_CURSES_WINDOW_H_
#define __MECHMISSION_CURSES_WINDOW_H_

// A copy of the PRINTFLIKE
#ifndef GCC_PRINTFLIKE
#ifndef printf
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define GCC_PRINTFLIKE(fmt,var) __attribute__((format(printf,fmt,var)))
#else
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define GCC_PRINTFLIKE(fmt,var) /*nothing*/
#endif
#endif

#include "color.hpp"
#include "mouse_position.hpp"

// Forward declare the WINDOW type from ncurses.
typedef struct _win_st WINDOW;

namespace curses {
    class Window {
        WINDOW* _parent_window;
        WINDOW* _window;
    public:
        // Create a new window with coordinates on the screen.
        Window(int x, int y, int width, int height) noexcept;
        // Create a new child window with coordinates on the screen.
        Window(const Window& parent, int x, int y, int width, int height) noexcept;
        // Explicitly use default moves.
        Window(Window&&) = default;
        Window& operator=(Window&&) = default;

        ~Window();

        // Disable copying and assignment.
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        void resize(int x, int y, int width, int height) noexcept;
        // Get the x position for the window.
        int x() const noexcept;
        // Get the y position for the window.
        int y() const noexcept;
        // Get the width of the window.
        int width() const noexcept;
        // Get the height of the window.
        int height() const noexcept;
        // Move the cursor to a given position in the window.
        void position_cursor(int x, int y) noexcept;
        // Fill a window with a given color.
        void fill(curses::Color color) noexcept;
        void color_on(curses::Color color) noexcept;
        void color_off(curses::Color color) noexcept;
        // Draw a character.
        void draw(int x, int y, char chr) noexcept;
        // Draw a character with a given color.
        void draw(curses::Color, int x, int y, char chr) noexcept;
        // Draw a string.
        void draw(int x, int y, const char* str) noexcept;
        // Draw a string with a given color.
        void draw(curses::Color, int x, int y, const char* str) noexcept;
        // Draw with a format string.
        void drawf(int x, int y, const char* fmt, ...) noexcept GCC_PRINTFLIKE(4,5);
        // Draw with a format string and a given color.
        void drawf(curses::Color, int x, int y, const char* fmt, ...) noexcept GCC_PRINTFLIKE(5,6);
        void draw_borders() noexcept;
        void clear() noexcept;
        void refresh() noexcept;
        // Get the mouse position for mouse input.
        // If the key pressed isn't for mouse input, {-1, -1} is returned.
        MousePosition get_mouse_position() noexcept;
    };
}

#endif
