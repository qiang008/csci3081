/* 

Copyright 2017 Kyle Doerrler, doerr029

 */
#ifndef GROUP_REPO_GROUP_TURTWIG_PROJECT_TESTS_SCANNER_TESTS_H_
#define GROUP_REPO_GROUP_TURTWIG_PROJECT_TESTS_SCANNER_TESTS_H_

#include <cxxtest/TestSuite.h>

#include <stdio.h>
#include <string>
#include "include/read_input.h"
#include "include/scanner.h"

namespace scanner = fcal::scanner;
class ScannerTestSuite : public CxxTest::TestSuite{
 public:
    /* A Scanner object is created in the test_setup_code method.
       This is a test, beginning with "test_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    scanner::Scanner *s;
    fcal::scanner::regex_helper *res;

    // function to get length of array for creation testing
    int regex_in_array(fcal::scanner::regex_helper *regex) {
        int n = 0;
        for (int i = 0; i < 44; i++) {
            if (regex[i].regex_ != NULL) {
            n++;
            }
        }
        return n;
    }

    void test_setup_code() {
        s = new scanner::Scanner();
    }

    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You should test the regular expressions you use to make and match
       regular expressions,and the functions used by makeRegex and
       matchRegex regex_tests.h.
       However, You will likely need to write several tests to
       adequately test  the functions that are called from "scan".
       Once you are confident that the components used by "scan"
       work properly, then you can run tests on the "scan" method itself.
    */

    void test_create_regex_array() {
        res = new fcal::scanner::regex_helper[44];
        res = s->MakeTokenRegex();
        TS_ASSERT(res != NULL);
        int length = regex_in_array(res);
        TS_ASSERT_EQUALS(length, 44);
    }

    void xtest_get_regex() {
        int g = s->GetRegexIndex();
        TS_ASSERT_EQUALS(g, 0);
    }

    void test_Compare_regex_success() {
        int matched = s->Compare_regex(res, "int");
        TS_ASSERT_EQUALS(matched, 3);
    }

    void test_Compare_regex_fail() {
        int matched = s->Compare_regex(res, "%");
        TS_ASSERT_EQUALS(matched, 0);
    }


    // You should place tests for these other functions used by
    // scan below.

    /* Below is one of the tests for these components in the project
       solution created by your instructor.  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_TokenMaker_leftCurly () {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{");
        }

        Note that this test is here named "xtest_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
    */


    void test_Get_Token_Type() {
      s->DecideTerminl("int");
      TS_ASSERT_EQUALS(*(s->GetTokenType()), scanner::kIntKwd);
    }

    void test_Get_Lexme() {
      s->DecideTerminl("int");
      std::string * lex = new std::string("int");
      std::string * str = new std::string[1];
        str = s->GetLexme();
      TS_ASSERT_EQUALS(*str, *lex);
    }

    void test_Get_Number_Of_Tokens() {
      s->DecideTerminl("int");
      TS_ASSERT_EQUALS(s->GetNumberOfTokens(), 1);
    }

    void test_Get_Max_Match() {
        s->DecideTerminl("int");
        TS_ASSERT_EQUALS(s->GetMaxMatch(), 3);
    }

    bool tokenMaker_tester(const char*pattern, \
                           int regex_index, scanner::TokenType *token, \
                           std::string *lex) {
        s->DecideTerminl(pattern);
        scanner::TokenType *tks = s->GetTokenType();
        std::string *lex_tok = s->GetLexme();
        int count = 0;
        int index = s->GetRegexIndex();
        int n = s->GetNumberOfTokens();
        for (int i = 0; i < n; i++) {
            if (tks[i] == token[i] && lex_tok[i] == lex[i] \
                && index == regex_index) {
                count++;
            } else {
                return false;
            }
        }
        return true;
    }

    void test_terminal_IntKwd() {
        std::string * lex = new std::string("int");
        scanner::TokenType tks[]={scanner::kIntKwd};
        TS_ASSERT(tokenMaker_tester("int ", 0, tks, lex));
    }

    void test_terminal_FloatKwd() {
      std::string * lex = new std::string("float");
      scanner::TokenType tks[]={scanner::kFloatKwd};
      TS_ASSERT(tokenMaker_tester("float ", 1, tks, lex));
    }
    void test_terminal_BooleanKwd() {
        std::string * lex = new std::string("boolean");
        scanner::TokenType tks[]={scanner::kBoolKwd};
        TS_ASSERT(tokenMaker_tester("boolean", 2, tks, lex));
    }

    void test_terminal_TrueKwd() {
        std::string * lex = new std::string("True");
        scanner::TokenType tks[]={scanner::kTrueKwd};
        TS_ASSERT(tokenMaker_tester("True ", 3, tks, lex));
    }
    void test_terminal_FalseKwd() {
        std::string * lex = new std::string("False");
        scanner::TokenType tks[]={scanner::kFalseKwd};
        TS_ASSERT(tokenMaker_tester("False ", 4, tks, lex));
    }

