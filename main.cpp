#include <iostream>
#include "lexer/lexer.h"
#include "parser/parser.h"

void printAST(AST* node, int depth = 0) {
    if (!node) return;

    for (int i = 0; i < depth; i++) std::cout << "  ";

    switch (node->type) {
    case NUM: std::cout << "NUM(" << node->value << ")\n"; break;
    case VAR: std::cout << "VAR(" << node->value << ")\n"; break;
    case ADD: std::cout << "ADD\n"; break;
    case SUB: std::cout << "SUB\n"; break;
    case MULN: std::cout << "MUL\n"; break;
    case DIVN: std::cout << "DIV\n"; break;
    case ASSIGNN: std::cout << "ASSIGN\n"; break;
    }

    printAST(node->left, depth + 1);
    printAST(node->right, depth + 1);
}

int main() {
    std::string input;
    std::cout << "Enter expression: ";
    std::getline(std::cin, input);

    Lexer lexer(input);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    AST* tree = parser.parse();

    std::cout << "\nAST:\n";
    printAST(tree);

    return 0;
}