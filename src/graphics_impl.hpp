/**
 * @file: graphics_impl.hpp
 *
 *  created on:  5 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#ifndef __GRAPHICS_IMPL_HPP__
#define __GRAPHICS_IMPL_HPP__

#include "graphics.hpp"

#include <cmath>
#include <cstdint>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <tuple>

#include <SDL2/SDL.h>

namespace mlogo {

namespace graphics {

namespace impl {

static constexpr double EPSILON  { 10e-5 };
static constexpr uint32_t WIN_FLAGS {
    SDL_WINDOW_SHOWN
#ifdef SDL_WINDOW_ALWAYS_ON_TOP
        |SDL_WINDOW_ALWAYS_ON_TOP
#endif
};

inline double zeroif(double val) {
    if(fabs(val)<EPSILON)
        return 0.0;

    return val;
}

struct SDLPath : Path {
    SDLPath(int ox, int oy) {
        addPoint(ox, oy);
    }

    Path &addPoint(int x, int y) override {
        SDL_Point p { x, y };
        points.push_back(std::move(p));
        return *this;
    }

    Path &move(int shift_x, int shift_y) override {
        for(auto &p : points) {
            p.x += shift_x;
            p.y += shift_y;
        }

        return *this;
    }

    Path &rotate(double angle, int ox=0, int oy=0) override {
        double rad = deg2rad(angle);
        double sin_a = zeroif(sin(rad));
        double cos_a = zeroif(cos(rad));

        for(auto &p : points) {
            // move path to the origin
            int x=p.x-ox;
            int y=p.y-oy;

            // rotate path
            double x1 = x * cos_a - y * sin_a;
            double y1 = x * sin_a + y * cos_a;

            // move in original position
            p.x = static_cast<int>(round(x1)) + ox;
            p.y = static_cast<int>(round(y1)) + oy;
        }

        return *this;
    }

    const void *data() const override {
        return points.data();
    };

    size_t size() const override {
        return points.size();
    }

    std::pair<int, int> last() override {
        auto p = points.back();
        return std::make_pair(p.x, p.y);
    }

    std::vector<SDL_Point> points;
};

class SDLWindow : public Window {
public:
    SDLWindow(const std::string &title, int width, int height) {
        using std::stringstream;
        using std::logic_error;

        window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height,
            WIN_FLAGS);

        if(!window) {
            stringstream ss;
            ss << "Window could not be created! SDL_Error: " << SDL_GetError();
            throw logic_error(ss.str());
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        if(!renderer){
            SDL_DestroyWindow(window);
            window = nullptr;

            stringstream ss;
            ss << "Window Renderer could not be created! SDL_Error: " << SDL_GetError();
            throw logic_error(ss.str());
        }
    }

    ~SDLWindow() {
        SDL_DestroyWindow(window);
    }

    Window *background(const Color &color) {
        _background = color;
        return this;
    }

    Window *foreground(const Color &color) {
        _foreground = color;
        return this;
    }

    Window *clear() {
        setColor(_background);
        SDL_RenderClear(renderer);
        setColor(_foreground);
        return this;
    }

    Window *draw(Path *path) {
        SDL_RenderDrawLines(
            renderer,
            static_cast<const SDL_Point*>(path->data()),
            path->size());
        return this;
    }

    Window *setColor(const Color &c) {
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        return this;
    }

    Window *paint() {
        SDL_RenderPresent(renderer);
        return this;
    }

private:
    SDLWindow(const Window &) = delete;
    SDLWindow(Window &&) = delete;

    Window &operator=(const Window &) = delete;
    Window &operator=(Window &&) = delete;

    SDL_Window* window { nullptr };
    SDL_Renderer *renderer { nullptr };
    Color _background { 0, 0, 0 };
    Color _foreground { 255, 255, 255 };
};

} /* ns: impl */

using SDLWindow = impl::SDLWindow;

} /* ns: graphics */

} /* ns: mlogo */


#endif /* __GRAPHICS_IMPL_HPP__ */
