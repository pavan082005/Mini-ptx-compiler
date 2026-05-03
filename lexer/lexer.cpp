#include "lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& source) : input(source), pos(0) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (pos < input.length()) {
        if (isspace(input[pos])) {
            pos++;
            continue;
        }

        // --- NEW: Semicolon Support ---
        if (input[pos] == ';') {
            tokens.push_back({ SEMICOLON, ";" });
            pos++;
            continue;
        }
        // Inside tokenize() while loop
        if (input[pos] == '[') {
            tokens.push_back({ LBRACKET, "[" });
            pos++;
            continue;
        }
        if (input[pos] == ']') {
            tokens.push_back({ RBRACKET, "]" });
            pos++;
            continue;
        }

        if (input[pos] == '+') { tokens.push_back({ PLUS, "+" }); pos++; }
        else if (input[pos] == '-') { tokens.push_back({ MINUS, "-" }); pos++; }
        else if (input[pos] == '*') { tokens.push_back({ MUL, "*" }); pos++; }
        else if (input[pos] == '/') { tokens.push_back({ DIV, "/" }); pos++; }
        else if (input[pos] == '=') { tokens.push_back({ ASSIGN, "=" }); pos++; }
        else if (input[pos] == '(') { tokens.push_back({ LPAREN, "(" }); pos++; }
        else if (input[pos] == ')') { tokens.push_back({ RPAREN, ")" }); pos++; }
        else if (isdigit(input[pos])) {
            std::string val;
            while (pos < input.length() && isdigit(input[pos])) val += input[pos++];
            tokens.push_back({ NUMBER, val });
        }
        else if (isalpha(input[pos]) || input[pos] == '_') {
            std::string val;
            while (pos < input.length() && (isalnum(input[pos]) || input[pos] == '_' || input[pos] == '.'))
                val += input[pos++];

            if (val == "threadIdx.x") tokens.push_back({ THREAD_IDX_X, val });
            else if (val == "blockIdx.x") tokens.push_back({ BLOCK_IDX_X, val });
            else if (val == "__syncthreads") tokens.push_back({ SYNCTHREADS, val });
            else tokens.push_back({ IDENT, val });
        }
        else {
            pos++; // Skip unknown
        }
    }
    tokens.push_back({ END, "" });
    return tokens;
}