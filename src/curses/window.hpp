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

namespace curses {
    class Window {
        void* _window;
    public:
        Window(int x, int y, int width, int height);
        ~Window();

        void resize(int x, int y, int width, int height);
        int width();
        int height();
        void position_cursor(int x, int y);
        void draw(int x, int y, char chr);
        void draw(int x, int y, const char* str);
        // Draw characters with a format string.
        void drawf(int x, int y, const char* fmt, ...) GCC_PRINTFLIKE(4,5);
        void draw_borders();
        void clear();
        void refresh();
    };
}

#endif
