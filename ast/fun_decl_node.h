#ifndef __XPL_FUN_DECL_NODE_H__
#define __XPL_FUN_DECL_NODE_H__

#include <cdk/ast/sequence_node.h> 
#include <cdk/basic_type.h>

namespace xpl {

  /**
   * Class for describing function declaration nodes.
   */
  class fun_decl_node: public cdk::basic_node {
    basic_type *_type;
    std::string *_identifier;
    int _scope; 
    cdk::sequence_node *_args;

  public:
    inline fun_decl_node(int lineno, basic_type *type, std::string *identifier, int scope, cdk::sequence_node *args) :
        basic_node(lineno), _type(type), _identifier(identifier), _scope(scope), _args(args) {
    }

  public:
    inline basic_type *type() {
      return _type;
    }
    
    inline std::string *identifier() {
      return _identifier;
    }
    inline cdk::sequence_node *args() {
      return _args;
    }
   inline int scope() {
      return _scope;
    }
   
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_fun_decl_node(this, level);
    }

  };

} // xpl

#endif
