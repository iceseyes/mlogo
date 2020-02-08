/**
 * @file: interpreter.hpp
 *
 *  created on:  1 settembre 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#ifndef __INTERPRETER_HPP__
#define __INTERPRETER_HPP__

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

#include "defines.hpp"
#include "eval.hpp"
#include "exceptions.hpp"
#include "memory.hpp"
#include "parser.hpp"

namespace mlogo {

struct InterpreterState {
    static InterpreterState &instance() {
        static InterpreterState __instance;
        return __instance;
    }

    void bye() { _running = false; }
    bool running() const { return _running; }

private:
    InterpreterState() : _running(true) {}
    InterpreterState(const InterpreterState &) = delete;
    InterpreterState(InterpreterState &&) = delete;

    InterpreterState &operator=(const InterpreterState &) = delete;
    InterpreterState &operator=(InterpreterState &&) = delete;

    bool _running;
};

template <typename InputStream, typename OutputStream,
          typename ErrorStream = OutputStream>
class Interpreter {
public:
    Interpreter(InputStream &is, OutputStream &os, ErrorStream &es,
                bool wPrompt, bool rethrow)
        : _iStream(is),
          _oStream(os),
          _eStream(es),
          _interactive(wPrompt),
          _rethrow(rethrow) {}

    void run() {
        using namespace std;
        using namespace boost;
        using namespace mlogo::parser;
        using namespace mlogo::eval;
        using namespace mlogo::memory;

        string str;

        Procedure *currentProc{nullptr};
        Statement stmt;

        showPrompt();
        while (InterpreterState::instance().running() &&
               getline(_iStream, str)) {
            AST ast;
            try {
                if (currentProc) {
                    if (currentProc->addLine(str)) {
                        Stack::instance().setProcedure(*currentProc);
                        delete currentProc;
                        currentProc = nullptr;

                        if (_interactive)
                            _eStream << "Procedure recorded." << endl;

                        showPrompt();
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

                if (_rethrow) throw e;
            }

            showPrompt();
        }
    }

    /**
     * Parse one string and if is a valid logo statement try to
     * run it. It does not support procedure definition.
     *
     * @param[in] line a string to parse and execute.
     * @throw mlogo::exceptions::Syntaxerror is string is not right
     * @throw mlogo::exceptions::InvalidStatmentException if string is a
     *          procedure definition.
     */
    void one(const std::string &line) const {
        using namespace mlogo::parser;
        using namespace mlogo::eval;

        auto stmt = parse(line);
        if (stmt.isStartProcedure())
            throw exceptions::InvalidStatmentException(line);

        auto ast = make_ast(stmt);
        ast.exec();
    }

    void startup() const {
        auto line =
            mlogo::memory::Stack::instance().getVariable("startup").toString();
        one(line);
    }

protected:
    void showPrompt() {
        if (_interactive) _eStream << "? ";
    }

private:
    InputStream &_iStream;
    OutputStream &_oStream;
    ErrorStream &_eStream;
    bool _interactive;
    bool _rethrow;
};

template <typename InputStream, typename OutputStream,
          typename ErrorStream = OutputStream>
Interpreter<InputStream, OutputStream, ErrorStream> getInterpreter(
    InputStream &is, OutputStream &os, ErrorStream &es, bool wPrompt = true,
    bool rethrow = false) {
    return Interpreter<InputStream, OutputStream, ErrorStream>(
        is, os, es, wPrompt, rethrow);
}

template <typename InputStream, typename OutputStream,
          typename ErrorStream = OutputStream>
Interpreter<InputStream, OutputStream, ErrorStream> *getInterpreterPtr(
    InputStream &is, OutputStream &os, ErrorStream &es, bool wPrompt = true,
    bool rethrow = false) {
    return new Interpreter<InputStream, OutputStream, ErrorStream>(
        is, os, es, wPrompt, rethrow);
}

using StdDynamicInterpreter =
    Interpreter<std::istream, std::ostream, std::ostream>;

}  // namespace mlogo

#endif /* __INTERPRETER_HPP__ */