    void test_terminal_StringKwd() {
      std::string * lex = new std::string("string");
      scanner::TokenType tks[]={scanner::kStringKwd};
      TS_ASSERT(tokenMaker_tester("string ", 5, tks, lex));
    }

    void test_terminal_MatrixKwd() {
      std::string * lex = new std::string("matrix");
      scanner::TokenType tks[]={scanner::kMatrixKwd};
      TS_ASSERT(tokenMaker_tester("matrix ", 6, tks, lex));
    }

    void test_terminal_LetKwd() {
      std::string * lex = new std::string("let");
      scanner::TokenType tks[]={scanner::kLetKwd};
      TS_ASSERT(tokenMaker_tester("let ", 7, tks, lex));
    }

    void test_terminal_InKwd() {
      std::string * lex = new std::string("in");
      scanner::TokenType tks[]={scanner::kInKwd};
      TS_ASSERT(tokenMaker_tester("in ", 8, tks, lex));
    }

    void test_terminal_EndKwd() {
      std::string * lex = new std::string("end");
      scanner::TokenType tks[]={scanner::kEndKwd};
      TS_ASSERT(tokenMaker_tester("end ", 9, tks, lex));
    }

    void test_terminal_IfKwd() {
      std::string * lex = new std::string("if");
      scanner::TokenType tks[]={scanner::kIfKwd};
      TS_ASSERT(tokenMaker_tester("if ", 10, tks, lex));
    }

    void test_terminal_ThenKwd() {
      std::string * lex = new std::string("then");
      scanner::TokenType tks[]={scanner::kThenKwd};
      TS_ASSERT(tokenMaker_tester("then ", 11, tks, lex));
    }

    void test_terminal_ElseKwd() {
      std::string * lex = new std::string("else");
      scanner::TokenType tks[]={scanner::kElseKwd};
      TS_ASSERT(tokenMaker_tester("else ", 12, tks, lex));
    }

    void test_terminal_RepeatKwd() {
      std::string * lex = new std::string("repeat");
      scanner::TokenType tks[]={scanner::kRepeatKwd};
      TS_ASSERT(tokenMaker_tester("repeat ", 13, tks, lex));
    }
    void test_terminal_WhileKwd() {
        std::string * lex = new std::string("while");
        scanner::TokenType tks[]={scanner::kWhileKwd};
        TS_ASSERT(tokenMaker_tester("while ", 14, tks, lex));
    }

    void test_terminal_PrintKwd() {
      std::string * lex2 = new std::string("print");
      scanner::TokenType tks[]={scanner::kPrintKwd};
      TS_ASSERT(tokenMaker_tester("print", 15, tks, lex2));
    }

    void test_terminal_ToKwd() {
      std::string * lex = new std::string("to");
      scanner::TokenType tks[]={scanner::kToKwd};
      TS_ASSERT(tokenMaker_tester("to ", 16, tks, lex));
    }

    void test_terminal_IntConst() {
      std::string * lex = new std::string("123");
      scanner::TokenType tks[]={scanner::kIntConst};
      TS_ASSERT(tokenMaker_tester("123 ", 17, tks, lex));
    }

    void test_terminal_FloatConst() {
      std::string * lex = new std::string("3.14");
      scanner::TokenType tks[]={scanner::kFloatConst};
      TS_ASSERT(tokenMaker_tester("3.14 ", 18, tks, lex));
    }

    void test_terminal_StringConst() {
      std::string * lex = new std::string[1];
      lex[0] = "\"string\"";
        scanner::TokenType tks[]={scanner::kStringConst};
      TS_ASSERT(tokenMaker_tester("\"string\"", 19, tks, lex));
    }

    void test_terminal_VariableName() {
      std::string * lex = new std::string("kyle");
      scanner::TokenType tks[]={scanner::kVariableName};
      TS_ASSERT(tokenMaker_tester("kyle ", 20, tks, lex));
    }

    void test_terminal_LeftParen() {
      std::string * lex = new std::string("(");
      scanner::TokenType tks[]={scanner::kLeftParen};
      TS_ASSERT(tokenMaker_tester("( ", 21, tks, lex));
    }

    void test_terminal_RightParen() {
      std::string * lex = new std::string(")");
      scanner::TokenType tks[]={scanner::kRightParen};
      TS_ASSERT(tokenMaker_tester(") ", 22, tks, lex));
    }

    void test_terminal_LeftCurly() {
      std::string * lex = new std::string("{");
      scanner::TokenType tks[]={scanner::kLeftCurly};
      TS_ASSERT(tokenMaker_tester("{ ", 23, tks, lex));
    }

