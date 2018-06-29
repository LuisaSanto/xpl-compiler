#ifndef __XPL_SWEEP_UP_NODE_H__
#define __XPL_SWEEP_UP_NODE_H__

#include <cdk/ast/sequence_node.h>
#include <cdk/ast/lvalue_node.h>

namespace xpl {

  /**
   * Class sweep down describing sweep down nodes.
   */
  class sweep_up_node: public cdk::basic_node {
    cdk::lvalue_node *_lval;
    cdk::sequence_node *_init;
    cdk::sequence_node *_cond;
    cdk::sequence_node *_after;
    cdk::basic_node *_block;

  public:
    inline sweep_up_node(int lineno, cdk::lvalue_node *lval, cdk::sequence_node *initialization, cdk::sequence_node *condition,
                    cdk::sequence_node *afterthought, cdk::basic_node *block) :
        basic_node(lineno), _lval(lval), _init(initialization), _cond(condition), _after(afterthought), _block(block) {
    }

  public:
    inline cdk::lvalue_node *lval() {
      return _lval;
    }

    inline cdk::sequence_node *init() {
      return _init;
    }

    inline cdk::sequence_node *cond() {
      return _cond;
    }

    inline cdk::sequence_node *after() {
      return _after;
    }

    inline cdk::basic_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_sweep_up_node(this, level);
    }

  };

} // xpl

#endif
