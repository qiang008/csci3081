/*******************************************************************************
 * Name            : parser.h
 * Project         : fcal
 * Module          : parser
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Erik Van Wyk
 * Modifications by: Dan Challou, John Harwell
 *
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "include/parser.h"
#include <assert.h>
#include <stdio.h>
#include "include/ext_token.h"
#include "include/scanner.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace parser {

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Parser::~Parser() {
  if (scanner_) delete scanner_;
  scanner::ExtToken *to_delete1;
  curr_token_ = tokens_;
  while (curr_token_) {
    to_delete1 = curr_token_;
    curr_token_ = curr_token_->next();
    delete to_delete1;
  } /* while() */

  scanner::Token *curr_scanner_token = stokens_;
  scanner::Token *to_delete2;
  while (curr_scanner_token) {
    to_delete2 = curr_scanner_token;
    curr_scanner_token = curr_scanner_token->next();
    to_delete2 = NULL;
  } /* while() */
} /* Parser::~Parser() */

ParseResult Parser::Parse(const char *text) {
  assert(text != NULL);

  ParseResult pr;
  try {
        scanner_ = new scanner::Scanner();
        stokens_ = scanner_->Scan(text);
        tokens_ = tokens_->ExtendTokenList(this, stokens_);

        assert(tokens_ != NULL);
        curr_token_ = tokens_;
        pr = ParseProgram();
  }
  catch (std::string errMsg) {
          pr.ok(false);
          pr.errors(errMsg);
          pr.ast(NULL);
  }
  return pr;
} /* Parser::parse() */

/*
 * parse methods for non-terminal symbols
 * --------------------------------------
 */

// Program
ParseResult Parser::ParseProgram() {
  ParseResult pr;
  // root
  // Program ::= varName '(' ')' '{' Stmts '}'
    match(scanner::kVariableName);
    std::string name(prev_token_->lexeme());
    ast::varName *varName = new ast::varName(name);
    match(scanner::kLeftParen);
    match(scanner::kRightParen);
    match(scanner::kLeftCurly);
    ParseResult pr_stmts = parse_stmts();
    ast::stmts *s = NULL;
    if(pr_stmts.ast()){
        s=dynamic_cast<ast::stmts *>(pr_stmts.ast());
        if(!s)throw((std::string)"Bad cast of State in parseProgram");
    }
    match(scanner::kRightCurly);
    match(scanner::kEndOfFile);
    pr.ast(new ast::ProgramRoot(varName, s));
    return pr;
} /* Parser::ParseProgram() */

