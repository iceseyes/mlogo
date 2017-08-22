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
	EvalStmtBuilderVisitor(ASTNode *node) :
		node(node) {}

	EvalStmtBuilderVisitor(AST *node) :
	    ast(node) {}

	void operator()(mlogo::parser::Word &v) const {
		setParent();

		new ASTNode (
			new ASTNode::Const(v.name), node);
	}

	void operator()(mlogo::parser::Expression &e) const {
		switch(e.node) {
		case parser::Expression::Node::NUMBER: this->operator()(e.number()); break;
		case parser::Expression::Node::VARIABLE: this->operator()(e.variable()); break;
		case parser::Expression::Node::STATEMENT: this->operator()(e.statement()); break;
		case parser::Expression::Node::FUNCTION:
			this->operator()(e.functor());
			for(auto &child: e.children) this->operator()(child);
			break;
		}
	}

	void operator()(const mlogo::parser::Number &n) const {
		setParent();

		new ASTNode (
			new ASTNode::Const(n.value), node);
    }


	void operator()(const mlogo::parser::Variable &v) const {
		setParent();

		new ASTNode (
			new ASTNode::Variable(v.name), node);
    }

	void operator()(const mlogo::parser::ProcName &v) const {
		setParent(true);

		if(!node) node = ast->createNode(v.name);
		else {
            node = new ASTNode (
                new ASTNode::Procedure(v.name), node);
		}
	}

	void operator()(const mlogo::parser::Statement &s) const {
		ASTNode *parent = node;
		setParent(true);

		node = { new ASTNode(make_statement(s)) };

		if(parent) node->setParent(parent);
	}

    void operator()(mlogo::parser::List &v) const {
		setParent();
		types::ListValue list;

		for(auto &w : v.items) {
		    std::stringstream ss;
		    ss << w;
		    list.push_back(ss.str());
		}

		new ASTNode (
			new ASTNode::List(list), node);
    }

private:
	void setParent(bool procedure=false) const {
		while(node && node->completed())
			node = node->parent();

		if(!node && (!ast || !procedure)) {
			throw std::logic_error("Exceed Procedure Arguments.");
		}
	}

	mutable ASTNode *node { nullptr };
	mutable AST *ast { nullptr };
};



} /* ns: impl */

} /* ns: eval */

} /* ns: mlogo */


#endif /* __EVAL_IMPL_HPP__ */
