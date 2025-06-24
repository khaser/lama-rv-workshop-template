#pragma once

#include "symb_stack.h"
#include "code_buffer.h"
#include <string>
#include <vector>

namespace lama::rv {

    class Compiler {
        public:
        SymbolicStack st;
        CodeBuffer cb;
        std::vector<const char *> strs;
        std::vector<const char *> globals;

        std::string dump_asm() {
            std::string res;
            res += ".section custom_data,\"aw\",@progbits\n";
            res += ".fill 128, 8, 1\n";
            res += ".data\n";
            res += dump_global_section();
            res += ".text\n";
            res += ".global main\n";
            res += "main:\n";
            res += cb.dump_asm();
            return res;
        }

        private:
        std::string dump_global_section() {
            std::string res;
            for (const char* glob : globals) {
                res += "global_" + std::string(glob) + ":\t.quad 0\n";
            }
            return res;
        }
    };
}