// MatrixDecl
// identical purpose of parse_decl, handles special matrix syntax.
ParseResult Parser::parse_matrix_decl() {
  ParseResult pr,p1,p2,p3,p4,p5,p6;
  ast::Expr *e1=NULL;
  ast::Expr *e2=NULL;
  ast::Expr *e3=NULL;
    ast::Expr *e4 =NULL;
    ast::varName *n1=NULL;
    ast::varName *n2=NULL;
  match(scanner::kMatrixKwd);
  match(scanner::kVariableName);
    std::string n= prev_token_->lexeme();
    ast::varName *name= new ast::varName(n);
    int flag =0;
  // Decl ::= 'matrix' varName '[' Expr ':' Expr ']' varName ':' varName  '='
  // Expr ';'
  if (attempt_match(scanner::kLeftSquare)) {
     p1= parse_expr(0);
    if(p1.ast()){e1=dynamic_cast<ast::Expr *>(p1.ast());}
    match(scanner::kColon);
     p2=parse_expr(0);
      if(p2.ast()){e2=dynamic_cast<ast::Expr *>(p2.ast());}
    match(scanner::kRightSquare);
    p3=parse_variable_name();
      if(p3.ast()){n1=dynamic_cast<ast::varName *>(p3.ast());}

    match(scanner::kColon);
    p4=parse_variable_name();
      if(p4.ast()){n2=dynamic_cast<ast::varName *>(p4.ast());}

    match(scanner::kAssign);
      p5=parse_expr(0);
      if(p5.ast()){e3=dynamic_cast<ast::Expr *>(p5.ast());}
      flag=1;
  } else if (attempt_match(scanner::kAssign)) {
    // Decl ::= 'matrix' varName '=' Expr ';'
    p6=parse_expr(0);
      if(p6.ast()){e4=dynamic_cast<ast::Expr *>(p6.ast());}
      flag=2;
  } else {
    throw((std::string) "Bad Syntax of Matrix Decl in in parseMatrixDecl");
  }
  match(scanner::kSemiColon);
    if(flag==1){
        pr.ast(new ast::matrixDecl(name,e1,e2,n1,n2,e3));
    }
    else if(flag==2){
        pr.ast(new ast::matrixSecondDecl(name, e4));
    }

  return pr;
}
// standardDecl
// Decl ::= integerKwd varName | floatKwd varName | stringKwd varName
ParseResult Parser::parse_standard_decl() {
  ParseResult pr;
  if (attempt_match(scanner::kIntKwd)) {
      // Type ::= intKwd
  } else if (attempt_match(scanner::kFloatKwd)) {
      // Type ::= floatKwd
  } else if (attempt_match(scanner::kStringKwd)) {
      // Type ::= stringKwd
  } else if (attempt_match(scanner::kBoolKwd)) {
      // Type ::= boolKwd
  }
  std::string t=prev_token_->lexeme();
  match(scanner::kVariableName);
  std::string name(prev_token_->lexeme());
  match(scanner::kSemiColon);
  ast::varName *varName = new ast::varName(name);
  pr.ast(new ast::StandardDecl(varName,t));
  //TO do other situation
  return pr;
}

// Decl
ParseResult Parser::parse_decl() {
  ParseResult pr;
  // Decl :: matrix variableName ....
  if (next_is(scanner::kMatrixKwd)) {
    pr = parse_matrix_decl();
  } else {
    // Decl ::= Type variableName semiColon
    pr = parse_standard_decl();
  }
  return pr;
}

// Stmts
ParseResult Parser::parse_stmts() {
  ParseResult pr;
  if (!next_is(scanner::kRightCurly) && !next_is(scanner::kInKwd)) {
    // Stmts ::= Stmt Stmts
    ParseResult pr_stmt = parse_stmt();
      ast::stmt *s1=NULL;
    if(pr_stmt.ast()){
          s1=dynamic_cast<ast::stmt *>(pr_stmt.ast());
          if(!s1)throw((std::string)"Bad cast of State in parseStmt");
    }
    ParseResult pr_stmts = parse_stmts();
    ast::stmts *s2=NULL;
    if(pr_stmts.ast()){
          s2=dynamic_cast<ast::stmts *>(pr_stmts.ast());
          if(!s2)throw((std::string)"Bad cast of State in parseProgram");
    }
    pr.ast(new ast::StmtsSeq(s1,s2));
    //pr_stmts.ast(new ast::stmts);
  } else {
    // Stmts ::=
    // nothing to match.k
    pr.ast(new ast::EmptyStmts());
  }
  return pr;
}

