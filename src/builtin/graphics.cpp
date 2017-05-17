/**
 * @file: builtin.cpp
 *
 *  created on:  4 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#include "common.hpp"

namespace mlogo { namespace builtin {

namespace {

/**
 * Turtle Graphics
 */
struct Forward : BuiltinProcedure {
	Forward() : BuiltinProcedure(1) {}
	void operator()() const override {
		int arg = fetchArg(0).asUnsigned();
		Turtle::instance().forward(arg);
	}
};

struct Right : BuiltinProcedure {
	Right() : BuiltinProcedure(1) {}
	void operator()() const override {
		double arg = fetchArg(0).asDouble();
		Turtle::instance().right(arg);
	}
};

struct Backward : BuiltinProcedure {
	Backward() : BuiltinProcedure(1) {}
	void operator()() const override {
		int arg = fetchArg(0).asUnsigned();
		Turtle::instance().forward(-1 * arg);
	}
};

struct Left : BuiltinProcedure {
	Left() : BuiltinProcedure(1) {}
	void operator()() const override {
		double arg = fetchArg(0).asDouble();
		Turtle::instance().right(-1 * arg);
	}
};

struct Home : BuiltinProcedure {
	Home() : BuiltinProcedure(0) {}
	void operator()() const override {
		Turtle::instance().home();
	}
};

struct Clean : BuiltinProcedure {
    Clean() : BuiltinProcedure(0) {}
    void operator()() const override {
        auto p = Turtle::instance().currentPosition();
        Turtle::instance().clear();
        Turtle::instance().currentPosition(p);
    }
};

struct ClearScreen : BuiltinProcedure {
	ClearScreen() : BuiltinProcedure(0) {}
	void operator()() const override {
        Turtle::instance().home();
		Turtle::instance().clear();
	}
};

struct SetPos : BuiltinProcedure {
    SetPos() : BuiltinProcedure(1) {}
    void operator()() const override {
        auto pos = fetchArg(0).list();
        if(pos.size()!=2) throw std::logic_error("Expected X,Y Coordinates");
        int x = ValueBox(pos[0]).asInteger();
        int y = ValueBox(pos[1]).asInteger();
        Turtle::instance().currentPosition(
            std::make_pair(x, y));
    }
};

struct SetXY : BuiltinProcedure {
    SetXY() : BuiltinProcedure(2) {}
    void operator()() const override {
        int x = fetchArg(0).asInteger();
        int y = fetchArg(1).asInteger();
        Turtle::instance().currentPosition(std::make_pair(x, y));
    }
};

struct SetX : BuiltinProcedure {
    SetX() : BuiltinProcedure(1) {}
    void operator()() const override {
        int x = fetchArg(0).asInteger();
        Turtle::instance().currentXPosition(x);
    }
};

struct SetY : BuiltinProcedure {
    SetY() : BuiltinProcedure(1) {}
    void operator()() const override {
        int y = fetchArg(0).asInteger();
        Turtle::instance().currentYPosition(y);
    }
};

struct SetHeading : BuiltinProcedure {
    SetHeading() : BuiltinProcedure(1) {}
    void operator()() const override {
        int alpha = fetchArg(0).asDouble();
        Turtle::instance().heading(-1 * alpha);
    }
};

struct Position : BuiltinProcedure {
    Position() : BuiltinProcedure(0, true) {}
    void operator()() const override {
        ListValue out;
        auto pos = Turtle::instance().currentPosition();
        stringstream ss, ss1;
        ss << pos.first;
        out.push_back(ss.str());

        ss1 << pos.second;
        out.push_back(ss1.str());

        setReturnValue(out);
    }
};

struct GetX : BuiltinProcedure {
    GetX() : BuiltinProcedure(0, true) {}
    void operator()() const override {
        auto pos = Turtle::instance().currentPosition();
        stringstream ss;
        ss << pos.first;
        setReturnValue(ss.str());
    }
};

struct GetY : BuiltinProcedure {
    GetY() : BuiltinProcedure(0, true) {}
    void operator()() const override {
        auto pos = Turtle::instance().currentPosition();
        stringstream ss;
        ss << pos.second;
        setReturnValue(ss.str());
    }
};

struct Heading : BuiltinProcedure {
    Heading() : BuiltinProcedure(0, true) {}
    void operator()() const override {
        double h { -1 * Turtle::instance().heading() };
        stringstream ss;

        while(h<0) h += 360;
        while(h>359) h -= 360;
        ss << h;
        setReturnValue(ss.str());
    }
};

struct Scrunch : BuiltinProcedure {
    Scrunch() : BuiltinProcedure(0, true) {}
    void operator()() const override {
        auto scrunch = Turtle::instance().scrunch();
        ListValue out;
        stringstream ss, ss1;
        ss << scrunch.first;
        out.push_back(ss.str());

        ss1 << scrunch.second;
        out.push_back(ss1.str());

        setReturnValue(out);
    }
};

