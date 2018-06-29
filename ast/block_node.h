#ifndef __XPL_BLOCK_NODE_H__
#define __XPL_BLOCK_NODE_H__

#include <cdk/ast/sequence_node.h>

namespace xpl{

	/**
	*Class for describing block nodes.
	*/
	class block_node: public cdk::basic_node{
		cdk::sequence_node *_declarations;
		cdk::sequence_node *_statements;

	public:
		inline block_node(int lineno, cdk::sequence_node *decl, cdk::sequence_node *stat) :
		cdk::basic_node(lineno), _declarations(decl), _statements(stat){

		}

		cdk::sequence_node *declarations(){
			return _declarations;
		}

		cdk::sequence_node *statements(){
			return _statements;
		}

		void accept(basic_ast_visitor *sp, int level){
			sp->do_block_node(this, level);
		}
	}; //xpl
}

#endif