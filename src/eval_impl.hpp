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
		setParent();

		new mlogo::eval::Statement (
			new mlogo::eval::Statement::Const(v.name), node);
	}

	void operator()(mlogo::parser::Number &v) const {
		setParent();

		new mlogo::eval::Statement (
			new mlogo::eval::Statement::Const(v.value), node);

	}

	void operator()(mlogo::parser::ProcName &v) const {
		setParent();

		node = new mlogo::eval::Statement (
			new mlogo::eval::Statement::Procedure(v.name), node);
	}

    void operator()(mlogo::parser::List &v) const {
		setParent();

		std::stringstream ss;
        ss << v;

		new mlogo::eval::Statement (
			new mlogo::eval::Statement::Const(ss.str()), node);
    }

private:
	void setParent() const {
		while(node && node->completed())
			node = node->parent();

		if(!node) {
			throw std::logic_error("Exceed Procedure Arguments.");
		}
	}

	mutable mlogo::eval::Statement *node;
};



} /* ns: impl */

} /* ns: eval */

} /* ns: mlogo */


#endif /* __EVAL_IMPL_HPP__ */
