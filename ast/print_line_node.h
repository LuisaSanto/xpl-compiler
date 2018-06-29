#ifndef __XPL_PRINT_LINE_NODE_H__
#define __XPL_PRINT_LINE_NODE_H__

#include <cdk/ast/expression_node.h>

namespace xpl {

  /**
   * Class for describing print_line nodes with line break (similar to puts in C).
   */
  class print_line_node: public cdk::basic_node {
    cdk::expression_node *_argument;

  public:
    inline print_line_node(int lineno, cdk::expression_node *argument) :
        cdk::basic_node(lineno), _argument(argument) {
    }

  public:
    inline cdk::expression_node *argument() {
      return _argument;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_print_line_node(this, level);
    }

  };

} // xpl

#endif