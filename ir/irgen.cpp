#include "irgen.h"

// Constructor
IRGenerator::IRGenerator() {
    tempCount = 1;
}

// Generate new temporary variable (t1, t2, ...)
std::string IRGenerator::newTemp() {
    return "t" + std::to_string(tempCount++);
}

// Main recursive function
std::string IRGenerator::generate(AST* node, std::vector<IR>& code) {
    if (!node) return "";

    // Case 1: Number
    if (node->type == NUM) {
        return node->value;  // e.g., "5"
    }

    // Case 2: Variable
    if (node->type == VAR) {
        return node->value;  // e.g., "a"
    }

    // Case 3: Assignment
    if (node->type == ASSIGNN) {
        std::string right = generate(node->right, code);
        std::string left = node->left->value;

        code.push_back({ "mov", right, "", left });
        return left;
    }

    // Case 4: Binary operations
    std::string left = generate(node->left, code);
    std::string right = generate(node->right, code);

    std::string temp = newTemp();

    if (node->type == ADD) {
        code.push_back({ "add", left, right, temp });
    }
    else if (node->type == SUB) {
        code.push_back({ "sub", left, right, temp });
    }
    else if (node->type == MULN) {
        code.push_back({ "mul", left, right, temp });
    }
    else if (node->type == DIVN) {
        code.push_back({ "div", left, right, temp });
    }

    return temp;
}

// Entry function
std::vector<IR> IRGenerator::generateIR(AST* root) {
    std::vector<IR> code;
    generate(root, code);
    return code;
}