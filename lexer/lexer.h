#ifndef LEXER_H
#define LEXER_H

#include <bits/stdc++.h>
using namespace std;

enum TokenType {
    IDENT,
    NUMBER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    ASSIGN,
    LPAREN,
    RPAREN,
    END
};

struct Token {
    TokenType type;
    string value;
};

class Lexer {
private:
    string input;
    int pos;

public:
    Lexer(const std::string& text);
    vector<Token> tokenize();
};

#endif