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

/**
 * A parser syntax error.
 *
 * When during the parsing of a string, part of the input cannot be
 * parsed (cause not accepted by grammar), this exception is raised.
 *
 * A SyntaxError return the original input string and the position
 * where error occured.
 */
struct SyntaxError : std::logic_error {
    std::string line;  //!< The original input string
    std::size_t pos;   //!< position in line where first error occured

    /**
     * Build a new SyntaxError from an input line and a position
     * where error occured.
     *
     * @param[in] line the original input string
     * @param[in] pos  the index in line where error occured
     */
    SyntaxError(const std::string &line, std::size_t pos)
        : std::logic_error(makeMessage(line, pos)), line(line), pos(pos) {}

private:
    static std::string makeMessage(const std::string &line, std::size_t pos) {
        std::stringstream ss;
        ss << "Syntax Error in position " << pos << ": " << line.substr(pos);

        return ss.str();
    }
};

/**
 * When a procedure with a wrong or corrupted body is submitted
 * to store in memory, this exception should be raised.
 *
 * Report *name* of the procedure and an error code used
 * to understand the cause of the error.
 */
struct InvalidProcedureBody : std::logic_error {
    enum Error {
        NULL_PTR  //!< function pointer is null
    };

    std::string procedureName;  //!< Procedure name
    Error error;                //!< Error code

    /**
     * Make a new exception encoding some error in the procedure
     * body specification.
     *
     * @param[in] name name for the procedure
     * @param[in] code an error code used to describe the reason
     */
    InvalidProcedureBody(const std::string &name, Error code)
        : std::logic_error(makeMessage(name, code)),
          procedureName(name),
          error(code) {}

private:
    static std::string makeMessage(const std::string &name, Error code) {
        std::stringstream ss;
        ss << "Invalid body for procedure " << name << ": ";

        switch (code) {
        case NULL_PTR:
            ss << "Null pointer exception.";
            break;
        }

        return ss.str();
    }
};

/**
 * When a function which must return a value, do not this,
 * this exception should be raised.
 */
struct NoReturnValueException : std::logic_error {
    /// Build a No Return Value Exception.
    NoReturnValueException()
        : std::logic_error("Procedure does not recored any value to return.") {}
};

/**
 * You are looking for a procedure which does not exist in memory.
 */
struct UndefinedProcedure : std::logic_error {
    std::string name;  //!< name of the procedure undefined

    /**
     * Build a new exception from the name passed as argument.
     *
     * @param[in] name name of the procedure undefined.
     */
    UndefinedProcedure(const std::string &name)
        : std::logic_error("Procedure " + name + " was not found in memory."),
          name(name) {}
};

/**
 * Memory Frame is unclosable.
 *
 * If you try to call closeFrame() on an unclosable frame (global frame for
 * instance), you get this exception.
 */
struct UnclosableFrameException : std::logic_error {
    /// Build a new Exception.
    UnclosableFrameException() : std::logic_error("Frame cannot be closed.") {}
};

/**
 * Memory Frame does not define a return area.
 *
 * Last invoked function has returned a value, but no area for record it was
 * defined.
 */
struct InvalidReturnValue : std::logic_error {
    /// Build a new Exception.
    InvalidReturnValue()
        : std::logic_error("Procedure cannot return to none.") {}
};

/**
 * Memory Frame waits for a returned value, but nothing was returned.
 */
struct ExpectedReturnValue : std::logic_error {
    /// Build a new Exception.
    ExpectedReturnValue()
        : std::logic_error("Expected a function, found procedure instead.") {}
};

/**
 * ASTNodeAlreadyConnected is used when you try to re-parent an AST node
 * which is already connected to a AST.
 */
struct ASTNodeAlreadyConnected : std::logic_error {
    /// Build a new Exception.
    ASTNodeAlreadyConnected()
        : std::logic_error("This ASTNode belongs to another parent.") {}
};

/**
 * You are using differente reference system without specify
 * which is to use.
 */
struct UndefinedReferenceSystem : std::logic_error {
    /// Build a new Exception.
    UndefinedReferenceSystem()
        : std::logic_error("Ambiguous Reference System.") {}
};

/**
 * You are trying to define a new procedure in a place
 * where new procedures cannot be declared (like body of a repeat).
 */
struct InvalidStatmentException : std::logic_error {
    /// Build a new Exception.
    InvalidStatmentException(const std::string &line)
        : std::logic_error("Invalid Statment: " + line) {}
};

} /* ns: exceptions */

} /* ns: mlogo */

#endif /* __EXCEPTIONS_HPP__ */
