#include "memory.h"
#include "cpu.h"
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [binary_file]" << std::endl;
        return 1;
    }

    // Initialize 1MB of memory
    Memory mem(1024 * 1024);
    CPU cpu(mem);

    if (argc == 2) {
        // Load binary file
        std::ifstream file(argv[1], std::ios::binary);
        if (!file) {
            std::cerr << "Error opening file: " << argv[1] << std::endl;
            return 1;
        }

        file.seekg(0, std::ios::end);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        if (size > mem.get_data().size()) {
            std::cerr << "Error: File too large for memory." << std::endl;
            return 1;
        }

        if (!file.read(reinterpret_cast<char*>(mem.get_data().data()), size)) {
            std::cerr << "Error reading file." << std::endl;
            return 1;
        }
    } else {
        // Load a simple test program into memory
        // addi x1, x0, 5   (00500093)
        // addi x2, x0, 10  (00a00113)
        // add  x3, x1, x2  (002081b3)
        mem.write32(0, 0x00500093);
        mem.write32(4, 0x00a00113);
        mem.write32(8, 0x002081b3);
        // ecall (Halt)
        mem.write32(12, 0x00000073);
    }

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
