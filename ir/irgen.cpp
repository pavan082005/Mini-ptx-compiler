#include "irgen.h"
#include <bits/stdc++.h>
using namespace std;
IRGenerator::IRGenerator() {
    tempCount = 1;
}

string IRGenerator::newTemp() {
    return "t" + std::to_string(tempCount++);
}