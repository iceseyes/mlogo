/**
 * @file: exceptions.hpp
 *
 *  created on: 26 agosto 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#ifndef __EXCEPTIONS_HPP__
#define __EXCEPTIONS_HPP__

#include <sstream>
#include <stdexcept>
#include <string>

namespace mlogo {

namespace exceptions {

struct SyntaxError : std::logic_error {
    std::string line;  //!< The original input string
    std::size_t pos;   //!< position in line where first error occured

    SyntaxError(const std::string &line, std::size_t pos)
        : std::logic_error(makeMessage(line, pos)), line(line), pos(pos) {}

private:
    static std::string makeMessage(const std::string &line, std::size_t pos) {
        std::stringstream ss;
        ss << "Syntax Error in position " << pos << ": " << line.substr(pos);

        return ss.str();
    }
};

} /* ns: exceptions */

} /* ns: mlogo */

#endif /* __EXCEPTIONS_HPP__ */
