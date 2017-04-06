/**
 * @file: graphics.cpp
 *
 *  created on:  5 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#include "graphics.hpp"

#include <string>
#include <sstream>
#include <stdexcept>

#include "graphics_impl.hpp"

using namespace std;

namespace mlogo {

namespace graphics {

static const double rad_coef { 0.0174533 };

Context::Context() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        stringstream ss;
        ss << "SDL could not initialize! SDL_Error: " << SDL_GetError();
        throw logic_error(ss.str());
		return;
    }
}

Context::~Context() {
    if(_window) delete _window;

    SDL_Quit();
}

Window *Context::window() {
    if(!_window) {
        _window = new SDLWindow("mlogo", SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    return _window;
}

Path *Context::createPath(int ox, int oy) const {
    return new impl::SDLPath(ox, oy);
}

double deg2rad(double deg) {
    int d = static_cast<int>(deg);
    while(d<0) d+=360;
    d %= 360;
    return d*rad_coef;
}

} /* ns: graphics */

} /* ns: mlogo */
