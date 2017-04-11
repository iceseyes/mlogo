/**
 * @file: types.cpp
 *
 *  created on:  1 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include "types.hpp"

#include <sstream>
#include <algorithm>

#include "memory.hpp"

namespace mlogo {

namespace types {

struct MultiValuePrinter : boost::static_visitor<std::string> {
    template <typename T>
    std::string operator()(const std::vector<T>& vec) const {
        std::ostringstream ost;

        ost << "( ";

        typename std::vector<T>::const_iterator it = vec.begin();
        for (; it != vec.end(); ++it)
            ost << boost::apply_visitor( MultiValuePrinter(), *it ) << " ";

        ost << ") ";

        return ost.str();
    }

    template <typename T>
    std::string operator()(const T& operand) const {
        std::ostringstream ost;
        ost << operand;
        return ost.str();
    }
};

struct ValueEqual : boost::static_visitor<bool> {
    template<typename T>
    bool operator()(const T &s1, const T &s2) {
        return s1==s2;
    }

    template<typename T, typename Q>
    bool operator()(T &&t, Q &&q) { return false; }
};

struct ValueSum : boost::static_visitor<Value> {
    Value operator()(const std::string &s1, const std::string &s2) const {
        return s1 + s2;
    }

    Value operator()(ListValue &v1, const ListValue &v2) const {
        std::copy(v2.begin(), v2.end(), std::back_inserter(v1));

        return v1;
    }

    Value operator()(ListValue &v1, const std::string &s2) const {
        v1.push_back(s2);

        return v1;
    }

    Value operator()(const std::string &s2, ListValue &v1) const {
        v1.push_back(s2);

        return v1;
    }
};

std::string toString(const Value &v) {
    return boost::apply_visitor(MultiValuePrinter(), v);
}


ActualArguments &ActualArguments::push_back(const Value &value) {
    arguments.push_back(value);
    return *this;
}

ActualArguments &ActualArguments::push_back(Value &&value) {
    arguments.push_back(value);
    return *this;
}

const Value &ActualArguments::at(uint8_t index) const {
    return arguments.at(index);
}

Value &ActualArguments::at(uint8_t index) {
    return arguments.at(index);
}

BasicProcedure::BasicProcedure(uint8_t args, bool funct) :
        _nArgs { args }, _funct { funct } {}

Value &BasicProcedure::fetchArg(uint8_t index) const {
    return memory::Stack::instance().getArgument(index);
}

void BasicProcedure::setReturnValue(const Value &output) const {
    if(!_funct) {
        throw std::logic_error("Pure procedure can not return a value");
    }

    memory::Stack::instance().storeResult(output);
}

} /* ns: types */

} /* ns: mlogo */
