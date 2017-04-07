//
//  ast.cpp
//  
//
//  Created by fudan on 17/4/1.
//
//

#include <stdio.h>
#include <iostream>
#include <string>
#include "include/ast.h"
namespace fcal {
namespace ast {
    
    //varName
    varName::varName(std::string name){
        name_=name;
    }
    std::string varName::unparse(){
        //std::cout<<"unparsing varname...."<<std::endl;
        return name_;
    }
    
    //programRoot
    ProgramRoot::ProgramRoot(varName *name, stmts* s)
    {
        v_=name;
        s_=s;
    }
    
    std::string ProgramRoot::unparse(){
        //std::cout<<"unparsing Program Root...."<<std::endl;
        //std::cout<<v_->unparse()+"("+")"+"{"+s_->unparse()+"}"<<std::endl;
        return v_->unparse()+" ( "+" )"+" { "+s_->unparse()+" } ";
    }
    //stmtDecl
    stmtDecl::stmtDecl(Decl *d){
        d_=d;
    }
    std::string stmtDecl::unparse(){
        //std::cout<<"unparsing stmt Decl..."<<std::endl;
        //std::cout<<d_->unparse()<<std::endl;
        return d_->unparse();
    }
    //stmt stmts
    stmtstmts::stmtstmts(ast::stmts *s){
        s_=s;
    }
    std::string stmtstmts::unparse(){
        //std::cout<<"unparsing stmt stmts....."<<std::endl;
        return "{ "+s_->unparse()+" }";
    }
    
    //if stmt
    ifstmt::ifstmt(ast::Expr *exp, ast::stmt *s){
        exp_=exp;
        s_=s;
    }
    std::string ifstmt::unparse(){
        //std::cout<<"unparsing if stmt"<<std::endl;
        return "if ( " +exp_->unparse()+ " ) "+s_->unparse();
    }

    //if elase stmt
    IfElseStmt::IfElseStmt(ast::Expr *exp, ast::stmt *s1, ast::stmt *s2){
        exp_=exp;
        s1_=s1;
        s2_=s2;
    }
    std::string IfElseStmt::unparse(){
        return "if ( "+exp_->unparse()+" ) "+s1_->unparse()+" else "+ s2_->unparse();
    }
    // assign first
    assignstmtFirst::assignstmtFirst(ast::varName *name, ast::Expr *e1){
        name_=name;
        e1_=e1;
    }
    std::string assignstmtFirst::unparse(){
        //std::cout<<"unparsing first assign..."<<std::endl;
        return name_->unparse()+" = "+ e1_->unparse()+";";
    }
    
    //assign second
    assignstmtSecond::assignstmtSecond(ast::varName *name, ast::Expr *e1, ast::Expr *e2, ast::Expr *e3){
        name_ = name;
        e1_ = e1;
        e2_ = e2;
        e3_ = e3;
    }
    std::string assignstmtSecond::unparse(){
        //std::cout<<"unparsing second assign..."<<std::endl;
        return name_->unparse()+" {"+e1_->unparse()+" : "+e2_->unparse()+" ] = "+e3_->unparse()+";" ;
    }
    
    //print
    printstmt::printstmt(ast::Expr *e)
    {
        e_ = e;
    }
    std::string printstmt::unparse(){
        //std::cout<<"unparsing print stmt..."<<std::endl;
        return "print (" + e_->unparse()+" );";
    }
    //repeat stmt
    repeatstmt::repeatstmt(ast::varName *name, ast::Expr *e1, ast::Expr *e2, ast::stmt *s){
        name_=name;
        e1_=e1;
        e2_=e2;
        s_=s;
    }
    std::string repeatstmt::unparse(){
        //std::cout<<"unparsing repeat stmt..."<<std::endl;
        return "repeat ("+name_->unparse()+" = "+e1_->unparse()+" to "+e2_->unparse()+") "+s_->unparse();
    }
    //while stmt
    whileStmt::whileStmt(ast::Expr *e, ast::stmt *s){
        e_=e;
        s_=s;
    }
    std::string whileStmt::unparse(){
        //std::cout<<"unparsing while stmt..."<<std::endl;
        return "while ("+e_->unparse()+") "+s_->unparse();
    }
    //all consts
    AllConstExpr::AllConstExpr(std::string s){
        const_=s;
    }
    std::string AllConstExpr::unparse(){
        //std::cout<<"unparsing const ..."<<std::endl;
        return const_;
    }
    //relation Exp
    relationExpr::relationExpr(std::string op, ast::Expr *s1, ast::Expr *s2){
        op_=op;
        s1_=s1;
        s2_=s2;
    }
    std::string relationExpr::unparse(){
        //std::cout<<"unparsing relation expr..."<<std::endl;
        return s1_->unparse()+" "+op_+" "+s2_->unparse();
    }
    //standard decl
    StandardDecl::StandardDecl(varName *name, std::string type){
        Name_=name;
        type_=type;
    }
    std::string StandardDecl::unparse(){
        //std::cout<<"unparsing Const kwd Decl..."<<std::endl;
        //std::cout<<"int"+intName_->unparse()+";"<<std::endl;
        return type_+" "+Name_->unparse()+";";
    }
    //matrix Decl
    matrixDecl::matrixDecl(ast::varName *name, ast::Expr *e1, ast::Expr *e2,
                           ast::varName *name2, ast::varName *name3,ast::Expr *e3){
        name_=name;
        e1_=e1;
        e2_=e2;
        name2_=name2;
        name3_=name3;
        e3_=e3;
    }
    std::string matrixDecl::unparse(){
        //std::cout<<"unparsing matrix1..."<<std::endl;
        return "matrix "+name_->unparse()+" [ "+e1_->unparse()+":"+e2_->unparse()+" ] "+ name2_->unparse()+":"+name3_->unparse()+"="+e3_->unparse()+";";
    }
    
