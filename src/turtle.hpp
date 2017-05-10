/**
 * @file: turtle.hpp
 *
 *  created on:  5 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#ifndef __TURTLE_HPP__
#define __TURTLE_HPP__

#include <tuple>
#include <iostream>

namespace mlogo {

namespace turtle {

enum class Mode {
    WRAP, WINDOW, FENCE
};

struct Pen {
    enum class State {
        DOWN, UP
    };

    enum class Mode {
        PAINT, ERASE, REVERSE
    };

    State state { State::DOWN };
    Mode mode { Mode::PAINT };
};

class Turtle {
public:
    using Position = std::pair<int, int>;

    static Turtle &instance() {
        static Turtle _instance;
        return _instance;
    }

    Turtle &home();
    Turtle &clear();
    Turtle &forward(int steps);
    Turtle &right(double angle);

    Position currentPosition() const;
    Turtle &currentPosition(const Position &pos);
    Turtle &currentXPosition(int x);
    Turtle &currentYPosition(int y);

    double heading() const;
    Turtle &heading(double h);

    double towards(int x, int y) const;

    std::pair<double, double> scrunch();
    Turtle &scrunch(double xScrunch, double yScrunch);

    Turtle &showTurtle();
    Turtle &hideTurtle();
    bool &visible() const;

    Turtle &penUp();
    Turtle &penDown();

    Turtle &mode(Mode m);
    Mode mode() const;

    Pen &pen();
    const Pen &pen() const;

private:
    Turtle();
    ~Turtle();

    Turtle(const Turtle &) = delete;
    Turtle(Turtle &&) = delete;

    Turtle &operator=(const Turtle &) = delete;
    Turtle &operator=(Turtle &&) = delete;

    void render();

    struct _pImpl;
    _pImpl *impl;
};


std::ostream &operator<<(std::ostream &s, const Mode &value);

} /* ns: turtle */

} /* ns: mlogo */



#endif /* __TURTLE_HPP__ */
