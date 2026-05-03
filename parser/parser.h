#ifndef PARSER_H
#define PARSER_H

#include <bits/stdc++.h>
using namespace std;
#include "../lexer/lexer.h"
#include "../ast/ast.h"

class Parser {
private:
    vector<Token> tokens;
    int pos;

    Token currentToken();
    void eat(TokenType type);

    AST* parseStatement();
    AST* parseExpr();
    AST* parseTerm();
    AST* parseFactor();

public:
    Parser(const std::vector<Token>& tokens);
    AST* parse();
};

#endif