// Stmt
ParseResult Parser::parse_stmt() {
  ParseResult pr;

  // Stmt ::= Decl
  if (next_is(scanner::kIntKwd) || next_is(scanner::kFloatKwd) ||
      next_is(scanner::kMatrixKwd) || next_is(scanner::kStringKwd) ||
      next_is(scanner::kBoolKwd)) {
      ParseResult p_decl= parse_decl();
      ast::Decl *d = NULL;
      if(p_decl.ast()){
          d=dynamic_cast<ast::Decl *>(p_decl.ast());
          if(!d)throw((std::string)"Bad cast of stmt Decl in parseProgram");
      }
     pr.ast(new ast::stmtDecl(d));
  } else if (attempt_match(scanner::kLeftCurly)) {
    // Stmt ::= '{' Stmts '}'
    ParseResult p_if = parse_stmts();
      ast::stmts *s=NULL;
      if(p_if.ast()){s=dynamic_cast<ast::stmts *>(p_if.ast());}
    match(scanner::kRightCurly);
      pr.ast(new ast::stmtstmts(s));
  } else if (attempt_match(scanner::kIfKwd)) {
    // Stmt ::= 'if' '(' Expr ')' Stmt
    // Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
    match(scanner::kLeftParen);
      ast::Expr *e1=NULL;
    ParseResult p_expr = parse_expr(0);
      if(p_expr.ast()){ e1=dynamic_cast<ast::Expr *>(p_expr.ast());}
    match(scanner::kRightParen);
      ast::stmt *s1=NULL;
    ParseResult st = parse_stmt();
      if(st.ast()){s1=dynamic_cast<ast::stmt *>(st.ast());}
      ast::stmt *s2=NULL;
    if (attempt_match(scanner::kElseKwd)) {
      ParseResult st2= parse_stmt();
        if(st2.ast()){s2=dynamic_cast<ast::stmt *>(st2.ast());}
        pr.ast(new ast::IfElseStmt(e1,s1,s2));
    }
      pr.ast(new ast::ifstmt(e1,s1));
  } else if (attempt_match(scanner::kVariableName)) {
    /*
     * Stmt ::= varName '=' Expr ';'  | varName '[' Expr ':' Expr ']'
     * '=' Expr ';'
     */
      int flag = 0;
      std::string name(prev_token_->lexeme());
      ast::varName *n = new ast::varName(name);
      ParseResult p1;
      ParseResult p2;
      ast::Expr *exp1=NULL;
      ast::Expr *exp2=NULL;
    if (attempt_match(scanner::kLeftSquare)) {
      flag =3;
      p1=parse_expr(0);
        if(p1.ast()){ exp1=dynamic_cast<ast::Expr *>(p1.ast());}
      match(scanner::kColon);
      p2=parse_expr(0);
        if(p2.ast()){ exp2=dynamic_cast<ast::Expr *>(p2.ast());}
      match(scanner::kRightSquare);
    }
    match(scanner::kAssign);
    ParseResult p_expr = parse_expr(0);
    ast::Expr *exp3=NULL;
      if(p_expr.ast())
      {
          exp3=dynamic_cast<ast::Expr *>(p_expr.ast());
          if(!exp3)throw((std::string)"Bad cast of Expr in parseProgram");
          flag=4;
      }
      
    match(scanner::kSemiColon);
      if(flag==4){
          pr.ast(new ast::assignstmtFirst(n, exp3));
      }
      else if(flag == 3){
          pr.ast(new ast::assignstmtSecond(n,exp1,exp2,exp3));
      }

  } else if (attempt_match(scanner::kPrintKwd)) {
    // Stmt ::= 'print' '(' Expr ')' ';'
    match(scanner::kLeftParen);
    ParseResult exp = parse_expr(0);
      ast::Expr *e = NULL;
      if(exp.ast()){e=dynamic_cast<ast::Expr *>(exp.ast());}
    match(scanner::kRightParen);
    match(scanner::kSemiColon);
      pr.ast(new ast::printstmt(e));
  } else if (attempt_match(scanner::kRepeatKwd)) {
    // Stmt ::= 'repeat' '(' varName '=' Expr 'to' Expr ')' Stmt
    match(scanner::kLeftParen);
    match(scanner::kVariableName);
      std::string vname = prev_token_->lexeme();
      ast::varName *n= new ast::varName(vname);
    match(scanner::kAssign);
    ParseResult e1= parse_expr(0);
      ast::Expr *expr1=NULL;
      if(e1.ast()){expr1=dynamic_cast<ast::Expr *>(e1.ast());}
    match(scanner::kToKwd);
    ParseResult e2= parse_expr(0);
      ast::Expr *expr2=NULL;
      if(e2.ast()){expr2=dynamic_cast<ast::Expr *>(e2.ast());}
    match(scanner::kRightParen);
    ParseResult s= parse_stmt();
      ast::stmt *st = NULL;
      if(s.ast()){st=dynamic_cast<ast::stmt *>(s.ast());}
      pr.ast(new ast::repeatstmt(n,expr1,expr2,st));
  } else if (attempt_match(scanner::kWhileKwd)) {
    // Stmt ::= 'while' '(' Expr ')' Stmt
    match(scanner::kLeftParen);
      ParseResult e1= parse_expr(0);
      ast::Expr *expr1=NULL;
      if(e1.ast()){expr1=dynamic_cast<ast::Expr *>(e1.ast());}
      match(scanner::kRightParen);
      ParseResult s= parse_stmt();
      ast::stmt *st = NULL;
      if(s.ast()){st=dynamic_cast<ast::stmt *>(s.ast());}
      pr.ast(new ast::whileStmt(expr1,st));
  } else if (attempt_match(scanner::kSemiColon)) {
    // Stmt ::= ';
    // parsed a skip
  } else {
    throw(make_error_msg(curr_token_->terminal()) +
          " while parsing a statement");
  }
  // Stmt ::= variableName assign Expr semiColon
  return pr;
}

