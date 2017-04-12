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

    bool operator==(const ValueBox &v1) const;
    bool operator!=(const ValueBox &v1) const { return !(*this == v1); }

private:
    Value _value;
};

} /* ns: types */

} /* ns: mlogo */


namespace std {
    template<typename S>
    S &operator<<(S &s, const mlogo::types::ListValue &v) {
        for(auto &i : v) s << i;
        return s;
    }
} /* ns: std */



#endif /* VALUE_HPP_ */
