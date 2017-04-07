/*******************************************************************************
 * Name            : regex.h
 * Project         : fcal
 * Module          : ast
 * Description     : Header file for AST
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Eric Van Wyk
 * Modifications by: John Harwell
 ******************************************************************************/

#ifndef INCLUDE_AST_H_
#define INCLUDE_AST_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <iostream>
#include <string>
#include "include/scanner.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace ast {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
class Node {
 public:
  virtual std::string unparse(void) { return " This should be pure virtual "; }
  virtual std::string CppCode(void) { return " This should be pure virtual"; }
  virtual ~Node(void) {}
};
    
//Decl
class Decl:public Node{};
    
//Expr
class Expr:public Node{};
    
//stmts
class stmts:public Node{};

//stmt
class stmt:public Node{};


//varName
class varName:public Expr{
private:
    std::string name_;
public:
    varName(std::string name);
    std::string unparse();
};
//stmts sequence
class StmtsSeq : public stmts {
public:
    StmtsSeq(stmt *stmt, stmts *stmts);
    std::string unparse();
private:
    stmt* stmt_;
    stmts* stmts_;
   };
//empty stmts
class EmptyStmts:public stmts{
    public:
    EmptyStmts();
    std::string unparse();
};

//stmtDecl
class stmtDecl:public stmt{
private:
    ast::Decl *d_;
public:
    stmtDecl(ast::Decl *d);
    std::string unparse();
};
//stmtstmts
class stmtstmts:public stmt{
private:
        ast::stmts *s_;
public:
        stmtstmts(ast::stmts *s);
        std::string unparse();
};

//ifstmt
class ifstmt:public stmt{
private:
    ast::Expr *exp_;
    ast::stmt *s_;
public:
    ifstmt(ast::Expr *exp, ast::stmt *s);
    std::string unparse();
};

//ifelsestmt
class IfElseStmt:public stmt{
private:
    ast::Expr *exp_;
    ast::stmt *s1_;
    ast::stmt *s2_;
public:
    IfElseStmt(ast::Expr *exp, ast::stmt *s1, ast::stmt *s2);
    std::string unparse();
};
//assignstmt first
class assignstmtFirst:public stmt{
private:
        ast::varName *name_;
        ast::Expr *e1_;
public:
        assignstmtFirst(ast::varName *name, ast::Expr *e1);
        std::string unparse();
};
//assignstmt second
class assignstmtSecond:public stmt{
private:
    ast::varName *name_;
    ast::Expr *e1_;
    ast::Expr *e2_;
    ast::Expr *e3_;
public:
    assignstmtSecond(ast::varName *name, ast::Expr *e1, ast::Expr *e2, ast::Expr *e3);
    std::string unparse();
};
//print
class printstmt:public stmt{
private:
    ast::Expr *e_;
public:
    printstmt(ast::Expr *e);
    std::string unparse();
};

//repeat
    class repeatstmt:public stmt{
    private:
        ast::varName *name_;
        ast::Expr *e1_;
        ast::Expr *e2_;
        ast::stmt *s_;
    public:
        repeatstmt(ast::varName *name, ast::Expr *e1, ast::Expr *e2, ast::stmt *s);
        std::string unparse();
    };
    //while
    class whileStmt:public stmt{
    private:
        ast::Expr *e_;
        ast::stmt *s_;
    public:
        whileStmt(ast::Expr *e, ast::stmt *s);
        std::string unparse();
    };
//standard Decl
class StandardDecl:public Decl{
private:
    varName *Name_;
    std::string type_;
public:
    StandardDecl(varName *name, std::string type);
    std::string unparse();
};

    //first matrix
    class matrixDecl:public Decl{
    private:
        ast::varName *name_;
        ast::Expr *e1_;
        ast::Expr *e2_;
        ast::Expr *e3_;
        ast::varName *name2_;
        ast::varName *name3_;
    public:
        matrixDecl(ast::varName *name, ast::Expr *e1, ast::Expr *e2,
                   ast::varName *name2, ast::varName *name3,ast::Expr *e3);
        std::string unparse();
    };
    //second matrix
    class matrixSecondDecl:public Decl{
    private:
        ast::varName *name_;
        ast::Expr *e_;
    public:
        matrixSecondDecl(ast::varName *name, ast::Expr *e);
        std::string unparse();
    };

//ProgramRoot
class ProgramRoot:public Node{
    private:
        varName *v_;
        stmts* s_;
    public:
        ProgramRoot(varName*, stmts*);
        std::string unparse();
};
//all const const
class AllConstExpr : public Expr{
private:
    std::string const_;
public:
    AllConstExpr(std::string s);
    std::string unparse();
};

//True or false
    class TFExpr: public Expr{
    private:
        std::string s_;
    public:
        TFExpr(std::string s);
        std::string unparse();
    };
    
    //relation Expr
    class relationExpr:public Expr{
    private:
        std::string op_;
        ast::Expr *s1_;
        ast::Expr *s2_;
    public:
        relationExpr(std::string op, ast::Expr *s1, ast::Expr *s2);
        std::string unparse();
    };
    
    //matrix REF
    class matrixREF:public Expr{
    private:
        ast::varName *name_;
        ast::Expr *e1_;
        ast::Expr *e2_;
    public:
        matrixREF(ast::varName *name, ast::Expr *e1, ast::Expr *e2);
        std::string unparse();
    };
    //nested
    class nestedExpr:public Expr{
    private:
        ast::varName *name_;
        ast::Expr *e_;
    public:
        nestedExpr(ast::varName *name, ast::Expr *e);
        std::string unparse();
    };
    //bracket expr
    class bracketExpr:public Expr{
    private:
        ast::Expr *e_;
    public:
        bracketExpr(ast::Expr *e);
        std::string unparse();
    };
    //Let Expr
    class LetExpr:public Expr{
    private:
        ast::stmts *ss_;
        ast::Expr *e_;
    public:
        LetExpr(ast::stmts *ss, ast::Expr *e);
        std::string unparse();
    };
    
    //ifExpr
    class ifthenExpr:public Expr{
    private:
        ast::Expr *e1_;
        ast::Expr *e2_;
        ast::Expr *e3_;
    public:
        ifthenExpr(ast::Expr *e1, ast::Expr *e2, ast::Expr *e3);
        std::string unparse();
    };
    //not
    class notExpr:public Expr{
    private:
        ast::Expr *e_;
    public:
        notExpr(ast::Expr *e);
        std::string unparse();
    };
/*
// VarName
class VarName {
public:
  explicit VarName(std::string lexeme) : lexeme_(lexeme) {}
  std::string unparse();
  std::string cpp_code();

private:
VarName() : lexeme_((std::string) "") {}
  VarName(const VarName &) {};
  std::string lexeme_;
};

//Stmts
class Stmts: public Node {};

//Stmt
class Stmt: public Node {};

//Stmts Concrete Classes
 class StmtsSeq : public Stmts {
 public:
   explicit StmtsSeq(Stmt *stmt, Stmts *stmts) : stmt_(stmt), stmts_(stmts) {}
   std::string unparse();

 private:
 StmtsSeq() :stmt_(NULL), stmts_(NULL) {}
   //MAY NEED MORE HERE
 };
*/
} /* namespace ast */
} /* namespace fcal */

#endif /* INCLUDE_AST_H_ */
