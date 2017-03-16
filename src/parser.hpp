/**
 * @file: parser.hpp
 *
 *  Created on: 16 mar 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */
#include <string>
#include <vector>

#include <boost/fusion/include/adapt_struct.hpp>

namespace mlogo {
namespace parser {

struct Argument {
	std::string name;

	Argument() {}
	Argument(const std::string &name) :
			name(name) {}
};

struct Statement {
	std::string name;
	std::vector<Argument> arguments;

	Statement() {}
	Statement(const std::string &name) {
		this->name = name;
	}

};

Statement parse(const std::string &line);

}

}

BOOST_FUSION_ADAPT_STRUCT(
	mlogo::parser::Argument,
	(std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(
	mlogo::parser::Statement,
	(std::string, name)
	(std::vector<mlogo::parser::Argument>, arguments)
)