    //second metrix
    matrixSecondDecl::matrixSecondDecl(ast::varName *name, ast::Expr *e){
        name_=name;
        e_=e;
    }
    std::string matrixSecondDecl::unparse(){
        //std::cout<<"unparsing matrix 2..."<<std::endl;
        return "matrix "+name_->unparse()+"="+e_->unparse()+";";
    }
    
    //true false
    TFExpr::TFExpr(std::string s){
        s_=s;
    }
    std::string TFExpr::unparse(){
        //std::cout<<"unparsing True or False..."<<std::endl;
        return s_;
    }
    
    //matrix REF
    matrixREF::matrixREF(ast::varName *name, ast::Expr *e1, ast::Expr *e2){
        name_=name;
        e1_=e1;
        e2_=e2;
    }
    std::string matrixREF::unparse(){
        //std::cout<<"unparsing matrix REF..."<<std::endl;
        return name_->unparse()+"[ "+e1_->unparse()+":"+e2_->unparse()+"]";
    }
    
    //nested
    nestedExpr::nestedExpr(ast::varName *name, ast::Expr *e){
        name_=name;
        e_=e;
    }
    std::string nestedExpr::unparse(){
        //std::cout<<"unparsing nested function call..."<<std::endl;
        return name_->unparse()+" ("+e_->unparse()+")";
    }
    //bracket
    bracketExpr::bracketExpr(ast::Expr *e){
        e_=e;
    }
    std::string bracketExpr::unparse(){
        //std::cout<<"unparsing bracket expr..."<<std::endl;
        return "("+e_->unparse()+")";
    }
    //let expr
    LetExpr::LetExpr(ast::stmts *ss, ast::Expr *e){
        ss_=ss;
        e_=e;
    }
    std::string LetExpr::unparse(){
        //std::cout<<"unparsing let expr..."<<std::endl;
        return "let "+ss_->unparse()+" in "+e_->unparse()+" end";
    }
    
    //if then expr
    ifthenExpr::ifthenExpr(ast::Expr *e1, ast::Expr *e2, ast::Expr *e3){
        e1_=e1;
        e2_=e2;
        e3_=e3;
    }
    std::string ifthenExpr::unparse(){
        //std::cout<<"unparsing if then expr..."<<std::endl;
        return "if "+e1_->unparse()+" then "+e2_->unparse()+" else "+e3_->unparse();
    }
    
    //not
    notExpr::notExpr(ast::Expr *e){
        e_=e;
    }
    std::string notExpr::unparse(){
        //std::cout<<"unparsing not expr..."<<std::endl;
        return "!"+e_->unparse();
    }
    
    //stmtsseq
    StmtsSeq::StmtsSeq(stmt *stmt, stmts *stmts){
        stmt_=stmt;
        stmts_=stmts;
    }
    std::string StmtsSeq::unparse(){
         //std::cout<<"unparsing stmts seq...."<<std::endl;
         //std::cout<<"stmts...."+stmt_->unparse()+stmts_->unparse()<<std::endl;
         return stmt_->unparse()+" "+stmts_->unparse();
    }
    //Empty
    EmptyStmts::EmptyStmts(){}
    std::string EmptyStmts::unparse(){
       //std::cout<<"unparsing empty stmts...."<<std::endl;
       return "";
    }
}
}
