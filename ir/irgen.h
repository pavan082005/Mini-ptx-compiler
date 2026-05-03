#ifndef IRGEN_H
#define IRGEN_H

#include "../ast/ast.h"
#include "ir.h" // <-- Include your existing struct IR definition
#include <vector>
#include <string>

class IRGenerator {
public:
    IRGenerator();
    std::vector<IR> generateIR(AST* root);
    void optimize(std::vector<IR>& code); // Optimizer Pass

private:
    int tempCount;
    std::string newTemp();
    std::string generate(AST* node, std::vector<IR>& code);
};

#endif