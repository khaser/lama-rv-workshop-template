#pragma once
#include <cstddef>
namespace lama::rv {

    class SymbolicStack {
        public:
        enum class SymbolicLocationType {
            Register, Memory
        };

        struct SymbolicLocation {
            SymbolicLocationType type;
            size_t number;
        };

        constexpr static size_t regs[] = {9, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 5, 6, 7, 28, 29};

        size_t top;
        SymbolicStack() : top(0) {}

        SymbolicLocation alloc() {
            // TODO: shift for locals
        }

        void push(SymbolicLocation loc) {
            top++;
        }

        SymbolicLocation pop() {
            // TODO
        }
    };
}
