#ifndef __XPL_IDENTITY_NODE_H__
#define __XPL_IDENTITY_NODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace xpl {

  /**
   * Class for describing the identity operator
   */
  class identity_node: public cdk::unary_expression_node {
    
  public:
    inline identity_node(int lineno, expression_node *arg) :
        cdk::unary_expression_node(lineno, arg) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identity_node(this, level);
    }

  };

} // xpl

#endif
