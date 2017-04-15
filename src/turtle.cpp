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
        newPath(std::make_pair(0, 0));
    }

    void newPath(const Turtle::Position &o) {
        auto origin = toWindowSystem(o);
        paths.push_back(GC::instance().createPath(origin.first, origin.second));
    }

    Turtle::Position lastPos(bool tsp=true) const { // (T)urtle (S)ystem  (P)osition
        auto lastPath = paths.back();
        if(tsp) return toTurtleSystem(lastPath->last());

        return lastPath->last();
    }

    void moveTurtleTo(const Turtle::Position &o) {
        auto current = lastPos();
        turtle->move(o.first - current.first, -1*o.second + current.second);
    }

    void moveTo(const Turtle::Position &o) {
        moveTurtleTo(o);
        newPath(o);
    }

    void walkBy(int offsetX, int offsetY) {
        auto current = lastPos();
        auto dest = std::make_pair(current.first + offsetX, current.second - offsetY);
        auto wDest = toWindowSystem(dest);
        moveTurtleTo(dest);
        paths.back()->addPoint(wDest.first, wDest.second);
    }

    void setAngle(double a) {
        double rotateBy = a - angle;
        int x,y;
        std::tie(x,y) = lastPos(false);

        angle = a;
        turtle->rotate(-rotateBy, x, y);
    }

    Turtle::Position toTurtleSystem(const Turtle::Position &p) const {
        return std::make_pair(p.first - GC::SCREEN_WIDTH/2, -1 * p.second + GC::SCREEN_HEIGHT/2);
    }

    Turtle::Position toWindowSystem(const Turtle::Position &p) const {
        return std::make_pair(p.first + GC::SCREEN_WIDTH/2, -1 * p.second + GC::SCREEN_HEIGHT/2);
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
    impl->newPath();
    render();

    return *this;
}

Turtle &Turtle::clear() {
    impl->clearPaths();
    render();
    return *this;
}

Turtle &Turtle::forward(int steps) {
    double alpha = graphics::deg2rad(impl->angle);
    int sx = sin(alpha) * steps;
    int sy = cos(alpha) * steps;

    impl->walkBy(sx, sy);
    render();

    return *this;
}

Turtle &Turtle::right(double angle) {
    impl->setAngle(impl->angle - angle);
    render();
    return *this;
}

Turtle::Position Turtle::currentPosition() const {
    return impl->lastPos();
}

Turtle &Turtle::currentPosition(const Position &pos) {
    impl->moveTo(pos);
    render();
    return *this;
}

Turtle &Turtle::currentXPosition(int x) {
    auto pos = currentPosition();
    pos.first = x;
    return currentPosition(pos);
}

Turtle &Turtle::currentYPosition(int y) {
    auto pos = currentPosition();
    pos.second = y;
    return currentPosition(pos);
}

double Turtle::heading() const {
    return impl->angle;
}

Turtle &Turtle::heading(double h) {
    impl->setAngle(180 - h);
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
