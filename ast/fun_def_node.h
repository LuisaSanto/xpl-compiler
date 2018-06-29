#ifndef __XPL_FUN_DEF_NODE_H__
#define __XPL_FUN_DEF_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include "ast/fun_decl_node.h" 

namespace xpl {

  /**
   * Class for describing function definition nodes. 
   */
  class fun_def_node: public cdk::basic_node {
    xpl::fun_decl_node *_declaration;
    cdk::expression_node *_retval;
    cdk::basic_node *_block;

  public:
    inline fun_def_node(int lineno, fun_decl_node *declaration, cdk::expression_node *retval, cdk::basic_node *block) :
        cdk::basic_node(lineno), _declaration(declaration), _retval(retval), _block(block) {
    }

  public:
    inline cdk::basic_node *block() {
      return _block;
    }

   inline cdk::expression_node *retval() {
      return _retval;
    }

   inline fun_decl_node *declaration() {
      return _declaration;
    }


    void accept(basic_ast_visitor *sp, int level) {
      sp->do_fun_def_node(this, level);
    }

  };

} // xpl

#endif

