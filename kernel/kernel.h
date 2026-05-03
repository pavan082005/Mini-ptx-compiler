#ifndef KERNEL_H
#define KERNEL_H

#include <bits/stdc++.h>
using namespace std;
#include "../ast/ast.h"

struct Kernel {
    string name;
    int numThreads;

    AST* body;

    bool isVectorized;
};

#endif