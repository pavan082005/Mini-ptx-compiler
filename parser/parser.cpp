#include "parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : pos(0), tokens(tokens) {}

Token Parser::currentToken() {
    if (pos >= tokens.size()) return { END, "" };
    return tokens[pos];
}

void Parser::eat(TokenType type) {
    if (currentToken().type == type) pos++;
    else throw std::runtime_error("Unexpected token: " + currentToken().value);
}

AST* Parser::parse() {
    if (currentToken().type == END) return nullptr;

    AST* node = new AST(BLOCKN, "STMT_BLOCK");
    node->left = parseStatement();

    if (currentToken().type == SEMICOLON) eat(SEMICOLON);

    node->right = parse();
    return node;
}

AST* Parser::parseStatement() {
    Token token = currentToken();

    if (token.type == IDENT) {
        eat(IDENT);
        std::string name = token.value;

        // Array Store: name[index] = expr
        if (currentToken().type == LBRACKET) {
            eat(LBRACKET);
            AST* index = parseExpr();
            eat(RBRACKET);
            eat(ASSIGN);
            AST* valNode = parseExpr();

            AST* node = new AST(STOREN, name);
            node->left = index;   // Index
            node->right = valNode; // Value
            return node;
        }
        // Standard Assignment: name = expr
        else {
            eat(ASSIGN);
            AST* right = parseExpr();
            AST* node = new AST(ASSIGNN);
            node->left = new AST(VAR, name);
            node->right = right;
            return node;
        }
    }
    else if (token.type == SYNCTHREADS) {
        eat(SYNCTHREADS); eat(LPAREN); eat(RPAREN);
        return new AST(SYNCN);
    }
    throw std::runtime_error("Invalid statement: " + token.value);
}

AST* Parser::parseExpr() {
    AST* node = parseTerm();
    while (currentToken().type == PLUS || currentToken().type == MINUS) {
        TokenType type = currentToken().type;
        eat(type);
        AST* newNode = new AST(type == PLUS ? ADD : SUB);
        newNode->left = node;
        newNode->right = parseTerm();
        node = newNode;
    }
    return node;
}

AST* Parser::parseTerm() {
    AST* node = parseFactor();
    while (currentToken().type == MUL || currentToken().type == DIV) {
        TokenType type = currentToken().type;
        eat(type);
        AST* newNode = new AST(type == MUL ? MULN : DIVN);
        newNode->left = node;
        newNode->right = parseFactor();
        node = newNode;
    }
    return node;
}

AST* Parser::parseFactor() {
    Token t = currentToken();
    if (t.type == NUMBER) { eat(NUMBER); return new AST(NUM, t.value); }
    if (t.type == THREAD_IDX_X) { eat(THREAD_IDX_X); return new AST(THREAD_IDXN, "%tid.x"); }
    if (t.type == BLOCK_IDX_X) { eat(BLOCK_IDX_X); return new AST(BLOCK_IDXN, "%ctaid.x"); }
    if (t.type == IDENT) {
        eat(IDENT);
        if (currentToken().type == LBRACKET) { // Array Load: name[index]
            eat(LBRACKET);
            AST* index = parseExpr();
            eat(RBRACKET);
            AST* node = new AST(LOADN, t.value);
            node->left = index;
            return node;
        }
        return new AST(VAR, t.value);
    }
    if (t.type == LPAREN) { eat(LPAREN); AST* n = parseExpr(); eat(RPAREN); return n; }
    throw std::runtime_error("Unexpected factor: " + t.value);
}