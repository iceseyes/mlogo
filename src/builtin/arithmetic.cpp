/**
 * @file: arithmetic.cpp
 *
 *  Created on: 17 mag 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include "common.hpp"

#include "../geometry.hpp"

using namespace mlogo::geometry;

/**
RANDOM num
RERANDOM
FORM num width precision
BITAND num1 num2
BITOR num1 num2
BITXOR num1 num2
BITNOT num
ASHIFT num1 num2
LSHIFT num1 num2
*/
namespace mlogo {

namespace builtin {

namespace {

struct ArithmeticOperation : BuiltinProcedure {
    ArithmeticOperation() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        stringstream ss;
        double arg0 = fetchArg(0).asDouble();
        double arg1 = fetchArg(1).asDouble();

        double result = _result(arg0, arg1);
        long rlong = static_cast<long>(result);

        if (result - rlong < 1e-5)
            ss << rlong;
        else
            ss << result;

        setReturnValue(ss.str());
    }

    virtual double _result(double arg0, double arg1) const = 0;
};

struct ArithmeticUnary : BuiltinProcedure {
    ArithmeticUnary() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        stringstream ss;
        double arg0 = fetchArg(0).asDouble();

        double result = _result(arg0);
        long rlong = static_cast<long>(result);

        if (result - rlong < 1e-5)
            ss << rlong;
        else
            ss << result;

        setReturnValue(ss.str());
    }

    virtual double _result(double arg0) const = 0;
};

struct Sum : ArithmeticOperation {
    Sum() : ArithmeticOperation() {}
    double _result(double arg0, double arg1) const override {
        return arg0 + arg1;
    }
};

struct Difference : ArithmeticOperation {
    Difference() : ArithmeticOperation() {}
    double _result(double arg0, double arg1) const override {
        return arg0 - arg1;
    }
};

struct Product : ArithmeticOperation {
    Product() : ArithmeticOperation() {}
    double _result(double arg0, double arg1) const override {
        return arg0 * arg1;
    }
};

struct Quotient : ArithmeticOperation {
    Quotient() : ArithmeticOperation() {}
    double _result(double arg0, double arg1) const override {
        return arg0 / arg1;
    }
};

struct Remainder : ArithmeticOperation {
    Remainder() : ArithmeticOperation() {}
    double _result(double arg0, double arg1) const override {
        return int(arg0) % int(arg1);
    }
};

struct Module : ArithmeticOperation {
    Module() : ArithmeticOperation() {}
    double _result(double arg0, double arg1) const override {
        return int(arg0) % int(arg1);
    }
};

struct Power : ArithmeticOperation {
    Power() : ArithmeticOperation() {}
    double _result(double arg0, double arg1) const override {
        return pow(arg0, arg1);
    }
};

struct Int : ArithmeticUnary {
    Int() : ArithmeticUnary() {}
    double _result(double arg0) const override { return trunc(arg0); }
};

struct Minus : ArithmeticUnary {
    Minus() : ArithmeticUnary() {}
    double _result(double arg0) const override { return -1 * arg0; }
};

struct Round : ArithmeticUnary {
    Round() : ArithmeticUnary() {}
    double _result(double arg0) const override { return round(arg0); }
};

struct Sqrt : ArithmeticUnary {
    Sqrt() : ArithmeticUnary() {}
    double _result(double arg0) const override { return sqrt(arg0); }
};

struct Exp : ArithmeticUnary {
    Exp() : ArithmeticUnary() {}
    double _result(double arg0) const override { return exp(arg0); }
};

struct Log10 : ArithmeticUnary {
    Log10() : ArithmeticUnary() {}
    double _result(double arg0) const override { return log10(arg0); }
};

struct Ln : ArithmeticUnary {
    Ln() : ArithmeticUnary() {}
    double _result(double arg0) const override { return log(arg0); }
};

struct Sin : ArithmeticUnary {
    Sin() : ArithmeticUnary() {}
    double _result(double arg0) const override {
        return sin(Angle::Degrees(arg0));
    }
};

struct RadSin : ArithmeticUnary {
    RadSin() : ArithmeticUnary() {}
    double _result(double arg0) const override { return sin(Angle::Rad(arg0)); }
};

struct Cos : ArithmeticUnary {
    Cos() : ArithmeticUnary() {}
    double _result(double arg0) const override {
        return cos(Angle::Degrees(arg0));
    }
};

struct RadCos : ArithmeticUnary {
    RadCos() : ArithmeticUnary() {}
    double _result(double arg0) const override { return cos(Angle::Rad(arg0)); }
};

struct ArcTan : ArithmeticUnary {
    ArcTan() : ArithmeticUnary() {}
    double _result(double arg0) const override {
        return arctan(arg0).degrees().value();
    }
};

struct RadArcTan : ArithmeticUnary {
    RadArcTan() : ArithmeticUnary() {}
    double _result(double arg0) const override {
        return arctan(arg0).radians().value();
    }
};

struct Less : BuiltinProcedure {
    Less() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        setReturnValue(fetchArg(0) < fetchArg(1));
    }
};

