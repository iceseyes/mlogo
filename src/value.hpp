/**
 * @file: value.hpp
 *
 *  Created on: 09 apr 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#ifndef VALUE_HPP_
#define VALUE_HPP_

#include <string>
#include <vector>

#include <boost/variant.hpp>

namespace mlogo {

namespace types {

using WordValue = std::string;

using Value = boost::make_recursive_variant<
    WordValue,
    std::vector<boost::recursive_variant_>
>::type;

using ListValue = std::vector<Value>;

std::string toString(const Value &v);


class ValueBox {
public:
    ValueBox();
    ValueBox(const char v[]);
    ValueBox(const std::string &v);
    ValueBox(const ListValue &v);
    ValueBox(const Value &v);

    ValueBox &operator=(const ValueBox &v);

    bool isWord() const;
    bool isList() const { return !isWord(); }

    bool empty() const;

    std::string toString() const;

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

    ValueBox front() const;
    ValueBox back() const;

    ValueBox butFirst() const;
    ValueBox butLast() const;
    ValueBox at(std::size_t index) const;

private:
    Value _value;

    friend bool operator==(const ValueBox &v1, const ValueBox &v2);
};

bool operator==(const ValueBox &v1, const ValueBox &v2);
bool operator!=(const ValueBox &v1, const ValueBox &v2);

} /* ns: types */

} /* ns: mlogo */


namespace std {
    template<typename S>
    S &operator<<(S &s, const mlogo::types::ListValue &v) {
        bool first { true };
        for(auto &i : v) {
            if(!first) s << " ";
            s << i;

            first = false;
        }

        return s;
    }
} /* ns: std */



#endif /* VALUE_HPP_ */
