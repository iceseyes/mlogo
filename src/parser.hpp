/**
 * @file: parser.hpp
 *
 */
#include <string>
#include <vector>

namespace mlogo { namespace parser {

    struct Argument {
      std::string name;
    };
      
    struct Statement {
      std::string name;
      std::vector<Argument> arguments;

      Statement() {}
      Statement(const std::string &name) { this->name = name; }
      
    };

    Statement parse(const std::string &line);
}}
