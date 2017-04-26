/**
 * @file: graphics.hpp
 *
 *  created on:  5 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include <cstdint>

#include "geometry.hpp"

namespace mlogo {

namespace graphics {

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
    virtual Window *draw(const geometry::Path &path) = 0;
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

private:
    Context();
    ~Context();

    Context(const Context &) = delete;
    Context(Context &&) = delete;

    Context &operator=(const Context &) = delete;
    Context &operator=(Context &&) = delete;

    Window *_window;
};

}

}


#endif /* __GRAPHICS_HPP__ */