    void test_terminal_RightCurly() {
      std::string * lex = new std::string("}");
      scanner::TokenType tks[]={scanner::kRightCurly};
      TS_ASSERT(tokenMaker_tester("} ", 24, tks, lex));
    }

    void test_terminal_LeftSquare() {
      std::string * lex = new std::string("[");
      scanner::TokenType tks[]={scanner::kLeftSquare};
      TS_ASSERT(tokenMaker_tester("[ ", 25, tks, lex));
    }

    void test_terminal_RightSquare() {
      std::string * lex = new std::string("]");
      scanner::TokenType tks[]={scanner::kRightSquare};
      TS_ASSERT(tokenMaker_tester("] ", 26, tks, lex));
    }

    void test_terminal_SemiColon() {
      std::string * lex = new std::string(";");
      scanner::TokenType tks[]={scanner::kSemiColon};
      TS_ASSERT(tokenMaker_tester("; ", 27, tks, lex));
    }

    void test_terminal_Colon() {
      std::string * lex = new std::string(":");
      scanner::TokenType tks[]={scanner::kColon};
      TS_ASSERT(tokenMaker_tester(": ", 28, tks, lex));
    }

    void test_terminal_Assign() {
      std::string * lex = new std::string("=");
      scanner::TokenType tks[]={scanner::kAssign};
      TS_ASSERT(tokenMaker_tester("= ", 29, tks, lex));
    }

    void test_terminal_PlusSign() {
      std::string * lex = new std::string("+");
      scanner::TokenType tks[]={scanner::kPlusSign};
      TS_ASSERT(tokenMaker_tester("+ ", 30, tks, lex));
    }

    void test_terminal_Star() {
      std::string * lex = new std::string("*");
      scanner::TokenType tks[]={scanner::kStar};
      TS_ASSERT(tokenMaker_tester("* ", 31, tks, lex));
    }

    void test_terminal_Dash() {
      std::string * lex = new std::string("-");
      scanner::TokenType tks[]={scanner::kDash};
      TS_ASSERT(tokenMaker_tester("- ", 32, tks, lex));
    }

    void test_terminal_ForwardSlash() {
      std::string * lex = new std::string("/");
      scanner::TokenType tks[]={scanner::kForwardSlash};
      TS_ASSERT(tokenMaker_tester("/ ", 33, tks, lex));
    }

    void test_terminal_LessThan() {
      std::string * lex = new std::string("<");
      scanner::TokenType tks[]={scanner::kLessThan};
      TS_ASSERT(tokenMaker_tester("< ", 34, tks, lex));
    }

    void test_terminal_LessThanEqual() {
      std::string * lex = new std::string("<=");
      scanner::TokenType tks[]={scanner::kLessThanEqual};
      TS_ASSERT(tokenMaker_tester("<= ", 35, tks, lex));
    }

    void test_terminal_GreaterThan() {
      std::string * lex = new std::string(">");
      scanner::TokenType tks[]={scanner::kGreaterThan};
      TS_ASSERT(tokenMaker_tester("> ", 36, tks, lex));
    }

    void test_terminal_GreaterThanEqual() {
      std::string * lex = new std::string(">=");
      scanner::TokenType tks[]={scanner::kGreaterThanEqual};
      TS_ASSERT(tokenMaker_tester(">=", 37, tks, lex));
    }

    void test_terminal_EqualsEquals() {
      std::string * lex = new std::string("==");
      scanner::TokenType tks[]={scanner::kEqualsEquals};
      TS_ASSERT(tokenMaker_tester("== ", 38, tks, lex));
    }

    void test_terminal_NotEquals() {
      std::string * lex = new std::string("!=");
      scanner::TokenType tks[]={scanner::kNotEquals};
      TS_ASSERT(tokenMaker_tester("!= ", 39, tks, lex));
    }

    void test_terminal_AndOp() {
      std::string * lex = new std::string("&&");
      scanner::TokenType tks[]={scanner::kAndOp};
      TS_ASSERT(tokenMaker_tester("&& ", 40, tks, lex));
    }

    void test_terminal_OrOp() {
      std::string * lex = new std::string("||");
      scanner::TokenType tks[]={scanner::kOrOp};
      TS_ASSERT(tokenMaker_tester("|| ", 41, tks, lex));
    }

    void test_terminal_NotOp() {
      std::string * lex = new std::string("!");
      scanner::TokenType tks[]={scanner::kNotOp};
      TS_ASSERT(tokenMaker_tester("! ", 42, tks, lex));
    }

