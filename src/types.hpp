/**
 * @file: types.hpp
 *
 *  Created on: 18 mar 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <cinttypes>
#include <string>
#include <vector>

#include <boost/variant.hpp>

#include "parser.hpp"

namespace mlogo {

namespace eval {

class AST;

} /* ns: eval */

namespace types {

using WordValue = std::string;

using Value =
    boost::make_recursive_variant<WordValue,
                                  std::vector<boost::recursive_variant_>>::type;

using ListValue = std::vector<Value>;

std::string toString(const Value &v);

class ValueBox {
public:
    ValueBox();
    ValueBox(const char v[]);
    ValueBox(const std::string &v);
    ValueBox(bool v);
    ValueBox(const ListValue &v);
    ValueBox(const Value &v);

    ValueBox &operator=(const ValueBox &v);

    bool isWord() const;
    bool isList() const { return !isWord(); }

    bool empty() const;

    std::string toString(bool wBrackets = false) const;
    bool toBool() const;

    double asDouble() const;
    int32_t asInteger() const;
    uint32_t asUnsigned() const;

    WordValue &word();
    const WordValue &word() const;

    ListValue &list();
    const ListValue &list() const;

    Value &value() { return _value; }
    const Value &value() const { return _value; }

    ValueBox &push_front(const ValueBox &v);
    ValueBox &push_back(const ValueBox &v);
    ValueBox &push_all_back(const ValueBox &v);
    ValueBox &set(uint32_t index, const ValueBox &v);

    ValueBox front() const;
    ValueBox back() const;

    ValueBox butFirst() const;
    ValueBox butLast() const;
    ValueBox at(std::size_t index) const;

    bool in(const ValueBox &v) const;
    Value member(const ValueBox &v) const;

    std::size_t size() const;

private:
    Value _value;

    friend bool operator==(const ValueBox &v1, const ValueBox &v2);
    friend bool operator<(const ValueBox &v1, const ValueBox &v2);
};

class ActualArguments {
public:
    ActualArguments() {}

    ActualArguments &push_back(const ValueBox &value);
    ActualArguments &push_back(ValueBox &&value);

    const ValueBox &at(uint8_t index) const;
    ValueBox &at(uint8_t index);

private:
    std::vector<ValueBox> arguments;
};

class BasicProcedure {
public:
    BasicProcedure(uint8_t args, bool funct = false);
    virtual ~BasicProcedure() {}
    virtual void operator()() const = 0;

    uint8_t nArgs() const { return _nArgs; }
    bool isFunction() const { return _funct; }

protected:
    ValueBox &fetchArg(uint8_t index) const;
    void setReturnValue(const ValueBox &output) const;
    void setReturnValue(bool output) const;
    void setReturnValue(std::size_t output) const;
    void setReturnValue(int output) const;

private:
    uint8_t _nArgs;
    bool _funct;
};

class UserDefinedProcedure : public BasicProcedure {
    using AST = eval::AST;
    using Definition = parser::Procedure;
    using Parameters = std::vector<std::string>;

public:
    UserDefinedProcedure(const Definition &definition);
    virtual ~UserDefinedProcedure();

    void operator()() const override;

    const Parameters &params() const;
    const std::string paramName(std::size_t index) const;

    const AST ast() const;

private:
    void loadParameters();
    void loadArguments() const;

    Parameters _params;
    Definition definition;
};

bool operator==(const ValueBox &v1, const ValueBox &v2);
bool operator!=(const ValueBox &v1, const ValueBox &v2);
bool operator<(const ValueBox &v1, const ValueBox &v2);
bool operator>(const ValueBox &v1, const ValueBox &v2);
bool operator<=(const ValueBox &v1, const ValueBox &v2);
bool operator>=(const ValueBox &v1, const ValueBox &v2);
bool in(const ValueBox &v1, const ValueBox &v2);

std::string toString(const ListValue &v, bool withBrackets = false);

} /* ns types */

} /* ns mlogo */

namespace std {

template <typename S>
S &operator<<(S &s, const mlogo::types::ListValue &v) {
    bool first{true};

    s << "[";

    for (auto &i : v) {
        if (!first) s << " ";
        s << i;

        first = false;
    }

    s << "]";

    return s;
}

} /* ns: std */

#endif /* TYPES_HPP_ */
