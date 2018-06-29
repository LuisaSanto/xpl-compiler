#ifndef __XPL_NEXT_NODE_H__
#define __XPL_NEXT_NODE_H__

#include <cdk/ast/basic_node.h>

namespace xpl {

  /**
   * Class for describing next nodes.
   */
  class next_node: public cdk::basic_node {

  public:
    inline next_node(int lineno) :
        cdk::basic_node(lineno) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_next_node(this, level);
    }

  };

} // zu

#endif