struct Greater : BuiltinProcedure {
    Greater() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        setReturnValue(fetchArg(0) > fetchArg(1));
    }
};

struct LessEq : BuiltinProcedure {
    LessEq() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        setReturnValue(fetchArg(0) <= fetchArg(1));
    }
};

struct GreaterEq : BuiltinProcedure {
    GreaterEq() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        setReturnValue(fetchArg(0) >= fetchArg(1));
    }
};

struct And : BuiltinProcedure {
    And() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        setReturnValue(fetchArg(0).toBool() && fetchArg(1).toBool());
    }
};

struct Or : BuiltinProcedure {
    Or() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        setReturnValue(fetchArg(0).toBool() || fetchArg(1).toBool());
    }
};

struct Not : BuiltinProcedure {
    Not() : BuiltinProcedure(1, true) {}
    void operator()() const override { setReturnValue(!fetchArg(0).toBool()); }
};

struct RShift : BuiltinProcedure {
    RShift() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        setReturnValue(fetchArg(0).asUnsigned() >> fetchArg(1).asUnsigned());
    }
};

struct LShift : BuiltinProcedure {
    LShift() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        setReturnValue(fetchArg(0).asUnsigned() << fetchArg(1).asUnsigned());
    }
};

} /* ns */

/**
 * Register procedures in memory
 */

void initArithmeticBuiltInProcedures() {
    Stack::instance()
        .setProcedure<Sum>("sum")
        .setProcedure<Difference>("difference")
        .setProcedure<Minus>("minus")
        .setProcedure<Product>("product")
        .setProcedure<Quotient>("quotient")
        .setProcedure<Remainder>("remainder")
        .setProcedure<Module>("module")
        .setProcedure<Int>("int")
        .setProcedure<Round>("round")
        .setProcedure<Sqrt>("sqrt")
        .setProcedure<Power>("power")
        .setProcedure<Exp>("exp")
        .setProcedure<Log10>("log10")
        .setProcedure<Ln>("ln")
        .setProcedure<Sin>("sin")
        .setProcedure<RadSin>("radsin")
        .setProcedure<Cos>("cos")
        .setProcedure<RadCos>("radcos")
        .setProcedure<ArcTan>("arctan")
        .setProcedure<RadArcTan>("radarctan")
        .setProcedure<Less>("lessp")
        .setProcedure<Greater>("greaterp")
        .setProcedure<LessEq>("lessequalp")
        .setProcedure<GreaterEq>("greaterequalp")
        .setProcedure<And>("and")
        .setProcedure<Or>("or")
        .setProcedure<Not>("not")
        .setProcedure<RShift>("ashift")
        .setProcedure<LShift>("lshift");
}

} /* ns: builtin */

} /* ns: mlogo */
