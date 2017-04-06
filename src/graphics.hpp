/**
 * @file: graphics.hpp
 *
 *  created on:  5 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include <cstdint>
#include <tuple>

namespace mlogo {

namespace graphics {

struct Path;

struct Color {
    static constexpr uint8_t NO_ALPHA { 255 };

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha = NO_ALPHA) :
        r(r), g(g), b(b), a(alpha) {}

    uint8_t r,g,b,a;
};


class Window {
public:
    virtual ~Window() {}

    virtual Window *background(const Color &color) = 0;
    virtual Window *foreground(const Color &color) = 0;
    virtual Window *clear() = 0;
    virtual Window *draw(Path *path) = 0;
    virtual Window *setColor(const Color &c) = 0;
    virtual Window *paint() = 0;

protected:
    Window() {}

private:
    Window(const Window &) = delete;
    Window(Window &&) = delete;

    Window &operator=(const Window &) = delete;
    Window &operator=(Window &&) = delete;
};

class Context {
public:
    static constexpr int SCREEN_WIDTH { 640 };
    static constexpr int SCREEN_HEIGHT { 480 };

    static Context &instance() {
        static Context _instance;
        return _instance;
    }

    Window *window();
    Path *createPath(int ox, int oy) const;

private:
    Context();
    ~Context();

    Context(const Context &) = delete;
    Context(Context &&) = delete;

    Context &operator=(const Context &) = delete;
    Context &operator=(Context &&) = delete;

    Window *_window;
};

struct Path {
    virtual ~Path() {}

    virtual Path &addPoint(int x, int y) = 0;
    virtual Path &move(int shift_x, int shift_y) = 0;
    virtual Path &rotate(double angle, int ox=0, int oy=0) = 0;

    virtual const void *data() const = 0;
    virtual size_t size() const = 0;
    virtual std::pair<int, int> last() = 0;
    virtual bool empty() const { return size() == 0; }
};

double deg2rad(double deg);

}

}


#endif /* __GRAPHICS_HPP__ */
