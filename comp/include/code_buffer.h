#pragma once
#include <string>
#include <vector>

#include "symb_stack.h"
#include "register.h"
namespace lama::rv {

    class CodeBuffer {
        private:
        std::vector<std::string> _code;
        public:
        CodeBuffer() {}

        std::string dump_asm() {
            std::string s;
            s.reserve(1 << 14);
            for (const std::string& str : _code) {
                s += str + "\n";
            }
            return s + "ret\n";
        }

        using SymbolicLocation = SymbolicStack::SymbolicLocation;

        // insn reg_dest, reg_src1, reg_src1
        #define R_TYPE(rv_insn) \
            void emit_ ## rv_insn (Register dst, Register src1, Register src2) { \
                emit_r_type(#rv_insn, dst, src1, src2); \
            }

        // insn reg_dest, reg_src, immediate
        #define I_TYPE(rv_insn) \
            void emit_ ## rv_insn (Register dst, Register src, int imm) { \
                emit_i_type(#rv_insn, dst, src, imm); \
            }

        // insn reg_dest, base_reg(immediate)
        #define S_TYPE(rv_insn) \
            void emit_ ## rv_insn (Register dst, Register base, int off) { \
                emit_s_type(#rv_insn, dst, base, off); \
            }

        // insn reg_dest, immediate
        #define U_TYPE(rv_insn) \
            void emit_ ## rv_insn (Register dst, int imm) { \
                emit_u_type(#rv_insn, dst, imm); \
            }

        R_TYPE(add);
        R_TYPE(sub);
        R_TYPE(slt); // set less than (1 if src1 < src2, 0 otherwise)
        R_TYPE(or);
        R_TYPE(and);
        R_TYPE(srl); // logical
        R_TYPE(sra); // arithmetical
        R_TYPE(sll);
        R_TYPE(mul);
        R_TYPE(div);

        I_TYPE(addi);
        I_TYPE(slti);
        I_TYPE(ori);
        I_TYPE(xori);
        I_TYPE(andi);


        S_TYPE(ld);
        S_TYPE(sd);

        // labels???
        // (beq);
        // (bne);
        // (blt);
        // (bge);
        // (jal)
        // (jalr)

        // Load immediate
        U_TYPE(li);

        void emit_mv(Register dst, Register src) {
            emit_addi(dst, src, 0);
        }

        void emit_call(std::string label) {
            emit("call\t" + label);
        }

        private:
        void emit_r_type(const std::string& insn, Register dst, Register src1, Register src2) {
            emit(insn + "\t" + dst.to_string() + ",\t" \
                                + src1.to_string() + ",\t" \
                                + src2.to_string());
        }

        void emit_i_type(const std::string& insn, Register dst, Register src, int imm) {
            emit(insn + "\t" + dst.to_string() + ",\t" \
                            + src.to_string() + ",\t" \
                            + std::to_string(imm));
        }

        void emit_u_type(const std::string& insn, Register dst, int imm) {
            emit(insn + "\t" + dst.to_string() + ",\t" + std::to_string(imm));
        }

        void emit_s_type(const std::string& insn, Register dst, Register base, int off) {
            emit(insn + "\t" + dst.to_string() + ",\t" + std::to_string(off) + "(" + base.to_string() + ")");
        }

        inline Register to_reg(SymbolicLocation loc, Register temp) {
            return loc.type == SymbolicStack::SymbolicLocationType::Memory
                ? emit_ld(temp, rv::Register::base(), -loc.number), temp
                : Register{loc.number};
        }

        // TODO: May be convinient to have emit_insns, that takes any SymbolicLocation (not only Register)

        void emit(std::string str) {
            _code.emplace_back(str);
        }
    };

}
