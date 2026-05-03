#include "ptxgen.h"
#include <bits/stdc++.h>
using namespace std;

void PTXGenerator::generate(const vector<IR>& code) {
    unordered_map<string, string> regMap;
    int regCount = 1;

    auto getReg = [&](const string& var) {
        if (regMap.find(var) == regMap.end()) {
            regMap[var] = "%f" + to_string(regCount++);
        }
        return regMap[var];
        };

    cout << "\nPTX:\n";

    for (const auto& instr : code) {
        if (instr.op == "mov") {
            cout << "mov.f32 "
                << getReg(instr.result) << ", "
                << getReg(instr.arg1) << ";\n";
        }
        else if (instr.op == "add") {
            cout << "add.f32 "
                << getReg(instr.result) << ", "
                << getReg(instr.arg1) << ", "
                << getReg(instr.arg2) << ";\n";
        }
        else if (instr.op == "sub") {
            cout << "sub.f32 "
                << getReg(instr.result) << ", "
                << getReg(instr.arg1) << ", "
                << getReg(instr.arg2) << ";\n";
        }
        else if (instr.op == "mul") {
            cout << "mul.f32 "
                << getReg(instr.result) << ", "
                << getReg(instr.arg1) << ", "
                << getReg(instr.arg2) << ";\n";
        }
        else if (instr.op == "div") {
            cout << "div.f32 "
                << getReg(instr.result) << ", "
                << getReg(instr.arg1) << ", "
                << getReg(instr.arg2) << ";\n";
        }
    }
}