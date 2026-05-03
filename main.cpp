#include <iostream>
#include <vector>
#include <string>
#include "lexer/lexer.h"
#include "ast/ast.h"
#include "parser/parser.h"
#include "ir/irgen.h"
#include "codegen/ptxgen.h"

using namespace std;

void compile(string sourceCode) {
    cout << "\n====================================" << endl;
    cout << "SOURCE CODE:" << endl;
    cout << sourceCode << endl;
    cout << "====================================" << endl;

    try {
        Lexer lexer(sourceCode);
        vector<Token> tokens = lexer.tokenize();

        Parser parser(tokens);
        AST* tree = parser.parse();

        cout << "\n--- OPTIMIZATION PASSES ---" << endl;
        IRGenerator irGen;
        vector<IR> intermediateCode = irGen.generateIR(tree);

        cout << "\nGENERATED PTX ASSEMBLY:" << endl;
        PTXGenerator ptxGen;
        ptxGen.generate(intermediateCode);
    }
    catch (const exception& e) {
        cerr << "\nCOMPILER ERROR: " << e.what() << endl;
    }
}

int main() {
    // PERFORMANCE TEST
    // 1. '2 + 2' will be folded into '4' at compile time (No GPU add instruction).
    // 2. 'unused = val + 10' will be detected as dead code and completely removed.
    string perfTest =
        "idx = threadIdx.x; "
        "offset = 2 + 2; "
        "val = input[idx]; "
        "unused = val + 10; "
        "output[idx] = val + offset;";

    cout << "--- OPTIMIZING COMPILER TEST ---" << endl;
    compile(perfTest);

    return 0;
}