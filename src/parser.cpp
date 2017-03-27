/**
 * @file: parser.cpp
 *
 *  Created on: 15 mar 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */
#include "parser.hpp"
#include "parser_impl.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

namespace mlogo {
namespace parser {

using iterator_type = std::string::const_iterator;
using Parser = StatementParser<iterator_type>;

Statement parse(const std::string &line) {
	return parse<StatementParser, Statement>(line);
}

::std::ostream &operator<<(::std::ostream &s, const Number &n) {
	s << n.value;
	return s;
}

::std::ostream &operator<<(::std::ostream &s, const Word &n) {
	s << n.name;
	return s;
}

::std::ostream &operator<<(::std::ostream &s, const Variable &n) {
	s << n.name;
	return s;
}

::std::ostream &operator<<(::std::ostream &s, const ProcName &n) {
	s << n.name;
	return s;
}

::std::ostream &operator<<(::std::ostream &s, const List &n) {
    char space = 0;
    s << "[";
    for(auto &v : n.items) {
        if(space) s << space;
        space = ' ';
        s << v;
    }
    s << "]";
    return s;
}

}
}

