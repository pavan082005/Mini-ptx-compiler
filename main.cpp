#include <iostream>
#include <vector>
#include <string>
#include "lexer/lexer.h"
#include "ast/ast.h"
#include "parser/parser.h"
#include "ir/irgen.h"
#include "codegen/ptxgen.h"

using namespace std;

/**
 * Helper to visualize the AST - Updated for Phase 4
 */
void printAST(AST* node, int level = 0) {
    if (!node) return;
    for (int i = 0; i < level; ++i) cout << "  ";

    if (node->type == BLOCKN) cout << "[BLOCK]";
    else if (node->type == LOADN) cout << "[LOAD: " << node->value << "]";
    else if (node->type == STOREN) cout << "[STORE: " << node->value << "]";
    else if (node->type == ASSIGNN) cout << "[ASSIGN]";
    else cout << node->value;

    cout << "\n";
    printAST(node->left, level + 1);
    printAST(node->right, level + 1);
}

void compile(string sourceCode) {
    cout << "\n====================================" << endl;
    cout << "SOURCE CODE:" << endl;
    cout << sourceCode << endl;
    cout << "====================================" << endl;

    try {
        // 1. Lexical Analysis
        Lexer lexer(sourceCode);
        vector<Token> tokens = lexer.tokenize();

        // 2. Parsing
        Parser parser(tokens);
        AST* tree = parser.parse();

        cout << "\nAST STRUCTURE:" << endl;
        printAST(tree);

        // 3. IR Generation
        IRGenerator irGen;
        vector<IR> intermediateCode = irGen.generateIR(tree);

        // 4. PTX Code Generation
        cout << "\nGENERATED PTX ASSEMBLY:" << endl;
        PTXGenerator ptxGen;
        ptxGen.generate(intermediateCode);

    }
    catch (const exception& e) {
        cerr << "\nCOMPILER ERROR: " << e.what() << endl;
    }
}

int main() {
    // TEST 1: The Vector Scale (The most common GPU operation)
    // This tests: 
    // - Index calculation
    // - Loading from Global Memory
    // - Scalar multiplication
    // - Storing back to Global Memory
    string vectorScale =
        "i = threadIdx.x; "
        "val = input[i]; "
        "res = val * 10; "
        "output[i] = res;";

    cout << "--- TEST 1: VECTOR SCALE ---" << endl;
    compile(vectorScale);

    cout << "\n\n";

    // TEST 2: The Multi-Array Test
    // This tests if the compiler can handle two different arrays 
    // and a constant in the same line of code.
    string multiArray =
        "id = threadIdx.x; "
        "temp = dataA[id] + dataB[id]; "
        "result[id] = temp;";

    cout << "--- TEST 2: MULTI-ARRAY ADDITION ---" << endl;
    compile(multiArray);

    return 0;
}