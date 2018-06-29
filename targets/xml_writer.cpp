#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void xpl::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
  closeTag(node, lvl);
} 

//---------------------------------------------------------------------------

void xpl::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}

void xpl::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

//---------------------------------------------------------------------------

inline void xpl::xml_writer::do_unary_expression(cdk::unary_expression_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
} 

void xpl::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}

//---------------------------------------------------------------------------

inline void xpl::xml_writer::do_binary_expression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;
}

void xpl::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl); 

  node->lvalue()->accept(this, lvl);
  reset_new_symbol();

  node->rvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------
/*
void xpl::xml_writer::do_program_node(xpl::program_node * const node, int lvl) {
  openTag(node, lvl);
  node->statements()->accept(this, lvl + 4);
  closeTag(node, lvl);
}*/

//---------------------------------------------------------------------------

void xpl::xml_writer::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_print_node(xpl::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_read_node(xpl::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_while_node(xpl::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_if_node(xpl::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_if_else_node(xpl::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_block_node(xpl::block_node * const node, int lvl){
  openTag(node, lvl);
  openTag("declarations", lvl+2);
  if (node->declarations() != nullptr) node->declarations()->accept(this, lvl + 4);
  closeTag("declarations", lvl+2);
  openTag("statements", lvl+2);
  if (node->statements() != nullptr) node->statements()->accept(this, lvl + 4);
  closeTag("statements", lvl+2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_next_node(xpl::next_node * const node, int lvl){
  openTag(node, lvl);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_stop_node(xpl::stop_node * const node, int lvl){
  openTag(node, lvl);
  closeTag(node, lvl); 
}

void xpl::xml_writer::do_sweep_up_node(xpl::sweep_up_node * const node, int lvl){
  openTag(node, lvl);
  openTag("lval", lvl + 2);
  node->lval()->accept(this, lvl + 4);
  closeTag("lval", lvl + 2);
  openTag("initialization", lvl + 2);
  node->init()->accept(this, lvl + 4);
  closeTag("initialization", lvl + 2);
  openTag("condition", lvl + 2);
  node->cond()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("after", lvl + 2);
  node->after()->accept(this, lvl + 4);
  closeTag("after", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_sweep_down_node(xpl::sweep_down_node * const node, int lvl){
  openTag(node, lvl);
  openTag("lval", lvl + 2);
  node->lval()->accept(this, lvl + 4);
  closeTag("lval", lvl + 2);
  openTag("initialization", lvl + 2);
  node->init()->accept(this, lvl + 4);
  closeTag("initialization", lvl + 2);
  openTag("condition", lvl + 2);
  node->cond()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("after", lvl + 2);
  node->after()->accept(this, lvl + 4);
  closeTag("after", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_fun_call_node(xpl::fun_call_node * const node, int lvl){
  openTag(node, lvl);
  openTag("name", lvl + 2);
  os() << std::string(lvl + 4, ' ') << *node->name() << std::endl;
  closeTag("name", lvl + 2);
  openTag("arguments", lvl + 2);
  if (node->arguments()!=nullptr) node->arguments()->accept(this, lvl + 4);
  closeTag("arguments", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_fun_decl_node(xpl::fun_decl_node * const node, int lvl){
  openTag(node, lvl);
  openTag("type", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->type()->name() << std::endl;
  closeTag("type", lvl + 2);
  openTag("identifier", lvl + 2);
  os() << std::string(lvl + 4, ' ') << *node->identifier() << std::endl;
  closeTag("identifier", lvl + 2);
  openTag("scope", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->scope() << std::endl;
  closeTag("scope", lvl + 2);
  openTag("arguments", lvl + 2);
  if(node->args() != nullptr) node->args()->accept(this, lvl + 4);
  closeTag("arguments", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_fun_def_node(xpl::fun_def_node * const node, int lvl){
  openTag(node, lvl);
  openTag("declarations", lvl+2);
  node->declaration()->accept(this, lvl+4);
  closeTag("declarations", lvl+2);
  openTag("retval", lvl + 2);
  if (node->retval() != nullptr) node->retval()->accept(this, lvl + 4);
  closeTag("retval", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_identity_node(xpl::identity_node * const node, int lvl){
  do_unary_expression(node, lvl);
}

void xpl::xml_writer::do_index_node(xpl::index_node * const node, int lvl){
  openTag(node, lvl);
  openTag("base", lvl + 2);
  node->base()->accept(this, lvl + 4);
  closeTag("base", lvl + 2);
  openTag("index", lvl + 2);
  node->index()->accept(this, lvl + 4);
  closeTag("index", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_memory_alloc_node(xpl::memory_alloc_node * const node, int lvl){
  openTag(node, lvl);
  openTag("pointer", lvl + 2);
  node->pointer()->accept(this, lvl + 4);
  closeTag("pointer", lvl + 2);
  openTag("argument", lvl + 2);
  node->arg()->accept(this, lvl + 4);
  closeTag("argument", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_pos_indication_node(xpl::pos_indication_node * const node, int lvl){
  do_unary_expression(node, lvl);
}

void xpl::xml_writer::do_print_line_node(xpl::print_line_node * const node, int lvl){
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_return_node(xpl::return_node * const node, int lvl){
  openTag(node, lvl);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_null_node(xpl::null_node * const node, int lvl){
  openTag(node, lvl);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_variable_node(xpl::variable_node * const node, int lvl){
  openTag(node, lvl);
  openTag("type", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->type()->name() << std::endl;
  closeTag("type", lvl + 2);
  openTag("identifier", lvl + 2);
  os() << std::string(lvl + 4, ' ') << *node->identifier() << std::endl;
  closeTag("identifier", lvl + 2);
  openTag("scope", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->scope() << std::endl; 
  closeTag("scope", lvl + 2);
  openTag("value", lvl + 2);
  if (node->initializer() != nullptr) node->initializer()->accept(this, lvl + 4);
  closeTag("value", lvl + 2);
  closeTag(node, lvl);
}





