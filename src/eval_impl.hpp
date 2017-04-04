/**
 * @file: eval_impl.hpp
 *
 *  created on:  4 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#ifndef __EVAL_IMPL_HPP__
#define __EVAL_IMPL_HPP__

#include "eval.hpp"

#include <sstream>

#include <boost/variant.hpp>

#include "parser.hpp"

namespace mlogo {

namespace eval {

namespace impl {

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



} /* ns: impl */

} /* ns: eval */

} /* ns: mlogo */


#endif /* __EVAL_IMPL_HPP__ */
