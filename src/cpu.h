#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <array>
#include <string>
#include "memory.h"

class CPU {
public:
    CPU(Memory& memory, const std::string& isa);

    void reset();
    void step();

    void set_pc(uint32_t new_pc) { pc = new_pc; }
    uint32_t get_pc() const { return pc; }
    uint32_t get_reg(uint32_t i) const { return (i == 0) ? 0 : regs[i]; }

private:
    uint32_t pc;
    std::array<uint32_t, 32> regs;
    Memory& mem;
    std::string current_isa;

    void execute(uint32_t inst);
};

#endif // CPU_H