    void test_terminal_EndOfFile() {
      std::string * lex = new std::string("\0");
      scanner::TokenType tks[]={scanner::kEndOfFile};
      TS_ASSERT(tokenMaker_tester("\0", 43, tks, lex));
    }
    void test_terminal_LexicalError() {
        std::string * lex = new std::string("#");
        scanner::TokenType tks[]={scanner::kLexicalError};
        TS_ASSERT(tokenMaker_tester("#", 44, tks, lex));
    }


    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */



    // Tests for "scan"
    // --------------------------------------------------

    /* Below are some helper functions and sample tests for testing the
       "scan" method on Scanner.
    */

    // Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors(scanner::Token *tks) {
        scanner::Token *currentToken = tks;
        while (currentToken != NULL) {
          if (currentToken->terminal() == scanner::kLexicalError) {
              printf("problem: %s\n", currentToken->lexeme().c_str());
              fflush(stdout);
                                return false;
            } else {
              currentToken = currentToken->next();
            }
        }
        return true;
    }

    /* A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned.
    */
    void scanFileNoLexicalErrors(const char* filename) {
        char *text =  scanner::ReadInputFromFile(filename);
        TS_ASSERT(text);
        scanner::Token *tks = s->Scan(text);
        TS_ASSERT(tks != NULL);
        TS_ASSERT(noLexicalErrors(tks));
    }


    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
  bool sameTerminals(scanner::Token *tks, int numTerms,
                     scanner::TokenType *ts) {
        scanner::Token *currentToken = tks;
        int termIndex = 0;
        while (currentToken != NULL && termIndex < numTerms) {
            if (currentToken->terminal() != ts[termIndex]) {
                printf("item at index: %i: terminal: %i should be terminal:"\
                       " %i\n", termIndex, currentToken->terminal(), \
                       ts[termIndex]);
                std::fflush(stdout);
                return false;
            } else {
                ++termIndex;
                currentToken = currentToken->next();
            }
        }
        return true;
    }


    /* Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    // The "endOfFile" token is always the last one in the list of tokens.

    void test_scan_empty() {
        scanner::Token *tks = s->Scan("  ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }

    void test_scan_empty_comment() {
        scanner::Token *tks = s->Scan(" /* a comment */ ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }

    // When a lexical error occurs, the scanner creates a token with a
    // single-character lexeme and lexicalError as the terminal.
    void test_scan_lexicalErrors() {
        scanner::Token *tks = s->Scan("$&1  ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kLexicalError);
        TS_ASSERT_EQUALS(tks->lexeme(), "$");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kLexicalError);
        TS_ASSERT_EQUALS(tks->lexeme(), "&");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kIntConst);
        TS_ASSERT_EQUALS(tks->lexeme(), "1");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }


    // A test for scanning numbers and a variable.

    void test_scan_nums_vars() {
        scanner::Token *tks = s->Scan(" 123 x 12.34 ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kIntConst, \
                                    scanner::kVariableName, \
                                    scanner::kFloatConst, \
                                    scanner::kEndOfFile };
        TS_ASSERT(sameTerminals(tks, 4, ts));
    }


    /* This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexemes
       are correct.
     */

    void test_scan_bad_syntax_good_tokens() {
        const char *filename = "./samples/bad_syntax_good_tokens.dsl";
        char *text =  scanner::ReadInputFromFile(filename);
        TS_ASSERT(text);
        scanner::Token *tks = NULL;
        tks = s->Scan(text);
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = {
          scanner::kIntConst, scanner::kIntConst, scanner::kIntConst, \
          scanner::kIntConst,
          scanner::kStringConst, scanner::kStringConst, scanner::kStringConst,
          scanner::kFloatConst, scanner::kFloatConst, scanner::kFloatConst,
          scanner::kMatrixKwd,
          scanner::kSemiColon, scanner::kColon, scanner::kToKwd,
          scanner::kLeftCurly, scanner::kLeftParen, scanner::kRightCurly, \
          scanner::kRightParen,
          scanner::kPlusSign, scanner::kStar, scanner::kDash, \
          scanner::kForwardSlash,
          scanner::kEqualsEquals, scanner::kLessThanEqual,
          scanner::kGreaterThanEqual, scanner::kNotEquals,
          scanner::kAssign,
          scanner::kVariableName, scanner::kVariableName, \
          scanner::kVariableName, scanner::kVariableName,
          scanner::kVariableName, scanner::kVariableName, \
          scanner::kVariableName,
          scanner::kIntKwd, scanner::kFloatKwd, scanner::kStringKwd,
          scanner::kEndOfFile
        };
        int count = 38;
        TS_ASSERT(sameTerminals(tks, count, ts));
    }

    void test_scan_sample_forestLoss() {
        scanFileNoLexicalErrors("./samples/forest_loss_v2.dsl");
    }
};

#endif  // GROUP_REPO_GROUP_TURTWIG_PROJECT_TESTS_SCANNER_TESTS_H_
