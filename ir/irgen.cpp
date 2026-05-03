#include "irgen.h"

IRGenerator::IRGenerator() : tempCount(1) {}

std::string IRGenerator::newTemp() { return "t" + std::to_string(tempCount++); }

std::string IRGenerator::generate(AST* node, std::vector<IR>& code) {
    if (!node) return "";

    if (node->type == BLOCKN) {
        generate(node->left, code);
        generate(node->right, code);
        return "";
    }

    if (node->type == NUM || node->type == VAR) return node->value;

    if (node->type == THREAD_IDXN || node->type == BLOCK_IDXN) {
        std::string t = newTemp();
        code.push_back({ "mov", node->value, "", t });
        return t;
    }

    if (node->type == SYNCN) {
        code.push_back({ "sync", "", "", "" });
        return "";
    }

    if (node->type == LOADN) {
        std::string idx = generate(node->left, code);
        std::string t = newTemp();
        code.push_back({ "load", node->value, idx, t });
        return t;
    }

    if (node->type == STOREN) {
        std::string idx = generate(node->left, code);
        std::string val = generate(node->right, code);
        code.push_back({ "store", val, idx, node->value });
        return "";
    }

    if (node->type == ASSIGNN) {
        std::string r = generate(node->right, code);
        code.push_back({ "mov", r, "", node->left->value });
        return node->left->value;
    }

    std::string l = generate(node->left, code);
    std::string r = generate(node->right, code);
    std::string t = newTemp();

    if (node->type == ADD) code.push_back({ "add", l, r, t });
    else if (node->type == SUB) code.push_back({ "sub", l, r, t });
    else if (node->type == MULN) code.push_back({ "mul", l, r, t });
    else if (node->type == DIVN) code.push_back({ "div", l, r, t });

    return t;
}

std::vector<IR> IRGenerator::generateIR(AST* root) {
    std::vector<IR> code;
    generate(root, code);
    return code;
}