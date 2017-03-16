/**
 * @file: parser.cpp
 *
 *  Created on: 15 mar 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */
#include "parser.hpp"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

#include <iostream>
#include <string>

namespace mlogo { namespace parser {

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
struct StatementParser : qi::grammar<Iterator, Statement(), ascii::space_type> {
     StatementParser() : StatementParser::base_type(start) {
	 using ascii::char_;

	 word %= +(char_ - ' ');
	 
	 start %= word >> *(' ' >> word);
     }

     qi::rule<Iterator, std::string(), ascii::space_type> word;
     qi::rule<Iterator, Statement(), ascii::space_type> start;
};

Statement parse(const std::string &line) {
    typedef std::string::const_iterator iterator_type;

    StatementParser<iterator_type> parser; // Our grammar

    std::string str;
    Statement stmt;
    while (std::getline(std::cin, str)) {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q') break;

	std::string::const_iterator iter = str.begin();
	std::string::const_iterator end = str.end();
	
	bool r = phrase_parse(iter, end, parser, ascii::space, stmt);

	if (r && iter == end) {
	     std::cout << "-------------------------\n";
	     std::cout << "Parsing succeeded\n";
	     std::cout << "result = " << stmt.name << std::endl;
	     std::cout << "-------------------------\n";
	} else {
	     std::string rest(iter, end);
	     std::cout << "-------------------------\n";
	     std::cout << "Parsing failed\n";
	     std::cout << "stopped at: \": " << rest << "\"\n";
	     std::cout << "-------------------------\n";
	}

    }

    std::cout << "Bye... :-) \n\n";
    return stmt;
}
	  
}}





