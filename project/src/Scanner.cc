//
//  scanner.cc
//
//
//  Created by fudan on 17/2/18.
//  Copyright 2017 Kyle Doerrler, Liyu Qiang
//

#include <stdio.h>
#include <regex.h>
#include <assert.h>
#include <string>
#include <iostream>
#include "include/scanner.h"
#include "include/read_input.h"
#include "include/regex.h"
/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace scanner {
        Scanner::Scanner() {
            TokenType_ = new TokenType[100000];
            lexme_ = new std::string[100000];
            count_ = 0;
            max_matched_ = 0;
            white_space = make_regex("^[\n\t\r ]+");
            block_comment = \
              make_regex("^//[^\n]*\n|/\\*([^\\*]|\\*+[^\\*/])*\\*+/");
            assert(block_comment != NULL);
	    regex_helper_array = MakeTokenRegex();
        }


        Token* Scanner::Scan(const char* text) {
            assert(text != NULL);
            DecideTerminl(text);
            TokenType *type = GetTokenType();
            std::string *lex = GetLexme();
            int n = GetNumberOfTokens();
            Token *tokens = new Token[n + 1];

            int k = n;
            for (int j = 0; j < n; j++) {
                Token *next;
                next = &tokens[j+1];
                tokens[j].setToken(type[j], lex[j], next);

                if (j != 0) {
                    Token *last;
                    last = &tokens[j-1];
                    assert(last != NULL);
                    if (tokens[j].terminal() == kForwardSlash) {
                        assert(last->terminal() != kForwardSlash);
                    }
                }
                k--;
            }
            if (k == 0) {
                char end = '\0';
                std::string str = std::string(1, end);
                tokens[n].setToken(kEndOfFile, str, NULL);
            }
            return tokens;
        }
        
        regex_helper *Scanner::MakeTokenRegex() {
            //regex_t **re_ = new regex_t *[44];
            regex_helper *re_ = new regex_helper[44];
            re_[0].regex_ = make_regex("^int");
            re_[0].terminal_type = kIntKwd;
            re_[1].regex_ = make_regex("^float");
            re_[1].terminal_type = kFloatKwd;
            re_[2].regex_ = make_regex("^boolean");
            re_[2].terminal_type = kBoolKwd;
            re_[3].regex_ = make_regex("^True");
            re_[3].terminal_type = kTrueKwd;
            re_[4].regex_ = make_regex("^False");
            re_[4].terminal_type = kFalseKwd;
            re_[5].regex_ = make_regex("^string");
            re_[5].terminal_type = kStringKwd;
            re_[6].regex_ = make_regex("^matrix");
            re_[6].terminal_type = kMatrixKwd;
            re_[7].regex_ = make_regex("^let");
            re_[7].terminal_type = kLetKwd;
            re_[8].regex_ = make_regex("^in");
            re_[8].terminal_type = kInKwd;
            re_[9].regex_ = make_regex("^end");
            re_[9].terminal_type = kEndKwd;
            re_[10].regex_ = make_regex("^if");
            re_[10].terminal_type = kIfKwd;
            re_[11].regex_ = make_regex("^then");
            re_[11].terminal_type = kThenKwd;
            re_[12].regex_ = make_regex("^else");
            re_[12].terminal_type = kElseKwd;
            re_[13].regex_ = make_regex("^repeat");
            re_[13].terminal_type = kRepeatKwd;
            re_[14].regex_ = make_regex("^while");
            re_[14].terminal_type = kWhileKwd;
            re_[15].regex_ = make_regex("^print");
            re_[15].terminal_type = kPrintKwd;
            re_[16].regex_ = make_regex("^to");
            re_[16].terminal_type = kToKwd;
            re_[17].regex_ = make_regex("^[0-9]+");
            re_[17].terminal_type = kIntConst;
            re_[18].regex_ = make_regex("^[0-9]+\\.[0-9]+");
            re_[18].terminal_type = kFloatConst;
            re_[19].regex_ = make_regex("^\"([^\"]*)\"");
            re_[19].terminal_type = kStringConst;
            re_[20].regex_ = make_regex("^([a-zA-Z]+(\\_|[0-9])*)+");
            re_[20].terminal_type = kVariableName;
            re_[21].regex_ = make_regex("^\\(");
            re_[21].terminal_type = kLeftParen;
            re_[22].regex_ = make_regex("^\\)");
            re_[22].terminal_type = kRightParen;
            re_[23].regex_ = make_regex("^\\{");
            re_[23].terminal_type = kLeftCurly;
            re_[24].regex_ = make_regex("^\\}");
            re_[24].terminal_type = kRightCurly;
            re_[25].regex_ = make_regex("^\\[");
            re_[25].terminal_type = kLeftSquare;
            re_[26].regex_ = make_regex("^\\]");
            re_[26].terminal_type = kRightSquare;
            re_[27].regex_ = make_regex("^\\;");
            re_[27].terminal_type = kSemiColon;
            re_[28].regex_ = make_regex("^\\:");
            re_[28].terminal_type = kColon;
            re_[29].regex_ = make_regex("^\\=");
            re_[29].terminal_type = kAssign;
            re_[30].regex_ = make_regex("^\\+");
            re_[30].terminal_type = kPlusSign;
            re_[31].regex_ = make_regex("^\\*");
            re_[31].terminal_type = kStar;
            re_[32].regex_ = make_regex("^\\-");
            re_[32].terminal_type = kDash;
            re_[33].regex_ = make_regex("^\\/");
            re_[33].terminal_type = kForwardSlash;
            re_[34].regex_ = make_regex("^<");
            re_[34].terminal_type = kLessThan;
            re_[35].regex_ = make_regex("^<=");
            re_[35].terminal_type = kLessThanEqual;
            re_[36].regex_ = make_regex("^>");
            re_[36].terminal_type = kGreaterThan;
            re_[37].regex_ = make_regex("^>=");
            re_[37].terminal_type = kGreaterThanEqual;
            re_[38].regex_ = make_regex("^==");
            re_[38].terminal_type = kEqualsEquals;
            re_[39].regex_ = make_regex("^\\!\\=");
            re_[39].terminal_type = kNotEquals;
            re_[40].regex_ = make_regex("^\\&\\&");
            re_[40].terminal_type = kAndOp;
            re_[41].regex_ = make_regex("^\\|\\|");
            re_[41].terminal_type = kOrOp;
            re_[42].regex_ = make_regex("^\\!");
            re_[42].terminal_type = kNotOp;
            re_[43].regex_ = make_regex("^\\Z");
            re_[43].terminal_type = kEndOfFile;
            return re_;
        }
        int Scanner::Compare_regex(regex_helper* regex, const char* pattern) {
           int num_matched = 0;
           int max_matched = 0;
           for (int i= 0; i < 44; i++) {
               num_matched = match_regex(regex[i].regex_, pattern);
               if (num_matched > max_matched) {
                   max_matched = num_matched;
                   num_regex_ = i;
               }
           }
            if (max_matched == 0) {
                num_regex_ = 44;
            }
           max_matched_ = max_matched;
           return max_matched_;
       }

       int Scanner::consume_whitespace_and_comments(regex_t *white_space,
                                                    regex_t *block_comment,
                                                    const char *text) {
         int num_matched_chars = 0;
         int total_num_matched_chars = 0;
         int still_consuming_white_space;
         do {
             // exit loop if not reset by a match
              still_consuming_white_space = 0;

             // Try to match white space
             num_matched_chars = match_regex(white_space, text);
             total_num_matched_chars += num_matched_chars;
             if (num_matched_chars > 0) {
                 text = text + num_matched_chars;
                 still_consuming_white_space = 1;
             }

             // Try to match block comments
             num_matched_chars = match_regex(block_comment, text);
             total_num_matched_chars += num_matched_chars;
             if (num_matched_chars > 0) {
                 text = text + num_matched_chars;
                 still_consuming_white_space = 1;
             }
         } while (still_consuming_white_space);
         return total_num_matched_chars;
       } /* consume_whitespace_and_comments() */

  void Scanner::DecideTerminl(const char* text) {
            TokenType *token_type = new TokenType[strlen(text)];
            std::string *lexme = new std::string[strlen(text)];
            int max_num_matched_chars = 0;
            int num_matched_chars = 0;
            int number_tokens = 0;
            num_matched_chars =
            consume_whitespace_and_comments(white_space, block_comment,
                                             text);
            text = text + num_matched_chars;
            while (text[0] != '\0') {
                max_num_matched_chars = 0;
                int count = 0;
                TokenType current_token = kIntKwd;
		
                // Try to match a word
                max_num_matched_chars = \
		  Compare_regex(regex_helper_array, text);
                std::string current_text = "";
                if (max_num_matched_chars == 0) {
                    current_token = kLexicalError;
                    current_text.append(text, 1);
                    text = text + 1;
                } else {
                    count = GetRegexIndex();
                    current_token = \
		      regex_helper_array[count].terminal_type;
                    current_text.append(text, max_num_matched_chars);
                    text = text + max_num_matched_chars;
                }
                lexme[number_tokens] = current_text;
                token_type[number_tokens] = current_token;
                num_matched_chars = \
                consume_whitespace_and_comments(white_space, \
                                                block_comment,
                                                text);
                text = text + num_matched_chars;
                number_tokens++;
            }
            lexme_ = lexme;
            TokenType_ = token_type;
            count_ = number_tokens;
        }

    TokenType* Scanner::GetTokenType() {
        return TokenType_;
    }

    std::string* Scanner::GetLexme() {
        return lexme_;
    }

    int Scanner::GetMaxMatch() {
        return max_matched_;
    }

    int Scanner::GetRegexIndex() {
        return num_regex_;
    }

    int Scanner::GetNumberOfTokens() {
        return count_;
    }

}  // namespace scanner
}  // namespace fcal
