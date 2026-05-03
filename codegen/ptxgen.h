#ifndef PTXGEN_H
#define PTXGEN_H

#include <vector>
#include "../ir/ir.h"

class PTXGenerator {
public:
    void generate(const std::vector<IR>& code);
};

#endif