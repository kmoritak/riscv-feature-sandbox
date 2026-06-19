#include "elf_loader.h"
#include <elf.h>
#include <fstream>
#include <iostream>
#include <cstring>

bool ELFLoader::load(const std::string& filename, Memory& mem, uint32_t& entry_point, const std::string& expected_isa) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open ELF file " << filename << std::endl;
        return false;
    }

    // Read ELF Header
    Elf32_Ehdr ehdr;
    if (!file.read(reinterpret_cast<char*>(&ehdr), sizeof(Elf32_Ehdr))) {
        std::cerr << "Error: Could not read ELF header" << std::endl;
        return false;
    }

    // Verify magic number
    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
        std::cerr << "Error: Not a valid ELF file" << std::endl;
        return false;
    }

    // Check basic architecture match for ISA
    if (expected_isa.find("RV32") != std::string::npos) {
        if (ehdr.e_ident[EI_CLASS] != ELFCLASS32) {
            std::cerr << "Error: Architecture mismatch. Expected 32-bit ELF for ISA " << expected_isa << std::endl;
            return false;
        }
    } else if (expected_isa.find("RV64") != std::string::npos) {
        if (ehdr.e_ident[EI_CLASS] != ELFCLASS64) {
            std::cerr << "Error: Architecture mismatch. Expected 64-bit ELF for ISA " << expected_isa << std::endl;
            return false;
        }
        std::cerr << "Error: Simulator currently only supports RV32I. Cannot load 64-bit ELF." << std::endl;
        return false;
    }

    if (ehdr.e_machine != EM_RISCV) {
        std::cerr << "Error: Not a RISC-V ELF file" << std::endl;
        return false;
    }

    entry_point = ehdr.e_entry;

    // Read Program Headers
    std::vector<Elf32_Phdr> phdrs(ehdr.e_phnum);
    file.seekg(ehdr.e_phoff, std::ios::beg);
    if (!file.read(reinterpret_cast<char*>(phdrs.data()), ehdr.e_phnum * sizeof(Elf32_Phdr))) {
        std::cerr << "Error: Could not read program headers" << std::endl;
        return false;
    }

    // Load segments
    for (const auto& phdr : phdrs) {
        if (phdr.p_type == PT_LOAD && phdr.p_memsz > 0) {
            std::cout << "Loading segment: vaddr=0x" << std::hex << phdr.p_vaddr
                      << " filesz=0x" << phdr.p_filesz
                      << " memsz=0x" << phdr.p_memsz << std::dec << std::endl;

            // Simple bounds check based on our memory size
            if (phdr.p_vaddr + phdr.p_memsz > mem.get_data().size()) {
                 std::cerr << "Error: Segment loads beyond physical memory capacity." << std::endl;
                 return false;
            }

            file.seekg(phdr.p_offset, std::ios::beg);

            // Read data into memory
            if (phdr.p_filesz > 0) {
                file.read(reinterpret_cast<char*>(&mem.get_data()[phdr.p_vaddr]), phdr.p_filesz);
            }

            // Zero-pad BSS segment
            if (phdr.p_memsz > phdr.p_filesz) {
                std::memset(&mem.get_data()[phdr.p_vaddr + phdr.p_filesz], 0, phdr.p_memsz - phdr.p_filesz);
            }
        }
    }

    return true;
}
