#pragma once

#include "bytefile.h"
#include "error.h"
#include "opcode.h"
#include "instructions.h"

#include <cstddef>
#include <memory>
#include <optional>
#include <vector>

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

        case Opcode_String: {
            return std::make_unique<String>(read_string());
        }

        case Opcode_SExp: {
            return std::make_unique<SExpression>(read_string(), read_int());
        }

        case Opcode_StI: {
            return std::make_unique<StoreStack>();
        }

        case Opcode_StA: {
            return std::make_unique<StoreArray>();
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

        case Opcode_Swap: {
            return std::make_unique<Swap>();
        }

        case Opcode_Elem: {
            return std::make_unique<Elem>();
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

        case Opcode_CBegin: {
            return std::make_unique<Begin>(read_int(), read_int());
            break;
        }

        case Opcode_Closure: {
            int entry = read_int();
            std::vector<LocationEntry> captured;
            {
                int n = read_int();
                for (int i = 0; i < n; i++) {
                    captured.push_back(read_loc());
                }
            }
            return std::make_unique<Closure>(entry, std::move(captured));
        }

        case Opcode_CallC: {
            return std::make_unique<CallClosure>(read_int());
        }

        case Opcode_Call: {
            return std::make_unique<Call>(read_int(), read_int());
        }

        case Opcode_Tag: {
            return std::make_unique<Tag>(read_string(), read_int());
        }

        case Opcode_Array: {
            return std::make_unique<Array>(read_int());
        }

        case Opcode_Fail: {
            return std::make_unique<Fail>(read_int(), read_int());
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
                return std::make_unique<Load>(read_int(), l);
            }
            case HOpcode_LdA: {
                return std::make_unique<LoadArray>(read_int(), l);
            }
            case HOpcode_St: {
                return std::make_unique<Store>(LocationEntry{(Location) read_int(), l});
            }

            case HOpcode_Patt: {
                return std::make_unique<PatternInst>(l);
            }

            case HOpcode_LCall: {
                switch (l) {
                case LCall_Lread: {
                    return std::make_unique<BuiltinRead>();
                }
                case LCall_Lwrite: {
                    return std::make_unique<BuiltinWrite>();
                }
                case LCall_Llength: {
                    return std::make_unique<BuiltinLength>();
                }
                case LCall_Lstring: {
                    return std::make_unique<BuiltinString>();
                }
                case LCall_Barray: {
                    return std::make_unique<BuiltinArray>();
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
