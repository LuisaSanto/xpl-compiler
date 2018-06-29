#ifndef __XPL_MEMORY_ALLOC_NODE_H__
#define __XPL_MEMORY_ALLOC_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>

namespace xpl {

  /**
   * Class for describing memory allocation nodes.
   */
  class memory_alloc_node: public cdk::expression_node {
    cdk::lvalue_node *_pointer;
    cdk::expression_node *_arg;

  public:
    inline memory_alloc_node(int lineno, cdk::lvalue_node *pointer, cdk::expression_node *arg) :
        cdk::expression_node(lineno), _pointer(pointer), _arg(arg) {
    }
  
    inline cdk::expression_node *arg(){return _arg;}
    inline cdk::lvalue_node *pointer(){return _pointer;}

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_memory_alloc_node(this, level);
    }

  };

}

#endif