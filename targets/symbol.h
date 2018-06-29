#ifndef __XPL_SEMANTICS_SYMBOL_H__
#define __XPL_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h> 

/* include node forward declarations */
#define __NODE_DECLARATIONS_ONLY__
#include "ast/all.h"  // automatically generated
#undef __NODE_DECLARATIONS_ONLY__

namespace xpl {

    class symbol {
      basic_type *_type;
      std::string _name;
      long _value; // hack! 
      int _scope;
      int _laddr; //offset?
      cdk::sequence_node *_args;


    public:
      inline symbol(basic_type *type, const std::string &name, long value, int scope) :
          _type(type), _name(name), _value(value), _scope(scope), _laddr(0), _args(nullptr) {
      }

      virtual ~symbol() {
        delete _type;
      }

      inline basic_type *type() const {
        return _type;
      }
      inline const std::string &name() const {
        return _name;
      }
      inline long value() const {
        return _value;
      }
      inline long value(long v) {
        return _value = v;
      }
      inline cdk::sequence_node *args() const {
        return _args;
      }
      inline cdk::sequence_node *args(cdk::sequence_node *a) {
        return _args = a;
      }
      inline int laddr() const {
        return _laddr;
      }
      inline int laddr(int v) {
        return _laddr = v;
      }
      inline int scope() const {
        return _scope;
      }
      inline int scope(int s) {
        return _scope = s;
      }
    };

} // xpl

#endif
