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
using namespace mlogo::geometry;

namespace mlogo {

namespace turtle {

using GC = graphics::Context;

static constexpr int TURTLE_HEIGHT { 10 };
static constexpr int TURTLE_BASE   { 10 };
static constexpr int TURTLE_HEAD_X { GC::SCREEN_WIDTH / 2 };
static constexpr int TURTLE_HEAD_Y { (GC::SCREEN_HEIGHT - TURTLE_HEIGHT) / 2};

Path createTurle(const Reference &turtleSystem);

struct Turtle::_pImpl {
    _pImpl() :
        turtleSystem(1, TURTLE_HEAD_X, -1, TURTLE_HEAD_Y),
        angle(Angle::Degrees(0)),
        turtlePosition(0,0, turtleSystem),
        _turtle(createTurle(turtleSystem)) {
        initPaths();
    }

    ~_pImpl() {
        clearPaths();
    }

    void initPaths() {
        clearPaths();
        newPath();
    }

    void clearPaths() {
        paths.clear();
    }

    void newPath() {
        newPath(make_pair(0, 0));
    }

    void newPath(const Turtle::Position &o) {
        paths.push_back(Path(toPoint(o)));
    }

    Turtle::Position lastPos() const {
        return toPosition(turtlePosition);
    }

    void moveTo(const Turtle::Position &o) {
        auto current = toPoint(o);

        addPoint(current);
    }

    void walk(int steps) {
        auto current = turtlePosition;

        Point d { 0, steps, turtleSystem};
        d = d.rotate(angle).scale(xScrunch, yScrunch);
        current = current + d;

        addPoint(current);
    }

    void addPoint(const Point &current) {
    	if(paths.empty())
            paths.emplace_back(turtlePosition);

        paths.back().push_back(current);

        switch(mode) {
        case Mode::WINDOW: turtlePosition = current; break;
        case Mode::FENCE: break;
        case Mode::WRAP: wrapLine(current); break;
        }
    }

    void setAngle(double a) {
        setAngle(Angle::Degrees(a));
    }

    void setAngle(Angle a) {
        angle = a;
    }

    Turtle::Position toPosition(const Point &p) const {
        return make_pair(p.x, p.y);
    }

    Turtle::Position toPosition(Point &&p) const {
        return make_pair(p.x, p.y);
    }

    Point toPoint(const Turtle::Position &p) const {
        return Point(p.first, p.second, turtleSystem);
    }

    Path turtle() const {
        return _turtle
                .rotate(angle)
                .translate(turtlePosition);
    }

    Reference turtleSystem;

    Angle angle;
    Point turtlePosition;

    vector<Path> paths;
    double xScrunch { 1 };
    double yScrunch { 1 };
    bool showTurtle { true };
    Mode mode { Mode::WRAP };
    Path _turtle;

private:
    void wrapLine(const Point &current) {
        auto GPSPoint = current.toGPS();
        auto GPSTurtle = turtlePosition.toGPS();
        int offsetX {0}, offsetY {0};

        if(GPSPoint.x>GC::SCREEN_WIDTH) offsetX -= GC::SCREEN_WIDTH;
        else if(GPSPoint.x<0) offsetX += GC::SCREEN_WIDTH;

        if(GPSPoint.y>GC::SCREEN_HEIGHT) offsetY -= GC::SCREEN_HEIGHT;
        else if(GPSPoint.y<0) offsetY += GC::SCREEN_HEIGHT;

        GPSTurtle.x += offsetX;
        GPSTurtle.y += offsetY;
        GPSPoint.x += offsetX;
        GPSPoint.y += offsetY;

        cout << "Turtle " << GPSTurtle << " - Dest: " << GPSPoint << endl;
        turtlePosition = turtleSystem.fromGPS(GPSPoint);
        paths.emplace_back(turtleSystem.fromGPS(GPSTurtle));
        paths.back().push_back(turtlePosition);
    }
};

Path createTurle(const Reference &turtleSystem) {
    Path _turtle { turtleSystem, 0, TURTLE_HEIGHT/2 };

    _turtle.push_back( TURTLE_BASE/2, -TURTLE_HEIGHT/2);
    _turtle.push_back(-TURTLE_BASE/2, -TURTLE_HEIGHT/2);
    _turtle.push_back(0,  TURTLE_HEIGHT/2);

    return _turtle;
}

} /* ns: turtle */

} /* ns: mlogo */


#endif /* __TURTLE_IMPL_HPP__ */
