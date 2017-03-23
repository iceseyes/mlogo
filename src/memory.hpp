/**
 * @file: memory.hpp
 *
 *  Created on: 18 mar 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include <map>
#include <vector>
#include <string>
#include <functional>

namespace mlogo {

namespace memory {

class Frame {
public:
	std::map<std::string, std::function<void (Frame *)>> procedures;
	std::map<std::string, std::string> variables;
	std::vector<std::string> arguments;
};

} /* ns memory */

} /* ns mlogo */



#endif /* MEMORY_HPP_ */
