#include "ptxgen.h"
#include <iostream>
#include <unordered_map>

using namespace std;

void PTXGenerator::generate(const vector<IR>& code) {
    unordered_map<string, string> regMap;
    int regCount = 1;

    auto getReg = [&](const string& var) {
        if (var.empty()) return string("");
        if (isdigit(var[0]) || var[0] == '-') return var;
        if (regMap.find(var) == regMap.end()) regMap[var] = "%f" + to_string(regCount++);
        return regMap[var];
        };

    cout << ".version 7.0\n.target sm_50\n.address_size 64\n\n";
    cout << ".entry main_kernel (\n    .param .u64 input_ptr,\n    .param .u64 output_ptr\n) {\n";
    cout << "    .reg .f32 %f<100>;\n";
    cout << "    .reg .u64 %rd<10>; // For 64-bit address calculation\n\n";

    for (const auto& instr : code) {
        if (instr.op == "mov") {
            if (instr.arg1 == "%tid.x" || instr.arg1 == "%ctaid.x")
                cout << "    cvt.rn.f32.u32 " << getReg(instr.result) << ", " << instr.arg1 << ";\n";
            else
                cout << "    mov.f32 " << getReg(instr.result) << ", " << getReg(instr.arg1) << ";\n";
        }
        else if (instr.op == "load") {
            cout << "    // Arch: In reality, we'd cvt.u64.f32 to %rd here for memory offset\n";
            cout << "    ld.global.f32 " << getReg(instr.result) << ", [" << getReg(instr.arg2) << "];\n";
        }
        else if (instr.op == "store") {
            cout << "    // Arch: In reality, we'd cvt.u64.f32 to %rd here for memory offset\n";
            cout << "    st.global.f32 [" << getReg(instr.arg2) << "], " << getReg(instr.arg1) << ";\n";
        }
        else if (instr.op == "add") cout << "    add.f32 " << getReg(instr.result) << ", " << getReg(instr.arg1) << ", " << getReg(instr.arg2) << ";\n";
        else if (instr.op == "sub") cout << "    sub.f32 " << getReg(instr.result) << ", " << getReg(instr.arg1) << ", " << getReg(instr.arg2) << ";\n";
        else if (instr.op == "mul") cout << "    mul.f32 " << getReg(instr.result) << ", " << getReg(instr.arg1) << ", " << getReg(instr.arg2) << ";\n";
        else if (instr.op == "div") cout << "    div.f32 " << getReg(instr.result) << ", " << getReg(instr.arg1) << ", " << getReg(instr.arg2) << ";\n";
        else if (instr.op == "sync") cout << "    bar.sync 0;\n";
    }

    cout << "    ret;\n}\n";
}