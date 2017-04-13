/**
 * @file: value.cpp
 */

#include "value.hpp"

#include <sstream>
#include <algorithm>

using bad_get = boost::bad_get;

template<typename T>
using Visitor = boost::static_visitor<T>;

using boost::apply_visitor;

namespace mlogo {

namespace types {

namespace {

struct IsEmptyVisitor : Visitor<bool> {
    template<typename T>
    bool operator()(const T &v) const {
        return v.empty();
    }
};

struct PushAllBack : Visitor<ListValue> {
    PushAllBack(ListValue &container) :
        _container (container) {}

    ListValue &operator()(const WordValue &w) {
        _container.push_back(w);
        return _container;
    }

    ListValue &operator()(const ListValue &l) {
        for(auto &item : l)
            _container.push_back(item);
        return _container;
    }

    ListValue _container;
};

struct FrontVisitor : Visitor<Value> {
    Value operator()(const ListValue &v) const {
        return v.front();
    }

    Value operator()(const WordValue &v) const {
        return WordValue() + v.front();
    }
};

struct BackVisitor : Visitor<Value> {
    Value operator()(const ListValue &v) const {
        return v.back();
    }

    Value operator()(const WordValue &v) const {
        return WordValue() + v.back();
    }
};

struct AtVisitor : Visitor<Value> {
    AtVisitor(std::size_t index) :
        index(index) {}

    Value operator()(const ListValue &v) const {
        return v.at(index);
    }

    Value operator()(const WordValue &v) const {
        return WordValue() + v.at(index);
    }

    std::size_t index;
};

struct ButFirstVisitor : Visitor<Value> {
    template<typename T>
    Value operator()(const T &v) const {
        T out;
        auto start = std::begin(v);
        std::advance(start, 1);

        std::copy(start, std::end(v), std::back_inserter(out));

        return out;
    }
};

struct ButLastVisitor : Visitor<Value> {
    template<typename T>
    Value operator()(const T &v) const {
        T out;
        auto end = std::end(v);
        std::advance(end, -1);

        std::copy(std::begin(v), end, std::back_inserter(out));

        return out;
    }
};

}

ValueBox::ValueBox() {}

ValueBox::ValueBox(const char v[]) :
    _value(std::string(v)) {}

ValueBox::ValueBox(const std::string &v) :
        _value(v) {}

ValueBox::ValueBox(const ListValue &v) :
        _value(v) {}

ValueBox::ValueBox(const Value &v) :
        _value(v) {}

ValueBox &ValueBox::operator=(const ValueBox &v) {
    _value = v._value;
    return *this;
}

bool ValueBox::isWord() const {
	try {
		word();
	} catch (bad_get &e) {
		return false;
	}

	return true;
}

bool ValueBox::empty() const {
    return apply_visitor(IsEmptyVisitor(), _value);
}

std::string ValueBox::toString() const {
    std::stringstream ss;

    ss << _value;
    return ss.str();
}

double ValueBox::asDouble() const {
    return stod(word());
}

int32_t ValueBox::asInteger() const {
    return std::stoi(word());
}

uint32_t ValueBox::asUnsigned() const {
    return std::stoul(word());
}

WordValue &ValueBox::word() {
    return boost::get<WordValue>(_value);
}

const WordValue &ValueBox::word() const {
    return boost::get<WordValue>(_value);
}

ListValue &ValueBox::list() {
    return boost::get<ListValue>(_value);
}

const ListValue &ValueBox::list() const {
    return boost::get<ListValue>(_value);
}

ValueBox &ValueBox::push_front(const ValueBox &v) {
    list().insert(list().begin(), v._value);
    return *this;
}

ValueBox &ValueBox::push_back(const ValueBox &v) {
    list().push_back(v._value);
    return *this;
}

ValueBox &ValueBox::push_all_back(const ValueBox &v) {
    PushAllBack pab { list() };
    apply_visitor(pab, v._value);

    return *this;
}

ValueBox ValueBox::front() const {
    return apply_visitor(FrontVisitor(), _value);
}

ValueBox ValueBox::back() const {
    return apply_visitor(BackVisitor(), _value);
}

ValueBox ValueBox::butFirst() const {
    return apply_visitor(ButFirstVisitor(), _value);
}

ValueBox ValueBox::butLast() const {
    return apply_visitor(ButLastVisitor(), _value);
}

ValueBox ValueBox::at(std::size_t index) const {
    return apply_visitor(AtVisitor(index), _value);
}

bool operator==(const ValueBox &v1, const ValueBox &v2) {
    return v1._value == v2._value;
}

bool operator!=(const ValueBox &v1, const ValueBox &v2) {
    return !(v1 == v2);
}

} /* ns: types */

} /* ns: mlogo */