// Expr
ParseResult Parser::parse_expr(int rbp) {
  /* Examine current token, without consuming it, to call its
     associated parse methods.  The ExtToken objects have 'nud' and
     'led' methods that are dispatchers that call the appropriate
     parse methods.*/
  ParseResult left = curr_token_->nud();

  while (rbp < curr_token_->lbp()) {
    left = curr_token_->led(left);
  }

  return left;
}

/*
 * parse methods for Expr productions
 * ----------------------------------
 */

// Expr ::= trueKwd
ParseResult Parser::parse_true_kwd() {
  ParseResult pr;
  match(scanner::kTrueKwd);
    std::string s= prev_token_->lexeme();
    pr.ast(new ast::TFExpr(s));
  return pr;
}

// Expr ::= trueKwd
ParseResult Parser::parse_false_kwd() {
  ParseResult pr;
  match(scanner::kFalseKwd);
    std::string s= prev_token_->lexeme();
    pr.ast(new ast::TFExpr(s));
  return pr;
}

// Expr ::= intConst
ParseResult Parser::parse_int_const() {
  ParseResult pr;
  match(scanner::kIntConst);
  std::string s= prev_token_->lexeme();
  pr.ast(new ast::AllConstExpr(s));
  return pr;
}

// Expr ::= floatConst
ParseResult Parser::parse_float_const() {
  ParseResult pr;
    match(scanner::kFloatConst);
    std::string s= prev_token_->lexeme();
    pr.ast(new ast::AllConstExpr(s));
  return pr;
}

// Expr ::= stringConst
ParseResult Parser::parse_string_const() {
  ParseResult pr;
  match(scanner::kStringConst);
  std::string s= prev_token_->lexeme();
  pr.ast(new ast::AllConstExpr(s));
  return pr;
}

// Expr ::= variableName .....
ParseResult Parser::parse_variable_name() {
  ParseResult pr;
  match(scanner::kVariableName);
  std::string name(prev_token_->lexeme());
    ast::varName *n= new ast::varName(name);
  if (attempt_match(scanner::kLeftSquare)) {
      ast::Expr *e1=NULL;
    ParseResult p1= parse_expr(0);
      if(p1.ast()){e1=dynamic_cast<ast::Expr *>(p1.ast());}
    match(scanner::kColon);
      ast::Expr *e2=NULL;
    ParseResult p2= parse_expr(0);
      if(p2.ast()){e2=dynamic_cast<ast::Expr *>(p2.ast());}
    match(scanner::kRightSquare);
      pr.ast(new ast::matrixREF(n,e1,e2));
  }  else if (attempt_match(scanner::kLeftParen)) {
    // Expr ::= varableName '(' Expr ')'
      ast::Expr *e = NULL;
    ParseResult p= parse_expr(0);
       if(p.ast()){e=dynamic_cast<ast::Expr *>(p.ast());}
    match(scanner::kRightParen);
      pr.ast(new ast::nestedExpr(n,e));
  } else {
    // variable
    pr.ast(n);
  }
  return pr;
}

