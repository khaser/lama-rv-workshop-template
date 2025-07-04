#pragma once

#include "bytefile.h"
#include "error.h"
#include "opcode.h"
#include "instructions.h"

#include <memory>

namespace lama {

class InstReader {
public:
    InstReader(const bytefile *file) : file(file), ip(file->code_ptr) {}

private:
    const bytefile *file;
    const char *ip;

    inline void assert_can_read(int bytes) {
        ASSERT(file->code_ptr <= ip, 1,
               "ip is out of code section");
        ASSERT(ip + bytes <= (const char *)file + file->size, 1,
               "ip is out of code section");
    }

    inline char read_byte() {
        assert_can_read(1);
        ip += sizeof(char);
        return *(ip - sizeof(char));
    }

    inline int read_int() {
        assert_can_read(4);
        ip += sizeof(int);
        return *(int *)(ip - sizeof(int));
    }

    inline const char *read_string() {
        return get_string(file, read_int());
    }

    inline LocationEntry read_loc() {
        Location kind = (Location)read_byte();
        int index = read_int();
        return LocationEntry{kind, index};
    }

public:
    inline std::unique_ptr<Instruction> read_inst() {
        unsigned char x = read_byte(),
                      h = (x & 0xF0) >> 4,
                      l = x & 0x0F;

        switch (x) {
        case Opcode_Const: {
            return std::make_unique<Const>(read_int());
        }

        case Opcode_StI: {
            return std::make_unique<StoreStack>();
        }

        case Opcode_Jmp: {
            return std::make_unique<Jump>(read_int());
        }

        case Opcode_End: {
            return std::make_unique<End>();
        }

        case Opcode_Ret: {
            return std::make_unique<Return>();
        }

        case Opcode_Drop: {
            return std::make_unique<Drop>();
        }

        case Opcode_Dup: {
            return std::make_unique<Duplicate>();
        }

        case Opcode_CJmpZ: {
            return std::make_unique<ConditionalJump>(read_int(), true);
        }

        case Opcode_CJmpNZ: {
            return std::make_unique<ConditionalJump>(read_int(), false);
        }

        case Opcode_Begin: {
            return std::make_unique<Begin>(read_int(), read_int());
            break;
        }

        case Opcode_Call: {
            return std::make_unique<Call>(read_int(), read_int());
        }

        case Opcode_Line: {
            return std::make_unique<Line>(read_int());
        }

        default:

            switch (h) {
            case HOpcode_Binop: {
                return std::make_unique<Binop>(l);
            }

            case HOpcode_Ld: {
                return std::make_unique<Load>(LocationEntry{(Location) l, read_int()});
            }

            case HOpcode_St: {
                return std::make_unique<Store>(LocationEntry{(Location) l, read_int()});
            }

            case HOpcode_LCall: {
                switch (l) {
                case LCall_Lread: {
                    return std::make_unique<BuiltinRead>();
                }
                case LCall_Lwrite: {
                    return std::make_unique<BuiltinWrite>();
                }
                default:
                    FAIL(1, "Unknown LCall");
                }
                break;
            }
            case HOpcode_Stop: {
                return nullptr;
            }
            default:
                FAIL(1, "Unknown opcode %d\n", x);
            }
        }

    }
};

}
