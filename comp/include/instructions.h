#pragma once
#include "instruction.h"
#include "opcode.h"
#include <cassert>
#include <cstdint>

namespace lama {

#define INSTRUCTIONS(MACRO) \
    MACRO(Const)            \
    MACRO(String)           \
    MACRO(SExpression)      \
    MACRO(StoreStack)       \
    MACRO(StoreArray)       \
    MACRO(Jump)             \
    MACRO(ConditionalJump)  \
    MACRO(Return)           \
    MACRO(Drop)             \
    MACRO(Duplicate)        \
    MACRO(Swap)             \
    MACRO(Elem)             \
    MACRO(Closure)          \
    MACRO(Begin)            \
    MACRO(End)              \
    MACRO(CallClosure)      \
    MACRO(Call)             \
    MACRO(Tag)              \
    MACRO(Array)            \
    MACRO(Fail)             \
    MACRO(Line)             \
    MACRO(Binop)            \
    MACRO(Load)             \
    MACRO(LoadArray)        \
    MACRO(Store)            \
    MACRO(PatternInst)      \
    MACRO(BuiltinRead)      \
    MACRO(BuiltinWrite)     \
    MACRO(BuiltinLength)    \
    MACRO(BuiltinString)    \
    MACRO(BuiltinArray)


#define BASE(class_name, super_class_name)       \
  class class_name: public super_class_name {    \


#define LEAF(class_name, super_class_name)       \
  BASE(class_name, super_class_name)             \
   public:                                       \
    virtual const char* name() const             { return #class_name; }       \

using SymbolicLocationType = rv::SymbolicStack::SymbolicLocationType;

LEAF(Const, Instruction)
private:
    int _value;

public:
    Const(int value) : _value(value) {}

    inline int value() { return _value; }

    // Instruction implementation example
    void emit_code(rv::Compiler *c) override {
        auto loc = c->st.alloc();
        switch (loc.type) {
            case SymbolicLocationType::Memory: {
                c->cb.emit_li(rv::Register::temp1(), _value);
                c->cb.emit_sd(rv::Register::temp1(), rv::Register::base(), -loc.number);
                return;
            }
            case SymbolicLocationType::Register: {
                c->cb.emit_li({(uint8_t) loc.number}, _value);
                return;
            }
        }
    }
};

LEAF(StoreStack, Instruction)

public:
    void emit_code(rv::Compiler *c) override {
        // TODO
    }
};
LEAF(End, Instruction) };
LEAF(Return, Instruction) };
LEAF(Duplicate, Instruction) };
LEAF(Drop, Instruction) };
LEAF(Jump, Instruction)
private:
    size_t _offset;

public:
    Jump(int offset) : _offset(offset) {}

    void emit_code(rv::Compiler *c) override {
        // TODO
    }
};

LEAF(ConditionalJump, Instruction)
private:
    size_t _offset;
    bool _zero;

public:
    ConditionalJump(int offset, bool zero) : _offset(offset), _zero(zero) {}

    void emit_code(rv::Compiler *c) override {
        // TODO
    }
};

LEAF(Begin, Instruction)
private:
    size_t _argc, _locc;
public:
    Begin(int argc, int locc) : _argc(argc), _locc(locc) {}
};

LEAF(Call, Instruction)
private:
    size_t  _offset, _argc;
public:
    Call(int offset, int argc) : _offset(offset), _argc(argc) {}
};

LEAF(Line, Instruction)
private:
    size_t _line;
public:
    Line(int line) : _line(line) {}
};

LEAF(Binop, Instruction)
private:
    size_t _op;
public:
    Binop(int op) : _op(op) {}

    void emit_code(rv::Compiler *c) override {
        // TODO
    }
};

LEAF(Load, Instruction)
private:
    size_t _index;
    Location _loc;
public:
    Load(int index, int location) : _index(index), _loc((Location)location) {}

    void emit_code(rv::Compiler *c) override {
        // TODO
    }
};

LEAF(Store, Instruction)
private:
    LocationEntry _loc;
public:
    Store(LocationEntry loc) : _loc(loc) {}

    void emit_code(rv::Compiler *c) override {
        // TODO
    }
};

LEAF(BuiltinRead, Instruction) };
LEAF(BuiltinWrite, Instruction) };
LEAF(BuiltinLength, Instruction) };
LEAF(BuiltinString, Instruction) };
LEAF(BuiltinArray, Instruction) };

}
