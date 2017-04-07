/*******************************************************************************
 * Name            : iter2scanner.h
 * Project         : fcal
 * Module          : scanner
 * Description     : Definitions of keywords/constants/etc.
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Authors : John Harwell, Dan Challou
 *
 ******************************************************************************/

#ifndef INCLUDE_SCANNER_H_
#define INCLUDE_SCANNER_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <iostream>
/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace scanner {

/*******************************************************************************
 * Constant Definitions
 ******************************************************************************/
/*
 * This enumerated type is used to keep track of what kind of
 * construct was matched.
 */
enum kTokenEnumType {
  kBoolKwd,
  kIntKwd,
  kFloatKwd,
  //kBoolKwd,
  kTrueKwd,
  kFalseKwd,
  kStringKwd,
  kMatrixKwd,
  kLetKwd,
  kInKwd,
  kEndKwd,
  kIfKwd,
  kThenKwd,
  kElseKwd,
  kRepeatKwd,
  kWhileKwd,
  kPrintKwd,
  kToKwd,

  // Constants
  kIntConst,
  kFloatConst,
  kStringConst,

  // Names
  kVariableName,

  // Punctuation
  kLeftParen,
  kRightParen,
  kLeftCurly,
  kRightCurly,
  kLeftSquare,
  kRightSquare,
  kSemiColon,
  kColon,

  // Operators
  kAssign,
  kPlusSign,
  kStar,
  kDash,
  kForwardSlash,
  kLessThan,
  kLessThanEqual,
  kGreaterThan,
  kGreaterThanEqual,
  kEqualsEquals,
  kNotEquals,
  kAndOp,
  kOrOp,
  kNotOp,

  // Special terminal types
  kEndOfFile,
  kLexicalError
};
typedef enum kTokenEnumType TokenType;
inline TokenType& operator++ (TokenType& token, int) {
        const int i = static_cast<int>(token)+1;
        token = static_cast<TokenType>((i) % 45);
        return token;
}
 
 struct regex_helper{
   regex_t *regex_;
   TokenType terminal_type;
 };
 
class Token{
 private:
        TokenType terminal_;
        std::string lexeme_;
        Token *next_;
 public:
        Token();
        Token(TokenType terminal, std::string lexeme, Token *next);
        void setToken(TokenType terminal, std::string lexeme, Token *next);
        TokenType terminal();  // get the  terminal of token
        Token *next();  // point to next token
        std::string lexeme();  // get the lexme
};

class Scanner {
 private:
        TokenType *TokenType_;
        std::string *lexme_;
	regex_helper *regex_helper_array;
        int max_matched_;
        int count_;
        int num_regex_;
        regex_t *white_space;
        regex_t *block_comment;
    
 public:
        Scanner();
        Token* Scan(const char* text);
        TokenType* CreateTypeList();
        TokenType* GetTokenType();
        regex_helper *MakeTokenRegex();
        std::string* GetLexme();
        int GetNumberOfTokens();
        int GetMaxMatch();
        int GetRegexIndex();
        void DecideTerminl(const char* text);
        int Compare_regex(regex_helper* regex, const char* pattern);
        int consume_whitespace_and_comments(regex_t *white_space,
                                            regex_t *block_comment,
                                            const char *text);
};


} /* namespace scanner */
} /* namespace fcal */

#endif  // INCLUDE_I2SCANNER_H_
