#include "irgen.h"
#include <iostream>
#include <unordered_set>

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

    // ==========================================
    // OPTIMIZER: CONSTANT FOLDING
    // ==========================================
    bool l_is_num = !l.empty() && (isdigit(l[0]) || (l[0] == '-' && l.length() > 1 && isdigit(l[1])));
    bool r_is_num = !r.empty() && (isdigit(r[0]) || (r[0] == '-' && r.length() > 1 && isdigit(r[1])));

    if (l_is_num && r_is_num) {
        float val1 = std::stof(l);
        float val2 = std::stof(r);
        float result = 0;

        if (node->type == ADD) result = val1 + val2;
        else if (node->type == SUB) result = val1 - val2;
        else if (node->type == MULN) result = val1 * val2;
        else if (node->type == DIVN) result = val1 / val2;

        // Clean up trailing zeros
        std::string resStr = std::to_string(result);
        resStr.erase(resStr.find_last_not_of('0') + 1, std::string::npos);
        if (resStr.back() == '.') resStr.pop_back();

        return resStr; // Returns a number directly, skipping IR generation!
    }
    // ==========================================

    std::string t = newTemp();
    if (node->type == ADD) code.push_back({ "add", l, r, t });
    else if (node->type == SUB) code.push_back({ "sub", l, r, t });
    else if (node->type == MULN) code.push_back({ "mul", l, r, t });
    else if (node->type == DIVN) code.push_back({ "div", l, r, t });

    return t;
}

void IRGenerator::optimize(std::vector<IR>& code) {
    // ==========================================
    // OPTIMIZER: DEAD CODE ELIMINATION (DCE)
    // ==========================================
    std::unordered_set<std::string> readVars;

    // Pass 1: Find all variables that are actually used
    for (const auto& instr : code) {
        if (!instr.arg1.empty() && instr.op != "load" && instr.op != "store") readVars.insert(instr.arg1);
        if (!instr.arg2.empty()) readVars.insert(instr.arg2);
        if (instr.op == "store") {
            readVars.insert(instr.arg1); // Value stored
            readVars.insert(instr.arg2); // Index stored at
        }
    }

    std::vector<IR> optimizedCode;
    for (const auto& instr : code) {
        if (instr.op == "store" || instr.op == "sync" || instr.op == "load") {
            optimizedCode.push_back(instr);
            continue;
        }

        // Pass 2: If a variable is created but never read, delete it!
        if (readVars.find(instr.result) == readVars.end() && !instr.result.empty()) {
            std::cout << "    [Optimizer] Dead code eliminated -> " << instr.result << "\n";
            continue;
        }

        optimizedCode.push_back(instr);
    }
    code = optimizedCode;
}

std::vector<IR> IRGenerator::generateIR(AST* root) {
    std::vector<IR> code;
    generate(root, code);
    optimize(code); // Trigger optimization before passing to Codegen
    return code;
}