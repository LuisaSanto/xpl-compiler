#ifndef __XPL_NUULNODE__
#define __XPL_NUULNODE__

#include <cdk/ast/expression_node.h>

namespace xpl {

  /**
   * Class for describing null nodes.
   */
  class null_node: public cdk::expression_node {

  public:
    inline null_node(int lineno) :
        cdk::expression_node(lineno) {
          type(new basic_type(0, basic_type::TYPE_UNSPEC));
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_null_node(this, level);
    }

  };

} // xpl

#endif