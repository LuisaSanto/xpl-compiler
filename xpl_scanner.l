%option c++ yyclass="xpl_scanner" outfile="xpl_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
/* $Id: xpl_scanner.l,v 1.4 2017/05/19 23:15:00 ist179758 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "xpl_scanner.h"
#include "xpl_parser.tab.h"

// don't change this
#define yyerror LexerError 
%}
  
HEXA	[0123456789a-fA-F] 

%x X_STRING X_COMMENT X_SLASH X_SLASH_ZERO
%%

"//".*$ 					; /*ignore one line comments*/

<INITIAL,X_COMMENT>"/*"		yy_push_state(X_COMMENT);
<X_COMMENT>"*/"				yy_pop_state();
<X_COMMENT>.|\n 			/*ignore comments*/

">="						return tGE; 
"<="						return tLE; 
"=="						return tEQ; 
"!="						return tNE; 

"sweep"						return tSWEEP;
"next"						return tNEXT;
"stop"						return tSTOP;
"if"						return tIF;
"elsif"						return tELSEIF;
"else"						return tELSE;
"!!"						return tPRINTLN; 
"public"					return tPUBLIC;
"use"						return tUSE;
"return"					return tRETURN;
"null"						return tNULL;
"int"						return tINT;
"real"						return tREAL;
"string"					return tSTRINGIF;
"procedure"					return tPROCEDURE;
"apply"						return tAPPLY;
"and"						return tAND;

"while"						return tWHILE;


[A-Za-z][A-Za-z0-9_]*		yylval.s = new std::string(yytext); return tIDENTIFIER;

<INITIAL>\"					yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\\				yy_push_state(X_SLASH);
<X_STRING>\"				yy_pop_state(); return tSTRING;
<X_STRING>\\\"				*yylval.s += yytext + 1;
<X_STRING>.					*yylval.s += yytext;

<X_SLASH>"0"				yy_push_state(X_SLASH_ZERO);
<X_SLASH>"n"				*yylval.s += char('\n'); yy_pop_state();
<X_SLASH>"r"				*yylval.s += char('\r'); yy_pop_state();
<X_SLASH>"t"				*yylval.s += char('\t'); yy_pop_state();
<X_SLASH>{HEXA}{HEXA}?		*yylval.s += strtol(yytext, NULL, 16); yy_pop_state();

<X_SLASH_ZERO>\"			yy_pop_state(); yy_pop_state(); return tSTRING;
<X_SLASH_ZERO>.				;

"0"|([1-9][0-9]*)			yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;
"0"[xX]{HEXA}+				yylval.i = strtol(yytext, nullptr, 16); return tINTEGER;

[.][0-9]+([eE]-?[1-9][0-9]*)?					yylval.d = strtod("0" + *yytext, nullptr); return tDOUBLE;
("0"|[1-9]*)([.][0-9]+)?([eE]-?[1-9][0-9]*)?	yylval.d = strtod(yytext, nullptr); return tDOUBLE; 

[-?()<>=+*/;:,{}!~&|]|"["|"]" 	return *yytext;

[ \t\n\r]+						; /* ignore whitespace */

.								yyerror("Unknown character");

%%
// Very, very dirty hack: flex is a mess generating C++ scanners.
int xpl_scanner::yywrap() { return 1; }
