#include "cpu.h"
#include <iostream>
#include <iomanip>

CPU::CPU(Memory& memory) : pc(0), mem(memory) {
    regs.fill(0);
}

void CPU::reset() {
    pc = 0;
    regs.fill(0);
}

void CPU::step() {
    uint32_t inst = mem.read32(pc);
    pc += 4;
    execute(inst);
}

void CPU::execute(uint32_t inst) {
    uint32_t opcode = inst & 0x7F;
    uint32_t rd = (inst >> 7) & 0x1F;
    uint32_t funct3 = (inst >> 12) & 0x7;
    uint32_t rs1 = (inst >> 15) & 0x1F;
    uint32_t rs2 = (inst >> 20) & 0x1F;
    uint32_t funct7 = (inst >> 25) & 0x7F;

    switch (opcode) {
        case 0x13: // I-type (ADDI, SLTI, etc.)
        {
            int32_t imm = static_cast<int32_t>(inst) >> 20; // sign extension
            switch (funct3) {
                case 0x0: // ADDI
                    if (rd != 0) regs[rd] = regs[rs1] + imm;
                    break;
                // Add other I-type instructions here
                default:
                    std::cerr << "Unimplemented I-type funct3: " << funct3 << std::endl;
            }
            break;
        }
        case 0x33: // R-type (ADD, SUB, etc.)
        {
            switch (funct3) {
                case 0x0:
                    if (funct7 == 0x00) { // ADD
                        if (rd != 0) regs[rd] = regs[rs1] + regs[rs2];
                    } else if (funct7 == 0x20) { // SUB
                        if (rd != 0) regs[rd] = regs[rs1] - regs[rs2];
                    } else {
                        std::cerr << "Unimplemented R-type funct7: " << funct7 << std::endl;
                    }
                    break;
                // Add other R-type instructions here
                default:
                    std::cerr << "Unimplemented R-type funct3: " << funct3 << std::endl;
            }
            break;
        }
        case 0x37: // LUI
        {
            int32_t imm = inst & 0xFFFFF000;
            if (rd != 0) regs[rd] = imm;
            break;
        }
        case 0x17: // AUIPC
        {
            int32_t imm = inst & 0xFFFFF000;
            if (rd != 0) regs[rd] = (pc - 4) + imm;
            break;
        }
        case 0x6F: // JAL
        {
            // Sign extend the 21-bit immediate
            // shift arithmetic right from 31 to 20 maintains sign bits [31:21] -> [31:20]
            int32_t imm = (static_cast<int32_t>(inst & 0x80000000) >> 11) | // bit 20 (sign bit) extended
                          (((inst >> 12) & 0xFF) << 12) |
                          (((inst >> 20) & 0x1) << 11) |
                          (((inst >> 21) & 0x3FF) << 1);

            if (rd != 0) regs[rd] = pc;
            pc = (pc - 4) + imm;
            break;
        }
        case 0x67: // JALR
        {
            int32_t imm = static_cast<int32_t>(inst) >> 20;
            uint32_t t = pc;
            pc = (regs[rs1] + imm) & ~1;
            if (rd != 0) regs[rd] = t;
            break;
        }
        case 0x63: // B-type (BEQ, BNE, etc.)
        {
            int32_t imm = (static_cast<int32_t>(inst & 0x80000000) >> 19) | // bit 12 (sign bit) extended
                          (((inst >> 7) & 0x1) << 11) |
                          (((inst >> 25) & 0x3F) << 5) |
                          (((inst >> 8) & 0xF) << 1);
            switch (funct3) {
                case 0x0: // BEQ
                    if (regs[rs1] == regs[rs2]) pc = (pc - 4) + imm;
                    break;
                case 0x1: // BNE
                    if (regs[rs1] != regs[rs2]) pc = (pc - 4) + imm;
                    break;
                // Add other B-type instructions here
                default:
                    std::cerr << "Unimplemented B-type funct3: " << funct3 << std::endl;
            }
            break;
        }
        case 0x03: // I-type (Loads)
        {
            int32_t imm = static_cast<int32_t>(inst) >> 20;
            uint32_t addr = regs[rs1] + imm;
            switch (funct3) {
                case 0x0: // LB
                    if (rd != 0) regs[rd] = static_cast<int32_t>(static_cast<int8_t>(mem.read8(addr)));
                    break;
                case 0x1: // LH
                    if (rd != 0) regs[rd] = static_cast<int32_t>(static_cast<int16_t>(mem.read16(addr)));
                    break;
                case 0x2: // LW
                    if (rd != 0) regs[rd] = mem.read32(addr);
                    break;
                case 0x4: // LBU
                    if (rd != 0) regs[rd] = mem.read8(addr);
                    break;
                case 0x5: // LHU
                    if (rd != 0) regs[rd] = mem.read16(addr);
                    break;
                default:
                    std::cerr << "Unimplemented Load funct3: " << funct3 << std::endl;
            }
            break;
        }
        case 0x23: // S-type (Stores)
        {
            int32_t imm = ((inst >> 25) << 5) | ((inst >> 7) & 0x1F);
            // Sign extend the 12-bit immediate
            imm = (imm ^ 0x800) - 0x800;
            uint32_t addr = regs[rs1] + imm;
            switch (funct3) {
                case 0x0: // SB
                    mem.write8(addr, static_cast<uint8_t>(regs[rs2]));
                    break;
                case 0x1: // SH
                    mem.write16(addr, static_cast<uint16_t>(regs[rs2]));
                    break;
                case 0x2: // SW
                    mem.write32(addr, regs[rs2]);
                    break;
                default:
                    std::cerr << "Unimplemented Store funct3: " << funct3 << std::endl;
            }
            break;
        }
        case 0x73: // System (ECALL, EBREAK)
        {
            // Simple halt for ecall or ebreak
            std::cout << "ECALL/EBREAK encountered. Halting." << std::endl;
            pc = 0; // Trigger a halt condition in main loop
            break;
        }
        case 0x0: // Halt or padding
            std::cout << "Halt instruction (0x0) or uninitialized memory accessed. Halting." << std::endl;
            pc = 0; // Trigger a halt condition
            break;
        default:
            std::cerr << "Unknown opcode: 0x" << std::hex << opcode << std::endl;
    }
}
