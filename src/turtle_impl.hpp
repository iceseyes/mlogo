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
#include <iostream>
#include <vector>

#include "geometry.hpp"
#include "graphics.hpp"

using namespace std;
using namespace mlogo::geometry;

namespace mlogo {

namespace turtle {

using GC = graphics::Context;

static constexpr int TURTLE_HEIGHT{10};
static constexpr int TURTLE_BASE{10};
static constexpr int TURTLE_CENTER_X{GC::SCREEN_WIDTH / 2};
static constexpr int TURTLE_CENTER_Y{GC::SCREEN_HEIGHT / 2};

Path createTurle(const Reference &turtleSystem);

struct Turtle::_pImpl {
    _pImpl()
        : turtleSystem(1, TURTLE_CENTER_X, -1, TURTLE_CENTER_Y),
          angle(Angle::Degrees(0)),
          turtlePosition(0, 0, turtleSystem),
          _turtle(createTurle(turtleSystem)),
          topLeft(turtleSystem.fromGPS(Point(0, 0))),
          bottomRight(
              turtleSystem.fromGPS(Point(GC::SCREEN_WIDTH, GC::SCREEN_HEIGHT))),
          offsets(GC::SCREEN_WIDTH, GC::SCREEN_HEIGHT, turtleSystem) {
        initPaths();
    }

    ~_pImpl() { clearPaths(); }

    void initPaths() {
        clearPaths();
        newPath();
    }

    void clearPaths() { paths.clear(); }

    void newPath() { newPath(make_pair(0, 0)); }

    void newPath(const Turtle::Position &o) {
        paths.push_back(Path(toPoint(o)));
    }

    Turtle::Position lastPos() const { return toPosition(turtlePosition); }

    void moveTo(const Turtle::Position &o) {
        auto current = toPoint(o);

        addPoint(current);
    }

    void walk(int steps) {
        auto current = turtlePosition;

        Point d{0, steps, turtleSystem};
        d = d.rotate(angle).scale(xScrunch, yScrunch);
        current = current + d;

        addPoint(current);
    }

    void addPoint(const Point &current) {
        switch (mode) {
        case Mode::WINDOW:
            if (paths.empty()) paths.emplace_back(turtlePosition);

            addTocurrentPath(current);
            turtlePosition = current;
            break;
        case Mode::FENCE:
            fenceLine(current);
            break;
        case Mode::WRAP:
            wrapLine(current);
            break;
        }
    }

    void setAngle(double a) { setAngle(Angle::Degrees(a)); }

    void setAngle(Angle a) { angle = a; }

    Turtle::Position toPosition(const Point &p) const {
        return make_pair(p.x, p.y);
    }

    Turtle::Position toPosition(Point &&p) const { return make_pair(p.x, p.y); }

    Point toPoint(const Turtle::Position &p) const {
        return Point(p.first, p.second, turtleSystem);
    }

    Path turtle() const {
        return _turtle.rotate(angle).translate(turtlePosition);
    }

    Reference turtleSystem;

    Angle angle;
    Point turtlePosition;

    vector<Path> paths;
    double xScrunch{1};
    double yScrunch{1};
    bool showTurtle{true};
    Mode mode{Mode::WRAP};
    Path _turtle;
    Point topLeft, bottomRight, offsets;
    Pen pen;

private:
    void addTocurrentPath(const Point &point) {
        switch (pen.state) {
        case Pen::State::DOWN:
            paths.back().push_back(point);
            break;
        case Pen::State::UP:
            paths.emplace_back(point);
            break;
        }
    }

    void wrapLine(const Point &current) {
        Point next = current;
        StraightLine line(turtlePosition, current);
        Point middle = outOfBounds(line, current);

        if (middle.x || middle.y) {
            next = middle;
        }

        if (paths.empty()) paths.emplace_back(turtlePosition);
        addTocurrentPath(next);
        turtlePosition = next;

        if (next != current) {
            Point next_cur{current};
            if (turtlePosition.x == topLeft.x) {
                turtlePosition.x = bottomRight.x;
                next_cur.x += offsets.x;
            } else if (turtlePosition.x == bottomRight.x) {
                turtlePosition.x = topLeft.x;
                next_cur.x -= offsets.x;
            }

            if (turtlePosition.y == topLeft.y) {
                turtlePosition.y = bottomRight.y;
                next_cur.y -= offsets.y;
            } else if (turtlePosition.y == bottomRight.y) {
                turtlePosition.y = topLeft.y;
                next_cur.y += offsets.y;
            }

            paths.emplace_back(turtlePosition);
            wrapLine(next_cur);
        }
    }

    void fenceLine(const Point &current) {
        Point next = current;
        StraightLine line(turtlePosition, current);
        Point middle = outOfBounds(line, current);

        if (middle.x || middle.y) {
            next = middle;
        }

        if (paths.empty()) paths.emplace_back(turtlePosition);
        addTocurrentPath(next);
        turtlePosition = next;
    }

    Point outOfBounds(const StraightLine &line, const Point &current) {
        Point middle(0, 0, current.system);

        if (current.x < topLeft.x) {
            auto crossline = line.whenX(topLeft.x);
            if ((crossline.y <= topLeft.y) && (crossline.y >= bottomRight.y)) {
                middle = crossline;
            }
        } else if (current.x > bottomRight.x) {
            auto crossline = line.whenX(bottomRight.x);
            if ((crossline.y <= topLeft.y) && (crossline.y >= bottomRight.y)) {
                middle = crossline;
            }
        }

        if (current.y > topLeft.y) {
            auto crossline = line.whenY(topLeft.y);
            if ((crossline.x >= topLeft.x) && (crossline.x <= bottomRight.x)) {
                middle = crossline;
            }
        } else if (current.y < bottomRight.y) {
            auto crossline = line.whenY(bottomRight.y);
            if ((crossline.x >= topLeft.x) && (crossline.x <= bottomRight.x)) {
                middle = crossline;
            }
        }

        return middle;
    }
};

Path createTurle(const Reference &turtleSystem) {
    Path _turtle{turtleSystem, 0, TURTLE_HEIGHT / 2};

    _turtle.push_back(TURTLE_BASE / 2, -TURTLE_HEIGHT / 2);
    _turtle.push_back(-TURTLE_BASE / 2, -TURTLE_HEIGHT / 2);
    _turtle.push_back(0, TURTLE_HEIGHT / 2);

    return _turtle;
}

} /* ns: turtle */

} /* ns: mlogo */

#endif /* __TURTLE_IMPL_HPP__ */
