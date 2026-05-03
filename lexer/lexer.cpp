#include "lexer.h"
#include <bits/stdc++.h>
using namespace std;

Lexer::Lexer(const std::string& text) : input(text), pos(0) {}

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;
    while (pos < input.size()) {
        char current = input[pos];
        if (isspace(current)) {
            pos++;
        } else if (isalpha(current)) {
            string ident;
            while (pos < input.size() && isalnum(input[pos])) {
                ident += input[pos++];
            }
            tokens.push_back({IDENT, ident});
        } else if (isdigit(current)) {
            string number;
            while (pos < input.size() && isdigit(input[pos])) {
                number += input[pos++];
            }
            tokens.push_back({NUMBER, number});
        } else {
            switch (current) {
                case '+': tokens.push_back({PLUS, "+"}); break;
                case '-': tokens.push_back({MINUS, "-"}); break;
                case '*': tokens.push_back({MUL, "*"}); break;
                case '/': tokens.push_back({DIV, "/"}); break;
                case '=': tokens.push_back({ASSIGN, "="}); break;
                case '(': tokens.push_back({LPAREN, "("}); break;
                case ')': tokens.push_back({RPAREN, ")"}); break;
                default: throw runtime_error("Unknown character: " + string(1, current));
            }
            pos++;
        }
    }
    tokens.push_back({END, ""});
    return tokens;
}