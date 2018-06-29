#ifndef __XPL_POS_INDICATION_NODE_H__
#define __XPL_POS_INDICATION_NODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace xpl {

  /**
   * Class for describing memory allocation nodes.
   */
  class pos_indication_node: public cdk::unary_expression_node {

  public:
    inline pos_indication_node(int lineno, cdk::expression_node *argument) :
        cdk::unary_expression_node(lineno, argument) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_pos_indication_node(this, level);
    }

  };

} // xpl

#endif
