#include "parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) {
    this->tokens = tokens;
    pos = 0;
}

Token Parser::currentToken() {
    return tokens[pos];
}

void Parser::eat(TokenType type) {
    if (currentToken().type == type) {
        pos++;
    }
    else {
        throw std::runtime_error("Unexpected token");
    }
}

AST* Parser::parse() {
    return parseStatement();
}

AST* Parser::parseStatement() {
    // IDENT = expr
    Token token = currentToken();

    if (token.type == IDENT) {
        eat(IDENT);
        AST* left = new AST(VAR, token.value);

        eat(ASSIGN);

        AST* right = parseExpr();

        AST* node = new AST(ASSIGNN);
        node->left = left;
        node->right = right;

        return node;
    }

    throw std::runtime_error("Invalid statement");
}

AST* Parser::parseExpr() {
    AST* node = parseTerm();

    while (currentToken().type == PLUS || currentToken().type == MINUS) {
        Token token = currentToken();

        if (token.type == PLUS) {
            eat(PLUS);
            AST* newNode = new AST(ADD);
            newNode->left = node;
            newNode->right = parseTerm();
            node = newNode;
        }
        else if (token.type == MINUS) {
            eat(MINUS);
            AST* newNode = new AST(SUB);
            newNode->left = node;
            newNode->right = parseTerm();
            node = newNode;
        }
    }

    return node;
}

AST* Parser::parseTerm() {
    AST* node = parseFactor();

    while (currentToken().type == MUL || currentToken().type == DIV) {
        Token token = currentToken();

        if (token.type == MUL) {
            eat(MUL);
            AST* newNode = new AST(MULN);
            newNode->left = node;
            newNode->right = parseFactor();
            node = newNode;
        }
        else if (token.type == DIV) {
            eat(DIV);
            AST* newNode = new AST(DIVN);
            newNode->left = node;
            newNode->right = parseFactor();
            node = newNode;
        }
    }

    return node;
}

AST* Parser::parseFactor() {
    Token token = currentToken();

    if (token.type == NUMBER) {
        eat(NUMBER);
        return new AST(NUM, token.value);
    }

    if (token.type == IDENT) {
        eat(IDENT);
        return new AST(VAR, token.value);
    }

    if (token.type == LPAREN) {
        eat(LPAREN);
        AST* node = parseExpr();
        eat(RPAREN);
        return node;
    }

    throw std::runtime_error("Invalid factor");
}