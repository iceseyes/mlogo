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
    heading(0);
    currentPosition(make_pair(0,0));
    render();

    return *this;
}

Turtle &Turtle::clear() {
    impl->clearPaths();
    render();
    return *this;
}

Turtle &Turtle::forward(int steps) {
    impl->walk(steps);
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
    impl->setAngle(h);
    render();
    return *this;
}

pair<double, double> Turtle::scrunch() {
    return make_pair(impl->xScrunch, impl->yScrunch);
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
}

bool &Turtle::visible() const {
    return impl->showTurtle;
}

Turtle &Turtle::penUp() {
    impl->pen.state = Pen::State::UP;
    return *this;
}

Turtle &Turtle::penDown() {
    impl->pen.state = Pen::State::DOWN;
    return *this;
}

Turtle &Turtle::mode(Mode m) {
    impl->mode = m;
    return *this;
}

Mode Turtle::mode() const {
    return impl->mode;
}

Pen &Turtle::pen() {
    return impl->pen;
}

const Pen &Turtle::pen() const {
    return impl->pen;
}

void Turtle::render() {
    GC::instance().window()->clear();
    if(impl->showTurtle) {
        GC::instance().window()->draw(impl->turtle());
    }
    for(auto p : impl->paths) GC::instance().window()->draw(p);
    GC::instance().window()->paint();
}

std::ostream &operator<<(std::ostream &s, const Mode &value) {
    switch(value) {
    case Mode::WINDOW: s << "WINDOW"; break;
    case Mode::FENCE: s << "FENCE"; break;
    case Mode::WRAP: s << "WRAP"; break;
    }

    return s;
}

} /* ns: turtle */

} /* ns: mlogo */
