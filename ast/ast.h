#ifndef AST_H
#define AST_H
#include <bits/stdc++.h>
using namespace std;
enum NodeType {
    NUM,
    VAR,
    ADD,
    SUB,
    MULN,
    DIVN,
    ASSIGNN
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