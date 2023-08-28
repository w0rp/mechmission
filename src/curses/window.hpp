#ifndef __MECHMISSION_CURSES_WINDOW_H_
#define __MECHMISSION_CURSES_WINDOW_H_

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
        void draw_borders();
        void clear();
        void refresh();
    };
}

#endif
