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
#include "geometry.hpp"

using namespace std;

namespace mlogo {

namespace turtle {

using Path = geometry::Path;
using GC = graphics::Context;

struct Turtle::_pImpl {
    static constexpr float START_ANGLE { 180.0 };
    static constexpr int TURTLE_HEIGHT { 10 };
    static constexpr int TURTLE_BASE   { 10 };
    static constexpr int TURTLE_HEAD_X { GC::SCREEN_WIDTH / 2 };
    static constexpr int TURTLE_HEAD_Y { (GC::SCREEN_HEIGHT - TURTLE_HEIGHT) / 2};

    _pImpl() : angle(START_ANGLE), turtleSystem(1, TURTLE_HEAD_X, -1, TURTLE_HEAD_Y) {
        createTurle();
        initPaths();
    }

    ~_pImpl() {
        clearPaths();
    }

    void createTurle() {
        turtle = Path(0, TURTLE_HEIGHT/2);
        turtle.push_back(TURTLE_BASE/2, -TURTLE_HEIGHT/2);
        turtle.push_back(-TURTLE_BASE/2, -TURTLE_HEIGHT/2);
        turtle.push_back(0, TURTLE_HEIGHT/2);
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
        paths.push_back(Path(o.first, o.second));
    }

    Turtle::Position lastPos() const { // (T)urtle (S)ystem  (P)osition
        return paths.back().last();
    }

    void moveTurtleTo(const Turtle::Position &o) {
        if(!paths.empty()) {
            auto current = lastPos();
            turtle.translate(o.first - current.first, -1*o.second + current.second);
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
        paths.back().push_back(wDest.first, wDest.second);
    }

    void setAngle(double a) {
        double rotateBy = a - angle;
        int x,y;
        std::tie(x,y) = lastPos(false);

        angle = a;
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

    Path turtle;
    vector<Path> paths;
    double angle;
    double xScrunch { 1 };
    double yScrunch { 1 };
    bool showTurtle { true };
    Mode mode { Mode::WRAP };
    geometry::Reference turtleSystem;
};

} /* ns: turtle */

} /* ns: mlogo */


#endif /* __TURTLE_IMPL_HPP__ */
