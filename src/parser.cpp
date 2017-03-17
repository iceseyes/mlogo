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
	Statement stmt;
	Parser parser;
	iterator_type iter = line.begin();
	iterator_type end = line.end();

	bool r = phrase_parse(iter, end, parser, ascii::space, stmt);

	if (!r || iter != end) {
		std::string rest(iter, end);
		throw std::logic_error("Syntax Error at input line from: " + rest);
	}

	return stmt;
}

}
}

