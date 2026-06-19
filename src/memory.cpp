#include "memory.h"
#include <stdexcept>

Memory::Memory(size_t size) : data(size, 0) {}

uint8_t Memory::read8(uint32_t addr) const {
    if (addr >= data.size()) {
        throw std::out_of_range("Memory read out of bounds");
    }
    return data[addr];
}

uint16_t Memory::read16(uint32_t addr) const {
    if (addr + 1 >= data.size()) {
        throw std::out_of_range("Memory read out of bounds");
    }
    return static_cast<uint16_t>(data[addr]) | (static_cast<uint16_t>(data[addr + 1]) << 8);
}

uint32_t Memory::read32(uint32_t addr) const {
    if (addr + 3 >= data.size()) {
        throw std::out_of_range("Memory read out of bounds");
    }
    return static_cast<uint32_t>(data[addr]) |
           (static_cast<uint32_t>(data[addr + 1]) << 8) |
           (static_cast<uint32_t>(data[addr + 2]) << 16) |
           (static_cast<uint32_t>(data[addr + 3]) << 24);
}

void Memory::write8(uint32_t addr, uint8_t val) {
    if (addr >= data.size()) {
        throw std::out_of_range("Memory write out of bounds");
    }
    data[addr] = val;
}

void Memory::write16(uint32_t addr, uint16_t val) {
    if (addr + 1 >= data.size()) {
        throw std::out_of_range("Memory write out of bounds");
    }
    data[addr] = static_cast<uint8_t>(val & 0xFF);
    data[addr + 1] = static_cast<uint8_t>((val >> 8) & 0xFF);
}

void Memory::write32(uint32_t addr, uint32_t val) {
    if (addr + 3 >= data.size()) {
        throw std::out_of_range("Memory write out of bounds");
    }
    data[addr] = static_cast<uint8_t>(val & 0xFF);
    data[addr + 1] = static_cast<uint8_t>((val >> 8) & 0xFF);
    data[addr + 2] = static_cast<uint8_t>((val >> 16) & 0xFF);
    data[addr + 3] = static_cast<uint8_t>((val >> 24) & 0xFF);
}
