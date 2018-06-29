#ifndef __XPL_RETURN_NODE_H__
#define __XPL_RETURN_NODE_H__

#include <cdk/ast/basic_node.h>

namespace xpl {

  /**
   * Class for describing return nodes.
   */
  class return_node: public cdk::basic_node {

  public:
    inline return_node(int lineno) :
        cdk::basic_node(lineno) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }

  };

} // xpl

#endif
