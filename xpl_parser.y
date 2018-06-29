%{
// $Id: xpl_parser.y,v 1.8 2017/05/20 23:28:33 ist179758 Exp $
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE       compiler->scanner()->lineno()
#define yylex()    compiler->scanner()->scan()
#define yyerror(s) compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}
  
%union {
  int                  i;  /* integer value */
  double               d; /*souble value*/
  std::string          *s;  /* symbol name or string literal */
  cdk::basic_node      *node; /* node pointer */
  cdk::sequence_node   *sequence;
  xpl::fun_decl_node   *fdecl;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  basic_type           *type;

#define XPL_EXTERN 0
#define XPL_BLOCK 1 
#define XPL_GLOBAL 2
};

%token tPRINTLN tREAD tPUBLIC tUSE tIF tELSEIF tELSE tWHILE tSWEEP tNEXT tNULL tSTOP tRETURN
%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tIDENTIFIER tSTRING tSTRINGIF tREAL tINT tPROCEDURE

%nonassoc tIFX
%nonassoc ':'

%nonassoc tBUILD
%right '='
%left '|'
%left '&'
%nonassoc '~'
%left tEQ tNE
%left tGE tLE '<' '>'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tPUBLIC '?' tINT
%nonassoc tUNARY  
%nonassoc '(' ')' '[' ']'
%left tSTRING

%type <s> string
%type <type> type void 
%type <fdecl> v_func t_func x_func 
%type <node> decl file var_decl gx_var b_var bvar_decl f_def body inst cond cond_elseif iter block 
%type <sequence> decls funvars funvars_eps insts bvars exprs exprs_eps
%type <expression> expr lit 
%type <lvalue> id lval

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file  : decls         { compiler->ast(new cdk::sequence_node(LINE, $1)); }
      | /* epsilon */ { compiler->ast(new cdk::sequence_node(LINE, new cdk::nil_node(LINE))); } 
      ;

 /* ------------------------DECLARACOES------------------------ */


decls  : decl         { $$ = new cdk::sequence_node(LINE,$1); }
       | decls decl   { $$ = new cdk::sequence_node(LINE, $2, $1); }
       ;

decl  : var_decl ';'  { $$ = $1; }
      | f_def         { $$ = $1; }
      | x_func        { $$ = $1; }
      | t_func        { $$ = $1; }
      | v_func        { $$ = $1; }
      ;

/* --------------------------------------------------------- */
/* ------------------------VARIAVEIS------------------------ */
/* --------------------------------------------------------- */

var_decl : b_var    { $$ = $1; }
         | gx_var   { $$ = $1; }
         ;

gx_var  : tPUBLIC type tIDENTIFIER'=' lit      { $$ = new xpl::variable_node(LINE, $2, $3, XPL_GLOBAL, $5); }
        | tPUBLIC type tIDENTIFIER             { $$ = new xpl::variable_node(LINE, $2, $3, XPL_GLOBAL, nullptr); }
        | tUSE type tIDENTIFIER                { $$ = new xpl::variable_node(LINE, $2, $3, XPL_EXTERN, nullptr); }
        ;

 /* ----- BVARS ------ */ 
 /* variaveis de bloco - {declaracao[=init]}*, nao sao globais nem externas */

bvar_decl : type tIDENTIFIER       { $$ = new xpl::variable_node(LINE, $1, $2, XPL_BLOCK, nullptr); }
          ;   

b_var : bvar_decl                  { $$ = $1; }
      | type tIDENTIFIER'=' expr   { $$ = new xpl::variable_node(LINE, $1, $2, XPL_BLOCK, $4); } 
      ;  

 /* ----- FUNVARS ------ */
 /* bvars usadas em declaracoes de funcoes. nao tem inicializacao */

funvars : bvar_decl               { $$ = new cdk::sequence_node(LINE, $1); }
        | funvars ',' bvar_decl   { $$ = new cdk::sequence_node(LINE, $3, $1); }
        ;

funvars_eps : funvars     { $$ = $1; }
            | /* eps */   { $$ = nullptr; } 
            ;

 /* ------------------------------------------------------- */
 /* ------------------------FUNCOES------------------------ */
 /* ------------------------------------------------------- */

f_def : t_func '=' lit body       { $$ = new xpl::fun_def_node(LINE, $1, $3, $4); }
      | t_func body               { $$ = new xpl::fun_def_node(LINE, $1, nullptr, $2); }
      | v_func body               { $$ = new xpl::fun_def_node(LINE, $1, nullptr, $2); }
      ;

v_func  : tPUBLIC void tIDENTIFIER'(' funvars_eps ')'  { $$ = new xpl::fun_decl_node(LINE, $2, $3, XPL_GLOBAL, $5); }
        | void tIDENTIFIER'(' funvars_eps ')'          { $$ = new xpl::fun_decl_node(LINE, $1, $2, XPL_BLOCK, $4); }
        ;

t_func  : tPUBLIC type tIDENTIFIER'(' funvars_eps ')'  { $$ = new xpl::fun_decl_node(LINE, $2, $3, XPL_GLOBAL, $5); }
        | type tIDENTIFIER'(' funvars_eps ')'          { $$ = new xpl::fun_decl_node(LINE, $1, $2, XPL_BLOCK, $4); }
        ;

x_func  : tUSE type tIDENTIFIER'(' funvars_eps ')'    { $$ = new xpl::fun_decl_node(LINE, $2, $3, XPL_EXTERN, $5); }
        | tUSE void tIDENTIFIER'(' funvars_eps ')'    { $$ = new xpl::fun_decl_node(LINE, $2, $3, XPL_EXTERN, $5); } 
        ;

body  : block   { $$ = $1; }
      ;
 /* ----------------------------------------------------- */
 /* ------------------------TIPOS------------------------ */
 /* ----------------------------------------------------- */

type   : tINT                      { $$ = new basic_type(4, basic_type::TYPE_INT); }
       | tREAL                     { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
       | tSTRINGIF                 { $$ = new basic_type(4, basic_type::TYPE_STRING); }
       | '[' type ']'              { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $2;}
       ;

void   : tPROCEDURE                { $$ = new basic_type(0, basic_type::TYPE_VOID); }
       ;

 /* -------------------------------------------------------- */
 /* ------------------------LITERAIS------------------------ */
 /* -------------------------------------------------------- */

lit     : tINTEGER              { $$ = new cdk::integer_node(LINE, $1); }
        | tDOUBLE               { $$ = new cdk::double_node(LINE, $1); }
        | string %prec tBUILD   { $$ = new cdk::string_node(LINE, $1); }
        ;

string  : tSTRING               { $$ = new std::string(*$1); delete $1; }
        | string tSTRING        { $1->append(*$2); delete $2; }
        ;
 /* ------------------------------------------------------- */
 /* ------------------------LVALUES------------------------ */
 /* ------------------------------------------------------- */


lval : id                   { $$ = $1; }
     | expr '[' expr ']'    { $$ = new xpl::index_node(LINE, $3, $1); }
     ;

  id : tIDENTIFIER          { $$ = new cdk::identifier_node(LINE, $1); }
     ;
 /* ---------------------------------------------------------- */
 /* ------------------------INSTRUCOES------------------------ */
 /* ---------------------------------------------------------- */

insts : inst                { $$ = new cdk::sequence_node(LINE, $1); }
      | insts inst          { $$ = new cdk::sequence_node(LINE, $2, $1); }
      ;

inst  : expr ';'            { $$ = new xpl::evaluation_node(LINE, $1); }
      | expr '!'            { $$ = new xpl::print_node(LINE, $1); }
      | expr tPRINTLN       { $$ = new xpl::print_line_node(LINE, $1); }
      | tSTOP               { $$ = new xpl::stop_node(LINE); }
      | tNEXT               { $$ = new xpl::next_node(LINE); }
      | tRETURN             { $$ = new xpl::return_node(LINE); }
      | cond                { $$ = $1; }
      | iter                { $$ = $1; }
      | block               { $$ = $1; }
      ;

cond_elseif : tELSEIF '(' expr ')' inst   { $$ = new xpl::if_node(LINE, $3, $5); }
            | /* eps */                   { $$ = nullptr; }  
            ;

cond  : tIF '(' expr ')' inst cond_elseif tELSE inst   { $$ = new xpl::if_else_node(LINE, $3, $5, $8); } //cond
      ;

iter  : tSWEEP '+' '(' lval ':' exprs ':' exprs ')' inst                 { $$ = new xpl::sweep_up_node(LINE, $4, $6, $8, nullptr, $10); } //iteracoes
      | tSWEEP '+' '(' lval ':' exprs ':' exprs ':' exprs_eps ')' inst   { $$ = new xpl::sweep_up_node(LINE, $4, $6, $8, $10, $12); } //iteracoes
      | tSWEEP '-' '(' lval ':' exprs ':' exprs ')' inst                 { $$ = new xpl::sweep_down_node(LINE, $4, $6, $8, nullptr, $10); } //iteracoes
      | tSWEEP '-' '(' lval ':' exprs ':' exprs ':' exprs_eps ')' inst   { $$ = new xpl::sweep_down_node(LINE, $4, $6, $8, $10, $12); } //iteracoes
      | tWHILE '(' expr ')' inst                                         { $$ = new xpl::while_node(LINE, $3, $5); }
      ;

 /* ----------------------------------------------------- */
 /* ------------------------BLOCO------------------------ */
 /* ----------------------------------------------------- */

block  : '{' bvars insts '}'   { $$ = new xpl::block_node(LINE, $2, $3); }
       | '{' bvars '}'         { $$ = new xpl::block_node(LINE, $2, nullptr); }
       | '{' insts '}'         { $$ = new xpl::block_node(LINE, nullptr, $2); }
       | '{' '}'               { $$ = new xpl::block_node(LINE, nullptr, nullptr); }
       ;

bvars  : b_var ';'         { $$ = new cdk::sequence_node(LINE, $1); }
       | bvars b_var ';'   { $$ = new cdk::sequence_node(LINE, $2, $1); }
       ;

/* ---------------------------------------------------------- */
/* ------------------------EXPRESSOES------------------------ */
/* ---------------------------------------------------------- */

exprs_eps : exprs       { $$ = $1;}
          | /* eps */   { $$ = nullptr; } 
          ;

exprs : expr                           { $$ = new cdk::sequence_node(LINE, $1); }
      | exprs ',' expr                 { $$ = new cdk::sequence_node(LINE, $3, $1); }
      ;

expr : lit                             { $$ = $1; }
     | '-' expr %prec tUNARY           { $$ = new cdk::neg_node(LINE, $2); }
     | '~' expr                        { $$ = new cdk::neg_node(LINE, $2); }
     | expr '&' expr                   { $$ = new cdk::and_node(LINE, $1, $3); }
     | expr '|' expr                   { $$ = new cdk::or_node(LINE, $1, $3); }
     | expr '+' expr                   { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr                   { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr                   { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr                   { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr                   { $$ = new cdk::mod_node(LINE, $1, $3); }
     | '@'                             { $$ = new xpl::read_node(LINE); }
     | expr '<' expr                   { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr                   { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr                   { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr                   { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr                   { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr                   { $$ = new cdk::eq_node(LINE, $1, $3); }
     | '(' expr ')'                    { $$ = $2; }
     | lval                            { $$ = new cdk::rvalue_node(LINE, $1); }  //FIXME
     //| lval '=' expr                   { $$ = new cdk::assignment_node(LINE, $1, $3);
     //| lval '=' expr'['  ']'           { $$ = new xpl::memory_alloc_node(LINE, $1, $4); }
     | lval '?'                        { $$ = new xpl::pos_indication_node(LINE, $1); }
     | tIDENTIFIER '(' exprs_eps ')'   { $$ = new xpl::fun_call_node(LINE, $1, $3); }
     ;

%%
