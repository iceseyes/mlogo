/**
 * @file: turle.cpp
 *
 *  created on:  5 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#include "turtle_impl.hpp"

#include "graphics.hpp"

using namespace std;

namespace mlogo {

namespace turtle {

Turtle::Turtle() :
    impl (new _pImpl) {
    render();
}

Turtle::~Turtle() {
    delete impl;
}

Turtle &Turtle::home() {
    impl->angle = Angle::Degrees(_pImpl::START_ANGLE);
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
    int sx = sin(impl->angle) * steps;
    int sy = cos(impl->angle) * steps;

    impl->walkBy(sx, sy);
    render();

    return *this;
}

Turtle &Turtle::right(double angle) {
    impl->setAngle(impl->angle - Angle::Degrees(angle));
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
    return impl->angle.degrees().value();
}

Turtle &Turtle::heading(double h) {
    impl->setAngle(180 - h);
    render();
    return *this;
}

std::pair<double, double> Turtle::scrunch() {
    return std::make_pair(impl->xScrunch, impl->yScrunch);
}

Turtle &Turtle::scrunch(double xScrunch, double yScrunch) {
    impl->xScrunch = xScrunch;
    impl->yScrunch = yScrunch;
    return *this;
}

Turtle &Turtle::showTurtle() {
    impl->showTurtle = true;
    render();
    return *this;
}

Turtle &Turtle::hideTurtle() {
    impl->showTurtle = false;
    render();
    return *this;
};

void Turtle::render() {
    GC::instance().window()->clear();
    if(impl->showTurtle) GC::instance().window()->draw(impl->turtle);
    for(auto p : impl->paths) GC::instance().window()->draw(p);
    GC::instance().window()->paint();
}

} /* ns: turtle */

} /* ns: mlogo */
