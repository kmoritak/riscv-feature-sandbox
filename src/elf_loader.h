#ifndef ELF_LOADER_H
#define ELF_LOADER_H

#include <string>
#include <vector>
#include <cstdint>
#include "memory.h"

class ELFLoader {
public:
    static bool load(const std::string& filename, Memory& mem, uint32_t& entry_point);
};

#endif // ELF_LOADER_H
