/**
 * @file: turtle_impl.hpp
 *
 *  created on:  17 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#ifndef __TURTLE_IMPL_HPP__
#define __TURTLE_IMPL_HPP__

#include "turtle.hpp"

#include <cmath>
#include <vector>
#include <iostream>

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
        if(!paths.empty()) {
            auto current = lastPos();
            turtle->move(o.first - current.first, -1*o.second + current.second);
        }
    }

    void moveTo(const Turtle::Position &o) {
        moveTurtleTo(o);
        newPath(o);
    }

    void walkBy(int offsetX, int offsetY) {
        auto current = lastPos();
        auto dest = std::make_pair(
            current.first + offsetX*xScrunch,
            current.second - offsetY*yScrunch);

        switch(mode) {
        case Mode::WINDOW: walkTo(dest); break;
        case Mode::WRAP: wrapPathTo(current, dest); break;
        case Mode::FENCE: fencePathTo(current, dest); break;
        }
    }

    void walkTo(const Turtle::Position &pos) {
        auto wDest = toWindowSystem(pos);
        moveTurtleTo(pos);
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

    void wrapPathTo(const Turtle::Position &start, const Turtle::Position &dest) {
        auto leftTop = toTurtleSystem(std::make_pair(0, 0));
        auto rightBottom = toTurtleSystem(std::make_pair((int)GC::SCREEN_WIDTH, (int)GC::SCREEN_HEIGHT));

        /* TODO Handling path wrapping */
        walkTo(dest);
    }

    void fencePathTo(const Turtle::Position &start, const Turtle::Position &dest) {
        /* TODO Handling path wrapping */
        walkTo(dest);
    }

    Path *turtle { nullptr };
    vector<Path *> paths;
    double angle;
    double xScrunch { 1 };
    double yScrunch { 1 };
    bool showTurtle { true };
    Mode mode { Mode::WRAP };
};

} /* ns: turtle */

} /* ns: mlogo */


#endif /* __TURTLE_IMPL_HPP__ */
