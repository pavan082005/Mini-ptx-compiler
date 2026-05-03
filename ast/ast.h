#ifndef AST_H
#define AST_H

#include <string>

using namespace std;

enum NodeType {
    NUM,
    VAR,
    ADD,
    SUB,
    MULN,
    DIVN,
    ASSIGNN,
    // --- NEW GPU NODE TYPES ---
    THREAD_IDXN,
    BLOCK_IDXN,
    SYNCN,
    // --------------------------
    BLOCKN,
    LOADN, STOREN
};

struct AST {
    NodeType type;
    string value;
    AST* left;
    AST* right;

    AST(NodeType t, std::string val = "")
        : type(t), value(val), left(nullptr), right(nullptr) {
    }
};

#endif