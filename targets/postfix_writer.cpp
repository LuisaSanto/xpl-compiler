#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl); 
  }
} 

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  _pf.INT(node->value()); // push an integer
}

void xpl::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.STR(node->value()); // output string characters

  /* leave the address on the stack */
  _pf.TEXT(); // return to the TEXT segment
  _pf.ADDR(mklbl(lbl1)); // the string to be printed
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.ADD();
  } else if (node->type()->name() == basic_type::TYPE_DOUBLE) {
     node->left()->accept(this, lvl);
    if (node->left()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    node->right()->accept(this, lvl);
    if (node->right()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    _pf.DADD();
  }else {
    std::cerr << "add_node - ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}
void xpl::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.SUB();
  } else if(node->type()->name() == basic_type::TYPE_DOUBLE){
    node->left()->accept(this, lvl);
    if (node->left()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    node->right()->accept(this, lvl);
    if (node->right()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    _pf.DSUB();
  }else {
    std::cerr << "sub_node - ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }

}
void xpl::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.MUL();
  } else if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    if (node->left()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    node->right()->accept(this, lvl);
    if (node->right()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    _pf.DMUL();
  } else {
    std::cerr << "mul_node - ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }

}
void xpl::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  if (node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.DIV();
  } else if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    if (node->left()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    node->right()->accept(this, lvl);
    if (node->right()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    _pf.DDIV();
  } else {
    std::cerr << "div_node - ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }

}
void xpl::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void xpl::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    if (node->left()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    node->right()->accept(this, lvl);
    if (node->right()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    _pf.DCMP();
    _pf.INT(0);
    _pf.LT();
  } else if (node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
   _pf.LT();
  } else {
    std::cerr << "lt_node - ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}
void xpl::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
    if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    if (node->left()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    node->right()->accept(this, lvl);
    if (node->right()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    _pf.DCMP();
    _pf.INT(0);
    _pf.LE();
  } else if (node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.LE();
  } else {
    std::cerr << "le_node - ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}
void xpl::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
    if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    if (node->left()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    node->right()->accept(this, lvl);
    if (node->right()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    _pf.DCMP();
    _pf.INT(0);
    _pf.GE();
  } else if (node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.GE();
  } else {
    std::cerr << "ge_node - ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}
void xpl::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
    if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    if (node->left()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    node->right()->accept(this, lvl);
    if (node->right()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    _pf.DCMP();
    _pf.INT(0);
    _pf.GT();
  } else if (node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.GT();
  } else {
    std::cerr << "gt_node - ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}
void xpl::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    if (node->left()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    node->right()->accept(this, lvl);
    if (node->right()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    _pf.DCMP();
    _pf.INT(0);
    _pf.NE();
  } else if (node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.NE();
  } else {
    std::cerr << "ne_node - ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}
void xpl::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    if (node->left()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    node->right()->accept(this, lvl);
    if (node->right()->type()->name() == basic_type::TYPE_INT) _pf.I2D();
    _pf.DCMP();
    _pf.INT(0);
    _pf.EQ();
  } else if (node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.EQ();
  } else {
    std::cerr << "eq_node - ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  const std::string &id = node->name();
  std::shared_ptr<xpl::symbol> sym = _symtab.find(id);

  if (sym->laddr() == 0) {
      _pf.ADDR(sym->name()); //vars outside functions have local addr = 0 (uninitialized by declaration)
  } else  {
      _pf.LOCAL(sym->laddr());
  }
}

void xpl::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  
  if (node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DLOAD();        
  } else if(node->lvalue()->type()->name() == basic_type::TYPE_INT || node->lvalue()->type()->name() == basic_type::TYPE_STRING) {
    _pf.LOAD();    
  } else {
    std::cerr << "rvalue_node - ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }}

void xpl::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl); // determine the new value
  _pf.DUP();
  if (new_symbol() == nullptr) {
    node->lvalue()->accept(this, lvl); // where to store the value
  } else {
    _pf.DATA(); // variables are all global and live in DATA
    _pf.ALIGN(); // make sure we are aligned
    _pf.LABEL(new_symbol()->name()); // name variable location
    _pf.CONST(0); // initialize it to 0 (zero)
    _pf.TEXT(); // return to the TEXT segment
    node->lvalue()->accept(this, lvl);  //DAVID: bah!
  }
  _pf.STORE(); // store the value at address
}

//---------------------------------------------------------------------------
/*
void xpl::postfix_writer::do_program_node(xpl::program_node * const node, int lvl) {
  // The ProgramNode (representing the whole program) is the
  // main function node.

  // generate the main function (RTS mandates that its name be "_main")
  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL("_main", _pf.FUNC());
  _pf.LABEL("_main");
  _pf.ENTER(0);  // XPL doesn't implement local variables

  node->statements()->accept(this, lvl);

  // end the main function
  _pf.INT(0);
  _pf.POP();
  _pf.LEAVE();
  _pf.RET();

  // these are just a few library function imports
  _pf.EXTERN("readi");
  _pf.EXTERN("printi");
  _pf.EXTERN("prints");
  _pf.EXTERN("println");
}*/

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.TRASH(node->argument()->type()->size()); // trash the result

  /*node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.TRASH(4); // delete the evaluated value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.TRASH(4); // delete the evaluated value's address
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.TRASH(8); // delete the evaluated value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_POINTER) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else if (node->argument()->type()->name() == basic_type::TYPE_VOID) {
    // doesnt need to trash anything
  } else {
    std::cerr << "evaluation_node - ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }*/
}

void xpl::postfix_writer::do_print_node(xpl::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.EXTERN("printi");
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.EXTERN("printd");
    _pf.CALL("printd");
    _pf.TRASH(8); // delete the printed value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.EXTERN("prints");
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }
  else {
    std::cerr << "print_node - ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_read_node(xpl::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if(node->type()->name() == basic_type::TYPE_INT){
    _pf.EXTERN("readi");
    _pf.CALL("readi");
    _pf.PUSH();
  }else {
    _pf.EXTERN("readd");
    _pf.CALL("readd");
    _pf.DPUSH();
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_while_node(xpl::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  _pf.LABEL(mklbl(lbl1 = ++_lbl));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl2 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl1));
  _pf.LABEL(mklbl(lbl2));
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_if_node(xpl::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; 
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_if_else_node(xpl::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

void xpl::postfix_writer::do_block_node(xpl::block_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  _symtab.push();
  if (node->declarations())
    node->declarations()->accept(this, lvl);
  if (node->statements())
    node->statements()->accept(this, lvl);
  _symtab.pop();
}

void xpl::postfix_writer::do_next_node(xpl::next_node * const node, int lvl){
  //nothing to do yet
}

void xpl::postfix_writer::do_stop_node(xpl::stop_node * const node, int lvl){
  //nothing to do yet
}
void xpl::postfix_writer::do_sweep_up_node(xpl::sweep_up_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2, lbl3, lbl4;

  if(node->lval() != nullptr) node->lval()->accept(this, lvl);
  _pf.LABEL(mklbl(lbl4 = ++ _lbl));
  
  if (node->init() != nullptr) node->init()->accept(this, lvl);
  _pf.LABEL(mklbl(lbl1 = ++_lbl));

  if (node->cond() != nullptr) node->cond()->accept(this, lvl);
  _pf.JZ(mklbl(lbl2 = ++_lbl));

  if(node->block() != nullptr) node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl3 = ++_lbl));
  
  if (node->after() != nullptr) node->after()->accept(this, lvl);
  _pf.JMP(mklbl(lbl1));
  _pf.LABEL(mklbl(lbl2));
}
void xpl::postfix_writer::do_sweep_down_node(xpl::sweep_down_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2, lbl3, lbl4;

  if(node->lval() != nullptr) node->lval()->accept(this, lvl);
  _pf.LABEL(mklbl(lbl4 = ++ _lbl));

  if (node->init() != nullptr) node->init()->accept(this, lvl);
  _pf.LABEL(mklbl(lbl1 = ++_lbl));

  if (node->cond() != nullptr) node->cond()->accept(this, lvl);
  _pf.JZ(mklbl(lbl2 = ++_lbl));

  if(node->block() != nullptr) node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl3 = ++_lbl));
  
  if (node->after() != nullptr) node->after()->accept(this, lvl);
  _pf.JMP(mklbl(lbl1));
  _pf.LABEL(mklbl(lbl2));
}

void xpl::postfix_writer::do_fun_call_node(xpl::fun_call_node * const node, int lvl){
    ASSERT_SAFE_EXPRESSIONS;
    int totalArgs = 0;

    // _funargs flag ON makes sequence of args be stacked in reverse order
    _funargs = true;    
    if (node->arguments() != nullptr) {
      for (int i = node->arguments()->size() - 1; i >= 0; i--) {
        node->arguments()->node(i)->accept(this, lvl);
        totalArgs += ((cdk::expression_node *) node->arguments()->node(i))->type()->size();
      }
    } //puts the arguments in the stack
    _funargs = false;

    _pf.CALL(*node->name());
    _pf.TRASH(totalArgs);
            
    if (node->type()->name() != basic_type::TYPE_VOID) {
      if (node->type()->name() == basic_type::TYPE_DOUBLE) _pf.DPUSH();
      else _pf.PUSH();
    }
}

void xpl::postfix_writer::do_fun_decl_node(xpl::fun_decl_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  _pf.TEXT();
  _pf.ALIGN();
  
  const std::string &id = *node->identifier();
  std::string name;

  if (id.compare("xpl") == 0) {
     name = "_main";
  } else if (id.compare("_main") == 0) {
     name = "xpl";
  } else { 
     name = *node->identifier();
   }
  
  if (node->scope() == XPL_EXTERN) {
     _pf.EXTERN(name);
  } else if (node->scope() == XPL_GLOBAL) {
     _pf.GLOBAL(name, _pf.FUNC());
  }
}

void xpl::postfix_writer::do_fun_def_node(xpl::fun_def_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;

  // generate the main function (RTS mandates that its name be "_main")
  
  CHECK_TYPES(_compiler, _symtab, node); 
  _infunc = node->declaration()->identifier();
  _lastlocal = 0;
  node->declaration()->accept(this, lvl);
  
  const std::string &id = *node->declaration()->identifier();

  if (id.compare("xpl") == 0) {
     _pf.LABEL("_main");
  } else if (id.compare("_main") == 0) {
     _pf.LABEL("xpl");
  } else {
     _pf.LABEL(id);
  }

  // hack!
  _pf.ENTER(40); 

  if (node->declaration()->type()->name() != basic_type::TYPE_VOID) {
     // retval will reside at the first local address of the function    
     std::shared_ptr<xpl::symbol> sym = _symtab.find(id);
     _lastlocal -= sym->type()->size();
     sym->laddr(_lastlocal);

     if (node->retval() != nullptr) {
        node->retval()->accept(this, lvl);
          if (node->declaration()->type()->name() != basic_type::TYPE_DOUBLE) { 
            _pf.LOCA(sym->laddr()); //LOCAL+STORE
          }else { 
            if (node->retval()->type()->name() == basic_type::TYPE_INT) { 
              _pf.I2D(); 
            }
           _pf.LOCAL(sym->laddr()); 
           _pf.DSTORE();
        }
     }

  }
  
  _funargs = true;
  if (node->declaration()->args() != nullptr) node->declaration()->args()->accept(this, lvl);
  _funargs = false;

  node->block()->accept(this, lvl);

  // end the function
  if (node->declaration()->type()->name() != basic_type::TYPE_VOID) {
     std::shared_ptr<xpl::symbol> sym = _symtab.find(id);
     if (node->declaration()->type()->name() != basic_type::TYPE_DOUBLE) {
        _pf.LOCV(sym->laddr());
        _pf.POP();
    } else {
        _pf.LOCAL(sym->laddr());
        _pf.DLOAD();
        _pf.DPOP();
     }
  } 
  _pf.LEAVE();
  _pf.RET();
  _infunc = nullptr;
  _lastlocal = 0;
  _argsoffset = 8;

}
void xpl::postfix_writer::do_identity_node(xpl::identity_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}

void xpl::postfix_writer::do_index_node(xpl::index_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  node->base()->accept(this, lvl);
  node->index()->accept(this, lvl);
  _pf.INT(8);
  _pf.MUL();
  _pf.ADD();
  _pf.DLOAD();
}

void xpl::postfix_writer::do_memory_alloc_node(xpl::memory_alloc_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  node->arg()->accept(this, lvl);
  _pf.INT(8);
  _pf.MUL();
  _pf.ALLOC();
  _pf.SP();
}

void xpl::postfix_writer::do_pos_indication_node(xpl::pos_indication_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}

void xpl::postfix_writer::do_print_line_node(xpl::print_line_node * const node, int lvl){

  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.EXTERN("printi");
    _pf.CALL("printi");
    _pf.TRASH(4);
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.EXTERN("printd");
    _pf.CALL("printd");
    _pf.TRASH(8);
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.EXTERN("prints");
    _pf.CALL("prints");
    _pf.TRASH(4);
  }else {
    std::cerr << "println_node - ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }

  _pf.EXTERN("println");
  _pf.CALL("println"); // just prints a newline
}

void xpl::postfix_writer::do_return_node(xpl::return_node * const node, int lvl){
  std::shared_ptr<xpl::symbol> sym = _symtab.find(*_infunc);
  if (sym->type()->name() != basic_type::TYPE_VOID) {
      _pf.LOCAL(sym->laddr());
      if (sym->type()->name() == basic_type::TYPE_DOUBLE) _pf.DLOAD();
      else _pf.LOAD();
  _pf.POP();
  }
  _pf.LEAVE();
  _pf.RET();
}

void xpl::postfix_writer::do_null_node(xpl::null_node * const node, int lvl){

}

void xpl::postfix_writer::do_variable_node(xpl::variable_node * const node, int lvl){
    std::shared_ptr<xpl::symbol> sym = _symtab.find(*node->identifier());

  if (node->scope() == XPL_EXTERN) {
     _pf.EXTERN(sym->name());
     return;
  } 

 
  if (node->initializer() != nullptr) {
     int str_lbl;
    
     if (_infunc == nullptr) { 

        if (node->type()->name() == basic_type::TYPE_STRING) {
         cdk::string_node *sn = dynamic_cast<cdk::string_node *>(node->initializer());
     _pf.RODATA();
     _pf.ALIGN();
     _pf.LABEL(mklbl(str_lbl = ++_lbl));
     _pf.STR(sn->value());
        }

      _pf.DATA();
    _pf.ALIGN(); 
     
      if (node->scope() == XPL_GLOBAL) {
     _pf.GLOBAL(sym->name(), _pf.OBJ());
  }
       _pf.LABEL(sym->name());

    if (node->type()->name() == basic_type::TYPE_INT) {
     cdk::integer_node *in = dynamic_cast<cdk::integer_node *>(node->initializer());
     _pf.CONST(in->value());
     
        } else if (node->type()->name() == basic_type::TYPE_STRING) {
           _pf.ID(mklbl(str_lbl));

        } else if (node->type()->name() == basic_type::TYPE_DOUBLE) {
     if (node->initializer()->type()->name() == basic_type::TYPE_DOUBLE) {
         cdk::double_node *dn = dynamic_cast<cdk::double_node *>(node->initializer());
         _pf.DOUBLE(dn->value());
     } else if (node->initializer()->type()->name() == basic_type::TYPE_INT) {
          cdk::integer_node *in = dynamic_cast<cdk::integer_node *>(node->initializer());
          _pf.DOUBLE(in->value()); // FIXME SEE IF THIS WORKS WITH INT OR IF NEEDS CAST TO DOUBLE
     }
        }     
      } else {

  if (!_funargs) {   
      _lastlocal -= sym->type()->size();
      sym->laddr(_lastlocal);
  } else {
      sym->laddr(_argsoffset);
      _argsoffset += sym->type()->size();
  }
  
        node->initializer()->accept(this, lvl);

  if (node->type()->name() == basic_type::TYPE_INT) {
           _pf.LOCA(sym->laddr());
           
        } else if (node->type()->name() == basic_type::TYPE_STRING) {
     _pf.LOCA(sym->laddr());

        } else if (node->type()->name() == basic_type::TYPE_DOUBLE) {
     if (node->initializer()->type()->name() == basic_type::TYPE_INT) {
        _pf.I2D();
     }
           _pf.LOCAL(sym->laddr());
     _pf.DSTORE();
 
        }     
     }  
   
   } else { 
     if (_infunc == nullptr) {
        _pf.BSS();
        _pf.ALIGN();

        if (node->scope() == XPL_GLOBAL) {
     _pf.GLOBAL(sym->name(), _pf.OBJ());
        }

        _pf.LABEL(sym->name());
        _pf.BYTE(sym->type()->size());

     } else {
  if (!_funargs) {   
      _lastlocal -= sym->type()->size();
      sym->laddr(_lastlocal);
  } else {
      sym->laddr(_argsoffset);
      _argsoffset += sym->type()->size();
  }
     }
     
  }
  _pf.TEXT();
  _pf.ALIGN();
}


