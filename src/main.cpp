/**
 * @file: main.cc
 *
 *  Created on: 15 mar 2017
 *      Author: massimo Bianchi
 */

#include <iostream>

#include "parser.hpp"
#include "memory.hpp"
#include "eval.hpp"

using namespace std;

struct Print : mlogo::types::BasicProcedure {
	Print() : BasicProcedure(1) {}
	void operator()() const override {
		std::string arg = fetchArg(0);
		cout << arg << endl;
	}
};

struct Sum : mlogo::types::BasicProcedure {
	Sum() : BasicProcedure(2, true) {}
	void operator()() const override {
		std::stringstream ss;
		double arg0 = std::stod(fetchArg(0));
		double arg1 = std::stod(fetchArg(1));

		double result = arg0 + arg1;
		long rlong = static_cast<long>(result);

		if(result - rlong < 1e-5)
			ss << rlong;
		else
			ss << result;

		setReturnValue(ss.str());
	}
};

struct EvalStmtBuilderVisitor : boost::static_visitor<void> {
	EvalStmtBuilderVisitor(mlogo::eval::Statement *node) :
		node(node) {}

	template<typename Value>
	void operator()(Value &&v) const {
		if(!counter) node = node->parent();
		counter--;

		new mlogo::eval::Statement (
			new mlogo::eval::Statement::Const(v.name), node);
	}

	void operator()(mlogo::parser::Number &v) const {
		if(!counter) node = node->parent();
		counter--;

		new mlogo::eval::Statement (
			new mlogo::eval::Statement::Const(v.value), node);

	}

	void operator()(mlogo::parser::ProcName &v) const {
		node = new mlogo::eval::Statement (
			new mlogo::eval::Statement::Procedure(v.name), node);

		auto *f = mlogo::memory::Stack::instance().globalFrame().getProcedure(v.name);
		counter = f->nArgs();
	}

    void operator()(mlogo::parser::List &v) const {
		if(!counter) node = node->parent();
		counter--;
		std::stringstream ss;
        ss << v;

		new mlogo::eval::Statement (
			new mlogo::eval::Statement::Const(ss.str()), node);
    }

	mutable mlogo::eval::Statement *node;
	mutable int counter { -1 };
};

int main(int argc, char **argv) {
	mlogo::memory::Stack::instance().globalFrame().setProcedure<Print>("print");
	mlogo::memory::Stack::instance().globalFrame().setProcedure<Sum>("sum");

	string str;
	cout << endl << "? ";
	while(std::getline(std::cin, str)) {
		if(str.empty() || str[0] == 'q' || str[0] == 'Q') break;

		auto stmt = mlogo::parser::parse(str);
/*		mlogo::memory::ActualArguments args;
		for(auto a : stmt.arguments) args.push_back(boost::apply_visitor(mlogo::parser::DisplayArgumentVisitor(), a));
		mlogo::memory::Stack::instance().callProcedure(stmt.name.name, args);
*/

		auto s = new mlogo::eval::Statement ( new mlogo::eval::Statement::Procedure(stmt.name.name) );
		EvalStmtBuilderVisitor v(s);
		for(auto a : stmt.arguments) {
			boost::apply_visitor(v, a);
		}

		(*s)();

		delete s;
		cout << endl << "? ";
	}

	return 0;
}

