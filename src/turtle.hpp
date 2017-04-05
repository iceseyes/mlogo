/**
 * @file: turtle.hpp
 *
 *  created on:  5 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#ifndef __TURTLE_HPP__
#define __TURTLE_HPP__

namespace mlogo {

namespace turtle {

class Turtle {
public:
    static Turtle &instance() {
        static Turtle _instance;
        return _instance;
    }

    Turtle &home();
    Turtle &clear();
    Turtle &forward(int steps);
    Turtle &right(double angle);

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

} /* ns: turtle */

} /* ns: mlogo */



#endif /* __TURTLE_HPP__ */