// Expr ::= leftParen Expr rightParen
ParseResult Parser::parse_nested_expr() {
  ParseResult pr;
  match(scanner::kLeftParen);
  ast::Expr *e1=NULL;
  ParseResult p1= parse_expr(0);
  if(p1.ast()){e1=dynamic_cast<ast::Expr *>(p1.ast());}
  match(scanner::kRightParen);
  pr.ast(new ast::bracketExpr(e1));
  return pr;
}

// Expr ::= 'if' Expr 'then' Expr 'else' Expr
ParseResult Parser::parse_if_expr() {
  ParseResult pr;
    ast::Expr *e1=NULL;
    ast::Expr *e2=NULL;
    ast::Expr *e3=NULL;
  match(scanner::kIfKwd);
  ParseResult p1= parse_expr(0);
    if(p1.ast()){e1=dynamic_cast<ast::Expr *>(p1.ast());}
  match(scanner::kThenKwd);
  ParseResult p2= parse_expr(0);
    if(p2.ast()){e2=dynamic_cast<ast::Expr *>(p2.ast());}
  match(scanner::kElseKwd);
  ParseResult p3= parse_expr(0);
    if(p3.ast()){e3=dynamic_cast<ast::Expr *>(p3.ast());}
    pr.ast(new ast::ifthenExpr(e1,e2,e3));
  return pr;
}

// Expr ::= 'let' Stmts 'in' Expr 'end'
ParseResult Parser::parse_let_expr() {
  ParseResult pr;
  match(scanner::kLetKwd);
  ParseResult p = parse_stmts();
    ast::stmts *s=NULL;
    if(p.ast()){s=dynamic_cast<ast::stmts *>(p.ast());}
  match(scanner::kInKwd);
  ParseResult p2= parse_expr(0);
    ast::Expr *e=NULL;
    if(p2.ast()){e=dynamic_cast<ast::Expr *>(p2.ast());}
  match(scanner::kEndKwd);
    pr.ast(new ast::LetExpr(s,e));
  return pr;
}

// Expr ::= '!' Expr
ParseResult Parser::parse_not_expr() {
  ParseResult pr;
  match(scanner::kNotOp);
    ast::Expr *e=NULL;
  ParseResult p= parse_expr(0);
    if(p.ast()){e=dynamic_cast<ast::Expr *>(p.ast());}
    pr.ast(new ast::notExpr(e));
  return pr;
}
// Expr ::= Expr plusSign Expr
ParseResult Parser::parse_addition(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
  ast::Expr *left = NULL;
  if(prLeft.ast()){left=dynamic_cast<ast::Expr *>(prLeft.ast());}
  match(scanner::kPlusSign);
  std::string op=prev_token_->lexeme();
  ParseResult right=parse_expr(prev_token_->lbp());
    ast::Expr *r=NULL;
    if(right.ast()){r=dynamic_cast<ast::Expr *>(right.ast());}
    pr.ast(new ast::relationExpr(op,left,r));
  return pr;
}

// Expr ::= Expr star Expr
ParseResult Parser::parse_multiplication(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
    ast::Expr *left = NULL;
    if(prLeft.ast()){left=dynamic_cast<ast::Expr *>(prLeft.ast());}
    match(scanner::kStar);
    std::string op=prev_token_->lexeme();
    ParseResult right=parse_expr(prev_token_->lbp());
    ast::Expr *r=NULL;
    if(right.ast()){r=dynamic_cast<ast::Expr *>(right.ast());}
    pr.ast(new ast::relationExpr(op,left,r));
  return pr;
}

