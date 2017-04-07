//
//  Token.cpp
//
//
//  Created by fudan on 17/2/18.
//  Copyright 2017 Kyle Doerrler, Liyu Qiang
//

#include <stdio.h>
#include <string>
#include <iostream>
#include "include/scanner.h"

namespace fcal {
namespace scanner {
    Token::Token() {}
    Token::Token(TokenType terminal, std::string lexeme, Token *next) {
      setToken(terminal, lexeme, next);
    }

    void Token::setToken(TokenType terminal,
                         std::string lexeme, Token *next) {
      terminal_ = terminal;
      lexeme_ = lexeme;
      next_ = next;
    }

    // get the  terminal of token:
    TokenType Token::terminal() {return terminal_;}
    // point to next token:
    Token *Token::next() {return next_;}
    // get the lexme:
    std::string Token::lexeme() {return lexeme_;}

}  // namespace scanner
}  // namespace fcal

