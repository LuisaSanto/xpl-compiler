#ifndef __XPL_VARIABLE_NODE_H__
#define __XPL_VARIABLE_NODE_H__


#include "cdk/ast/lvalue_node.h"
#include <cdk/ast/basic_node.h>
#include <cdk/basic_type.h>

namespace xpl {

  /** 
   * Class for desclaring variables.
   */
  class variable_node: public cdk::basic_node {
    basic_type *_type;
    std::string *_identifier; 
    int _scope;
    cdk::expression_node *_initializer;

  public:
    /**
     * @param lineno source code line number for this node
     */
    inline variable_node(int lineno, basic_type *type, std::string *identifier, int scope, cdk::expression_node *initializer) :
      cdk::basic_node(lineno), _type(type), _identifier(identifier), _scope(scope), _initializer(initializer) {
    }
    
    inline std::string *identifier() {
      return _identifier;
    }
    inline basic_type *type() {
      return _type;
    }

    inline int scope() {
      return _scope;
    }
    
    inline cdk::expression_node *initializer() {
      return _initializer;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_node(this, level);
    }

  };

} // xpl

#endif
