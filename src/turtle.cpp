/**
 * @file: turle.cpp
 *
 *  created on:  5 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#include "turtle.hpp"

#include <cmath>
#include <vector>

#include "graphics.hpp"

using namespace std;

namespace mlogo {

namespace turtle {

using Path = graphics::Path;
using GC = graphics::Context;

struct Turtle::_pImpl {
    static constexpr float START_ANGLE { 180.0 };
    static constexpr int TURTLE_HEIGHT { 10 };
    static constexpr int TURTLE_BASE   { 10 };
    static constexpr int TURTLE_HEAD_X { GC::SCREEN_WIDTH / 2 };
    static constexpr int TURTLE_HEAD_Y { (GC::SCREEN_HEIGHT - TURTLE_HEIGHT) / 2};

    _pImpl() : angle(START_ANGLE) {
        createTurle();
        initPaths();
    }

    ~_pImpl() {
        delete turtle;
        clearPaths();
    }

    void createTurle() {
        if(turtle) delete turtle;

        turtle = GC::instance().createPath(TURTLE_HEAD_X, TURTLE_HEAD_Y);
        turtle->addPoint(TURTLE_HEAD_X - TURTLE_BASE/2, TURTLE_HEAD_Y+TURTLE_HEIGHT);
        turtle->addPoint(TURTLE_HEAD_X + TURTLE_BASE/2, TURTLE_HEAD_Y+TURTLE_HEIGHT);
        turtle->addPoint(TURTLE_HEAD_X, TURTLE_HEAD_Y);
    }

    void initPaths() {
        clearPaths();
        newPath();
    }

    void clearPaths() {
        for(auto p : paths) delete p;
        paths.clear();
    }

    void newPath() {
        paths.push_back(GC::instance().createPath(GC::SCREEN_WIDTH/2, GC::SCREEN_HEIGHT/2));
    }

    Path *turtle { nullptr };
    vector<Path *> paths;
    double angle;
};

Turtle::Turtle() :
    impl (new _pImpl) {
    render();
}

Turtle::~Turtle() {
    delete impl;
}

Turtle &Turtle::home() {
    impl->angle = _pImpl::START_ANGLE;
    impl->createTurle();
    if(!impl->paths.back()->empty()) impl->newPath();
    render();

    return *this;
}

Turtle &Turtle::clear() {
    impl->initPaths();
    render();
    return *this;
}

Turtle &Turtle::forward(int steps) {
    int x, y;
    auto current = impl->paths.back();
    std::tie(x, y) = current->last();
    double alpha = graphics::deg2rad(impl->angle);

    int sx = sin(alpha) * steps;
    int sy = cos(alpha) * steps;
    x += sx;
    y += sy;

    impl->turtle->move(sx, sy);
    current->addPoint(x, y);

    render();
    return *this;
}

Turtle &Turtle::right(double angle) {
    int x,y;
    std::tie(x,y) = impl->paths.back()->last();
    impl->angle -= angle;
    impl->turtle->rotate(angle, x, y);
    render();
    return *this;
}

void Turtle::render() {
    GC::instance().window()->clear();
    GC::instance().window()->draw(impl->turtle);
    for(auto p : impl->paths) GC::instance().window()->draw(p);
    GC::instance().window()->paint();
}

} /* ns: turtle */

} /* ns: mlogo */
