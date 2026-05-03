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
    SEMICOLON,
    LBRACKET, RBRACKET,
    // --- NEW GPU TOKENS ---
    THREAD_IDX_X,
    BLOCK_IDX_X,
    SYNCTHREADS,
    // ----------------------
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