#ifndef IRGEN_H
#define IRGEN_H

#include <bits/stdc++.h>
#include "../ast/ast.h"
#include "ir.h"
using namespace std;

class IRGenerator {
private:
    int tempCount;

    string newTemp();
    string generate(AST* node, vector<IR>& code);

public:
    IRGenerator();
    vector<IR> generateIR(AST* root);
};

#endif