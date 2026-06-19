#include "memory.h"
#include "cpu.h"
#include "elf_loader.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <elf_file> [--isa=<ISA>]" << std::endl;
        return 1;
    }

    std::string elf_file = argv[1];
    std::string isa = "RV32I"; // Default ISA

    // Parse CLI arguments
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.rfind("--isa=", 0) == 0) {
            isa = arg.substr(6);
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 1;
        }
    }

    // Initialize 16MB of memory
    Memory mem(16 * 1024 * 1024);
    CPU cpu(mem, isa);

    uint32_t entry_point = 0;
    if (!ELFLoader::load(elf_file, mem, entry_point, isa)) {
        return 1;
    }

    cpu.set_pc(entry_point);

    // Run the CPU
    std::cout << "Starting simulation..." << std::endl;

    // Run until PC is 0 (halted) or out of bounds
    // Start at PC=0 by stepping once and then evaluating condition, or just loop
    do {
        cpu.step();
    } while (cpu.get_pc() != 0 && cpu.get_pc() < mem.get_data().size());

    std::cout << "Simulation finished." << std::endl;
    std::cout << "Registers state:" << std::endl;
    for (int i = 0; i < 32; ++i) {
        std::cout << "x" << i << ": " << cpu.get_reg(i) << std::endl;
    }

    return 0;
}
