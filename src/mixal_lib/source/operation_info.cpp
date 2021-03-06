#include <mixal/operation_info.h>
#include <mixal/exceptions.h>

#include <mix/word.h>
#include <mix/command.h>

#include <core/utils.h>

#include <cassert>

namespace mixal {
namespace {

struct Data
{
    const OperationId id;
    const int command_id;
    const WordField field;
};

// Commands with same id should be grouped together
// to simplify QueryOperationInfo() search
const Data k_operations_info[] =
{
    {OperationId::NOP,   0,  WordField::FromByte(0)},
    {OperationId::ADD,   1,  Word::MaxField()},
    {OperationId::FADD,  1,  WordField::FromByte(6)},
    {OperationId::SUB,   2,  Word::MaxField()},
    {OperationId::FSUB,  2,  WordField::FromByte(6)},
    {OperationId::MUL,   3,  Word::MaxField()},
    {OperationId::FMUL,  3,  WordField::FromByte(6)},
    {OperationId::DIV,   4,  Word::MaxField()},
    {OperationId::FDIV,  4,  WordField::FromByte(6)},
    {OperationId::NUM,   5,  WordField::FromByte(0)},
    {OperationId::CHAR,  5,  WordField::FromByte(1)},
    {OperationId::HLT,   5,  WordField::FromByte(2)},
    {OperationId::SLA,   6,  WordField::FromByte(0)},
    {OperationId::SRA,   6,  WordField::FromByte(1)},
    {OperationId::SLAX,  6,  WordField::FromByte(2)},
    {OperationId::SRAX,  6,  WordField::FromByte(3)},
    {OperationId::SLC,   6,  WordField::FromByte(4)},
    {OperationId::SRC,   6,  WordField::FromByte(5)},
    {OperationId::MOVE,  7,  WordField::FromByte(1)},
    {OperationId::LDA,   8,  Word::MaxField()},
    {OperationId::LD1,   9,  Word::MaxField()},
    {OperationId::LD2,   10, Word::MaxField()},
    {OperationId::LD3,   11, Word::MaxField()},
    {OperationId::LD4,   12, Word::MaxField()},
    {OperationId::LD5,   13, Word::MaxField()},
    {OperationId::LD6,   14, Word::MaxField()},
    {OperationId::LDX,   15, Word::MaxField()},
    {OperationId::LDAN,  16, Word::MaxField()},
    {OperationId::LD1N,  17, Word::MaxField()},
    {OperationId::LD2N,  18, Word::MaxField()},
    {OperationId::LD3N,  19, Word::MaxField()},
    {OperationId::LD4N,  20, Word::MaxField()},
    {OperationId::LD5N,  21, Word::MaxField()},
    {OperationId::LD6N,  22, Word::MaxField()},
    {OperationId::LDXN,  23, Word::MaxField()},
    {OperationId::STA,   24, Word::MaxField()},
    {OperationId::ST1,   25, Word::MaxField()},
    {OperationId::ST2,   26, Word::MaxField()},
    {OperationId::ST3,   27, Word::MaxField()},
    {OperationId::ST4,   28, Word::MaxField()},
    {OperationId::ST5,   29, Word::MaxField()},
    {OperationId::ST6,   30, Word::MaxField()},
    {OperationId::STX,   31, Word::MaxField()},
    {OperationId::STJ,   32, WordField{0, 2}},
    {OperationId::STZ,   33, Word::MaxField()},
    {OperationId::JBUS,  34, WordField::FromByte(0)},
    {OperationId::IOC,   35, WordField::FromByte(0)},
    {OperationId::IN_,    36, WordField::FromByte(0)},
    {OperationId::OUT_,   37, WordField::FromByte(0)},
    {OperationId::JRED,  38, WordField::FromByte(0)},
    {OperationId::JMP,   39, WordField::FromByte(0)},
    {OperationId::JSJ,   39, WordField::FromByte(1)},
    {OperationId::JOV,   39, WordField::FromByte(2)},
    {OperationId::JNOV,  39, WordField::FromByte(3)},
    {OperationId::JL,    39, WordField::FromByte(4)},
    {OperationId::JE,    39, WordField::FromByte(5)},
    {OperationId::JG,    39, WordField::FromByte(6)},
    {OperationId::JGE,   39, WordField::FromByte(7)},
    {OperationId::JNE,   39, WordField::FromByte(8)},
    {OperationId::JLE,   39, WordField::FromByte(9)},
    {OperationId::JAN,   40, WordField::FromByte(0)},
    {OperationId::JAZ,   40, WordField::FromByte(1)},
    {OperationId::JAP,   40, WordField::FromByte(2)},
    {OperationId::JANN,  40, WordField::FromByte(3)},
    {OperationId::JANZ,  40, WordField::FromByte(4)},
    {OperationId::JANP,  40, WordField::FromByte(5)},
    {OperationId::J1N,   41, WordField::FromByte(0)},
    {OperationId::J1Z,   41, WordField::FromByte(1)},
    {OperationId::J1P,   41, WordField::FromByte(2)},
    {OperationId::J1NN,  41, WordField::FromByte(3)},
    {OperationId::J1NZ,  41, WordField::FromByte(4)},
    {OperationId::J1NP,  41, WordField::FromByte(5)},
    {OperationId::J2N,   42, WordField::FromByte(0)},
    {OperationId::J2Z,   42, WordField::FromByte(1)},
    {OperationId::J2P,   42, WordField::FromByte(2)},
    {OperationId::J2NN,  42, WordField::FromByte(3)},
    {OperationId::J2NZ,  42, WordField::FromByte(4)},
    {OperationId::J2NP,  42, WordField::FromByte(5)},
    {OperationId::J3N,   43, WordField::FromByte(0)},
    {OperationId::J3Z,   43, WordField::FromByte(1)},
    {OperationId::J3P,   43, WordField::FromByte(2)},
    {OperationId::J3NN,  43, WordField::FromByte(3)},
    {OperationId::J3NZ,  43, WordField::FromByte(4)},
    {OperationId::J3NP,  43, WordField::FromByte(5)},
    {OperationId::J4N,   44, WordField::FromByte(0)},
    {OperationId::J4Z,   44, WordField::FromByte(1)},
    {OperationId::J4P,   44, WordField::FromByte(2)},
    {OperationId::J4NN,  44, WordField::FromByte(3)},
    {OperationId::J4NZ,  44, WordField::FromByte(4)},
    {OperationId::J4NP,  44, WordField::FromByte(5)},
    {OperationId::J5N,   45, WordField::FromByte(0)},
    {OperationId::J5Z,   45, WordField::FromByte(1)},
    {OperationId::J5P,   45, WordField::FromByte(2)},
    {OperationId::J5NN,  45, WordField::FromByte(3)},
    {OperationId::J5NZ,  45, WordField::FromByte(4)},
    {OperationId::J5NP,  45, WordField::FromByte(5)},
    {OperationId::J6N,   46, WordField::FromByte(0)},
    {OperationId::J6Z,   46, WordField::FromByte(1)},
    {OperationId::J6P,   46, WordField::FromByte(2)},
    {OperationId::J6NN,  46, WordField::FromByte(3)},
    {OperationId::J6NZ,  46, WordField::FromByte(4)},
    {OperationId::J6NP,  46, WordField::FromByte(5)},
    {OperationId::JXN,   47, WordField::FromByte(0)},
    {OperationId::JXZ,   47, WordField::FromByte(1)},
    {OperationId::JXP,   47, WordField::FromByte(2)},
    {OperationId::JXNN,  47, WordField::FromByte(3)},
    {OperationId::JXNZ,  47, WordField::FromByte(4)},
    {OperationId::JXNP,  47, WordField::FromByte(5)},
    {OperationId::INCA,  48, WordField::FromByte(0)},
    {OperationId::DECA,  48, WordField::FromByte(1)},
    {OperationId::ENTA,  48, WordField::FromByte(2)},
    {OperationId::ENNA,  48, WordField::FromByte(3)},
    {OperationId::INC1,  49, WordField::FromByte(0)},
    {OperationId::DEC1,  49, WordField::FromByte(1)},
    {OperationId::ENT1,  49, WordField::FromByte(2)},
    {OperationId::ENN1,  49, WordField::FromByte(3)},
    {OperationId::INC2,  50, WordField::FromByte(0)},
    {OperationId::DEC2,  50, WordField::FromByte(1)},
    {OperationId::ENT2,  50, WordField::FromByte(2)},
    {OperationId::ENN2,  50, WordField::FromByte(3)},
    {OperationId::INC3,  51, WordField::FromByte(0)},
    {OperationId::DEC3,  51, WordField::FromByte(1)},
    {OperationId::ENT3,  51, WordField::FromByte(2)},
    {OperationId::ENN3,  51, WordField::FromByte(3)},
    {OperationId::INC4,  52, WordField::FromByte(0)},
    {OperationId::DEC4,  52, WordField::FromByte(1)},
    {OperationId::ENT4,  52, WordField::FromByte(2)},
    {OperationId::ENN4,  52, WordField::FromByte(3)},
    {OperationId::INC5,  53, WordField::FromByte(0)},
    {OperationId::DEC5,  53, WordField::FromByte(1)},
    {OperationId::ENT5,  53, WordField::FromByte(2)},
    {OperationId::ENN5,  53, WordField::FromByte(3)},
    {OperationId::INC6,  54, WordField::FromByte(0)},
    {OperationId::DEC6,  54, WordField::FromByte(1)},
    {OperationId::ENT6,  54, WordField::FromByte(2)},
    {OperationId::ENN6,  54, WordField::FromByte(3)},
    {OperationId::INCX,  55, WordField::FromByte(0)},
    {OperationId::DECX,  55, WordField::FromByte(1)},
    {OperationId::ENTX,  55, WordField::FromByte(2)},
    {OperationId::ENNX,  55, WordField::FromByte(3)},
    {OperationId::CMPA,  56, Word::MaxField()},
    {OperationId::FCMP,  56, WordField::FromByte(6)},
    {OperationId::CMP1,  57, Word::MaxField()},
    {OperationId::CMP2,  58, Word::MaxField()},
    {OperationId::CMP3,  59, Word::MaxField()},
    {OperationId::CMP4,  60, Word::MaxField()},
    {OperationId::CMP5,  61, Word::MaxField()},
    {OperationId::CMP6,  62, Word::MaxField()},
    {OperationId::CMPX,  63, Word::MaxField()},
    // Not implemented, unknown commands
    {OperationId::FIX,   -2, WordField::FromByte(0)},
    {OperationId::FLOT,  -2, WordField::FromByte(0)},
    {OperationId::SLB,   -2, WordField::FromByte(0)},
    {OperationId::SRB,   -2, WordField::FromByte(0)},
    {OperationId::JAE,   -2, WordField::FromByte(0)},
    {OperationId::JAO,   -2, WordField::FromByte(0)},
    {OperationId::JXE,   -2, WordField::FromByte(0)},
    {OperationId::JXO,   -2, WordField::FromByte(0)},
};

static_assert(core::ArraySize(k_operations_info) ==
    static_cast<std::size_t>(OperationId::MIXOpEnd) + 1,
        "Array does not match exact operation's count");

OperationInfo MakeInfo(const Data& data)
{
    OperationInfo info;
    info.id = data.id;
    info.computer_command = data.command_id;
    info.default_field = data.field;
    info.default_index = 0;
    return info;
}

} // namespace

OperationInfo QueryOperationInfo(const Operation& op)
{
    for (const Data& info : k_operations_info)
    {
        if (info.id == op.id())
        {
            return MakeInfo(info);
        }
    }
    throw InvalidOperationId(op.id());
}

OperationInfo QueryOperationInfo(const mix::Word& w)
{
    return QueryOperationInfo(mix::Command(w));
}

OperationInfo QueryOperationInfo(const mix::Command& cmd)
{
    const int id = static_cast<int>(cmd.id());
    const WordField field = cmd.word_field();

    const std::size_t count = core::ArraySize(k_operations_info);
    std::size_t start = count;
    for (std::size_t i = 0; i < count; ++i)
    {
        if (k_operations_info[i].command_id == id)
        {
            start = i;
            break;
        }
    }

    const Data* found = nullptr;
    // If there is only single command, return it.
    // Expects that commands with same id are grouped together.
    if ((start != count)
        && ((start + 1) < count)
        && (k_operations_info[start + 1].command_id != id))
    {
        found = &k_operations_info[start];
    }
    
    // Otherwise, search exact field match
    while (!found
        && (start < count)
        && (k_operations_info[start].command_id == id))
    {
        if (k_operations_info[start].field == field)
        {
            found = &k_operations_info[start];
            break;
        }
        ++start;
    }

    return found ? MakeInfo(*found) : OperationInfo{};
}

} // namespace mixal
