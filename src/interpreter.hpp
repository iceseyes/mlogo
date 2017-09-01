/**
 * @file: interpreter.hpp
 *
 *  created on:  1 settembre 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#ifndef __INTERPRETER_HPP__
#define __INTERPRETER_HPP__

#include <iostream>
#include <stdexcept>
#include <string>

#include <boost/algorithm/string.hpp>

#include "eval.hpp"
#include "memory.hpp"
#include "parser.hpp"

namespace mlogo {

template <typename InputStream, typename OutputStream,
          typename ErrorStream = OutputStream>
class Interpreter {
public:
    Interpreter(InputStream &is, OutputStream &os, ErrorStream &es)
        : _iStream(is), _oStream(os), _eStream(es) {}

    void run() {
        using namespace std;
        using namespace boost;
        using namespace mlogo::parser;
        using namespace mlogo::eval;
        using namespace mlogo::memory;

        string str;

        Procedure *currentProc{nullptr};
        Statement stmt;

        _eStream << endl << "? ";
        while (getline(_iStream, str)) {
            if (to_lower_copy(str) == "bye") break;

            AST ast;
            try {
                if (currentProc) {
                    if (currentProc->addLine(str)) {
                        Stack::instance().setProcedure(*currentProc);
                        delete currentProc;
                        currentProc = nullptr;
                        _eStream << "Procedure recorded." << endl;
                        _eStream << "? ";
                    }

                    continue;
                } else
                    stmt = parse(str);

                if (stmt.isStartProcedure()) {
                    currentProc = new Procedure(stmt);
                    continue;
                }

                if (!currentProc) {
                    ast = make_ast(stmt);
                    ast();
                }
            } catch (logic_error &e) {
                _eStream << "I don't know how to " << str << " (" << e.what()
                         << ")" << endl;
            }

            _eStream << "? ";
        }
    }

private:
    InputStream &_iStream;
    OutputStream &_oStream;
    ErrorStream &_eStream;
};

template <typename InputStream, typename OutputStream,
          typename ErrorStream = OutputStream>
Interpreter<InputStream, OutputStream, ErrorStream> getInterpreter(
    InputStream &is, OutputStream &os, ErrorStream &es) {
    return Interpreter<InputStream, OutputStream, ErrorStream>(is, os, es);
}

} /* ns: mlogo */

#endif /* __INTERPRETER_HPP__ */
