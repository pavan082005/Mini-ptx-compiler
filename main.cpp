#include <bits/stdc++.h>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "ir/irgen.h"
using namespace std;
// ---------------- TOKEN PRINT ----------------
string tokenToString(TokenType type) {
    switch (type) {
    case IDENT: return "IDENT";
    case NUMBER: return "NUMBER";
    case PLUS: return "PLUS";
    case MINUS: return "MINUS";
    case MUL: return "MUL";
    case DIV: return "DIV";
    case ASSIGN: return "ASSIGN";
    case LPAREN: return "LPAREN";
    case RPAREN: return "RPAREN";
    case END: return "END";
    default: return "UNKNOWN";
    }
}

void printTokens(const vector<Token>& tokens) {
    cout << "\nTokens:\n";
    for (const auto& token : tokens) {
        cout << tokenToString(token.type)
            << " (" << token.value << ")\n";
    }
}

// ---------------- AST PRINT ----------------
void printAST(AST* node, int depth = 0) {
    if (!node) return;

    for (int i = 0; i < depth; i++) cout << "  ";

    switch (node->type) {
    case NUM: cout << "NUM(" << node->value << ")\n"; break;
    case VAR: cout << "VAR(" << node->value << ")\n"; break;
    case ADD: cout << "ADD\n"; break;
    case SUB: cout << "SUB\n"; break;
    case MULN: cout << "MUL\n"; break;
    case DIVN: cout << "DIV\n"; break;
    case ASSIGNN: cout << "ASSIGN\n"; break;
    }

    printAST(node->left, depth + 1);
    printAST(node->right, depth + 1);
}

// ---------------- IR PRINT ----------------
void printIR(const vector<IR>& code) {
    cout << "\nIR:\n";
    for (const auto& instr : code) {
        if (instr.op == "mov") {
            cout << instr.result << " = " << instr.arg1 << "\n";
        }
        else {
            cout << instr.result << " = "
                << instr.arg1 << " "
                << instr.op << " "
                << instr.arg2 << "\n";
        }
    }
}

// ---------------- MAIN ----------------
int main() {
    string input;

    cout << "Enter expression: ";
    getline(cin, input);

    try {
        // Phase 1: Lexer
        Lexer lexer(input);
        auto tokens = lexer.tokenize();
        printTokens(tokens);

        // Phase 2: Parser → AST
        Parser parser(tokens);
        AST* tree = parser.parse();

        cout << "\nAST:\n";
        printAST(tree);

        // Phase 3: IR Generation
        IRGenerator irgen;
        auto ir = irgen.generateIR(tree);

        printIR(ir);
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}