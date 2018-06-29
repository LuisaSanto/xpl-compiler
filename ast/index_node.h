#ifndef __XPL_INDEXNODE_H__
#define __XPL_INDEXNODE_H__

#include <cdk/ast/lvalue_node.h>

namespace xpl {

  /**
   * Class for describing index nodes.
   */
  class index_node: public cdk::lvalue_node {
    cdk::expression_node *_base;
    cdk::expression_node *_index;

  public:
    inline index_node(int lineno, cdk::expression_node *base, cdk::expression_node *index) :
       lvalue_node(lineno), _base(base), _index(index) {
    } 

    inline cdk::expression_node *base() { return _base; }
    inline cdk::expression_node *index() { return _index; }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

} // xpl

#endif