struct SetScrunch : BuiltinProcedure {
    SetScrunch() : BuiltinProcedure(2) {}
    void operator()() const override {
        int alpha = fetchArg(0).asDouble();
        int beta = fetchArg(1).asDouble();

        Turtle::instance().scrunch(alpha, beta);
    }
};

struct ShowTurtle : BuiltinProcedure {
    ShowTurtle() : BuiltinProcedure(0) {}
    void operator()() const override {
        Turtle::instance().showTurtle();
    }
};

struct HideTurtle : BuiltinProcedure {
    HideTurtle() : BuiltinProcedure(0) {}
    void operator()() const override {
        Turtle::instance().hideTurtle();
    }
};

struct WindowMode : BuiltinProcedure {
    WindowMode() : BuiltinProcedure(0) {}
    void operator()() const override {
        Turtle::instance().mode(turtle::Mode::WINDOW);
    }
};

struct FenceMode : BuiltinProcedure {
    FenceMode() : BuiltinProcedure(0) {}
    void operator()() const override {
        Turtle::instance().mode(turtle::Mode::FENCE);
    }
};

struct WrapMode : BuiltinProcedure {
    WrapMode() : BuiltinProcedure(0) {}
    void operator()() const override {
        Turtle::instance().mode(turtle::Mode::WRAP);
    }
};

struct TurtleMode : BuiltinProcedure {
    TurtleMode() : BuiltinProcedure(0, true) {}
    void operator()() const override {
        stringstream ss;
        ss << Turtle::instance().mode();

        setReturnValue(ss.str());
    }
};

struct Shownp : BuiltinProcedure {
    Shownp() : BuiltinProcedure(0, true) {}
    void operator()() const override {
        stringstream ss;
        ss << (Turtle::instance().visible()?"TRUE":"FALSE");

        setReturnValue(ss.str());
    }
};

struct PenUp : BuiltinProcedure {
    PenUp() : BuiltinProcedure(0) {}
    void operator()() const override {
        Turtle::instance().penUp();
    }
};

struct PenDown : BuiltinProcedure {
    PenDown() : BuiltinProcedure(0) {}
    void operator()() const override {
        Turtle::instance().penDown();
    }
};

struct Towards : BuiltinProcedure {
    Towards() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        stringstream ss;
        auto pos = fetchArg(0).list();
        if(pos.size()!=2) throw std::logic_error("Expected X,Y Coordinates");
        int x = ValueBox(pos[0]).asInteger();
        int y = ValueBox(pos[1]).asInteger();
        double heading = Turtle::instance().towards(x, y);
        ss << heading;

        setReturnValue(ss.str());
    }
};

}

/**
 * Register procedures in memory
 */

void initGraphicsBuiltInProcedures() {
	Stack::instance()
	    .setProcedure<Forward>("forward")
        .setProcedure<Forward>("fd")
        .setProcedure<Backward>("back")
        .setProcedure<Backward>("bk")
        .setProcedure<Right>("right")
        .setProcedure<Right>("rt")
        .setProcedure<Left>("left")
        .setProcedure<Left>("lt")
        .setProcedure<Home>("home")
        .setProcedure<Clean>("clean")
        .setProcedure<ClearScreen>("clearscreen")
        .setProcedure<ClearScreen>("cs")
        .setProcedure<SetPos>("setpos")
        .setProcedure<SetXY>("setxy")
        .setProcedure<SetX>("setx")
        .setProcedure<SetY>("sety")
        .setProcedure<SetHeading>("setheading")
        .setProcedure<SetHeading>("seth")
        .setProcedure<Position>("pos")
        .setProcedure<GetX>("xcor")
        .setProcedure<GetY>("ycor")
        .setProcedure<Heading>("heading")
        .setProcedure<Scrunch>("scrunch")
        .setProcedure<SetScrunch>("setscrunch")
        .setProcedure<ShowTurtle>("showturtle")
        .setProcedure<ShowTurtle>("st")
        .setProcedure<HideTurtle>("hideturtle")
        .setProcedure<HideTurtle>("ht")
        .setProcedure<WindowMode>("window")
        .setProcedure<FenceMode>("fence")
        .setProcedure<WrapMode>("wrap")
        .setProcedure<TurtleMode>("turtlemode")
        .setProcedure<Shownp>("shownp")
        .setProcedure<Shownp>("shown?")
        .setProcedure<PenUp>("penup")
        .setProcedure<PenUp>("pu")
        .setProcedure<PenDown>("pendown")
        .setProcedure<PenDown>("pd")
        .setProcedure<Towards>("towards");
}

}}
