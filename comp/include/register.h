#pragma once
#include <cstddef>


namespace lama::rv {
    struct Register {
        size_t regno;
        public:

        static Register temp1() {return {30};}
        static Register temp2() {return {31};}
        static Register base() {return {8};}
        static Register arg(size_t argno) {return{(size_t) (10 + argno)};}

        const char* to_string() {
            constexpr static const char *reg_names[] = {
                "zero",
                "ra",
                "sp",
                "gp",
                "tp",
                "t0",
                "t1",
                "t2",
                "fp",
                "s1",
                "a0",
                "a1",
                "a2",
                "a3",
                "a4",
                "a5",
                "a6",
                "a7",
                "s2",
                "s3",
                "s4",
                "s5",
                "s6",
                "s7",
                "s8",
                "s9",
                "s10",
                "s11",
                "t3",
                "t4",
                "t5",
                "t6"
            };
            return reg_names[regno];
        }
    };

}