// Expr ::= Expr dash Expr
ParseResult Parser::parse_subtraction(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
    ast::Expr *left = NULL;
    if(prLeft.ast()){left=dynamic_cast<ast::Expr *>(prLeft.ast());}
  match(scanner::kDash);
    std::string op = prev_token_->lexeme();
    ParseResult prRight = parse_expr(prev_token_->lbp());
    ast::Expr *right=NULL;
    if(prRight.ast()){right=dynamic_cast<ast::Expr *>(prRight.ast());}
    pr.ast(new ast::relationExpr(op,left,right));
    return pr;
}

// Expr ::= Expr forwardSlash Expr
ParseResult Parser::parse_division(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
    ast::Expr *left = NULL;
    if(prLeft.ast()){left=dynamic_cast<ast::Expr *>(prLeft.ast());}
    match(scanner::kForwardSlash);
    std::string op = prev_token_->lexeme();
    ParseResult prRight = parse_expr(prev_token_->lbp());
    ast::Expr *right=NULL;
    if(prRight.ast()){right=dynamic_cast<ast::Expr *>(prRight.ast());}
    pr.ast(new ast::relationExpr(op,left,right));
  return pr;
}

// Expr ::= Expr equalEquals Expr
// Expr ::= Expr lessThanEquals Expr
// Expr ::= Expr greaterThanEquals Expr
// Expr ::= Expr notEquals Expr
// Expr ::= Expr leftAngle Expr
// Expr ::= Expr rightAngle Expr
/* Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
*/
ParseResult Parser::parse_relational_expr(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
    ast::Expr *left = NULL;
    if(prLeft.ast()){left=dynamic_cast<ast::Expr *>(prLeft.ast());}
  next_token();
  // just advance token, since examining it in parse_expr caused
  // this method being called.
  std::string op = prev_token_->lexeme();

  ParseResult prRight = parse_expr(prev_token_->lbp());
  ast::Expr *right=NULL;
  if(prRight.ast()){right=dynamic_cast<ast::Expr *>(prRight.ast());}
    pr.ast(new ast::relationExpr(op,left,right));
  return pr;
}

// Helper function used by the parser.

void Parser::match(const scanner::TokenType &tt) {
  if (!attempt_match(tt)) {
    throw(make_error_msg_expected(tt));
  }
}

bool Parser::attempt_match(const scanner::TokenType &tt) {
  if (curr_token_->terminal() == tt) {
    next_token();
    return true;
  }
  return false;
}

bool Parser::next_is(const scanner::TokenType &tt) {
  return curr_token_->terminal() == tt;
}

void Parser::next_token() {
  if (curr_token_ == NULL) {
    throw(std::string(
        "Internal Error: should not call nextToken in unitialized state"));
  } else if (curr_token_->terminal() == scanner::kEndOfFile &&
           curr_token_->next() == NULL) {
    prev_token_ = curr_token_;
  } else if (curr_token_->terminal() != scanner::kEndOfFile &&
             curr_token_->next() == NULL) {
    throw(make_error_msg("Error: tokens end with endOfFile"));
  } else {
    prev_token_ = curr_token_;
    curr_token_ = curr_token_->next();
  }
}

std::string Parser::terminal_description(const scanner::TokenType &terminal) {
  scanner::Token *dummy_token = new scanner::Token(terminal, "", NULL);
  scanner::ExtToken *dummy_ext_token = new scanner::ExtToken(this, dummy_token);
  dummy_ext_token->ExtendToken(this, dummy_token);
  std::string s = dummy_ext_token->description();
  delete dummy_token;
  delete dummy_ext_token;
  return s;
}

std::string Parser::make_error_msg_expected(
    const scanner::TokenType &terminal) {
  std::string s = (std::string) "Expected " + terminal_description(terminal) +
                  " but found " + curr_token_->description();
  return s;
}

std::string Parser::make_error_msg(const scanner::TokenType &terminal) {
  std::string s = "Unexpected symbol " + terminal_description(terminal);
  return s;
}

std::string Parser::make_error_msg(const char *msg) { return msg; }

} /* namespace parser */
} /* namespace fcal */
