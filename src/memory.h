#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <cstdint>

class Memory {
public:
    Memory(size_t size);

    uint8_t read8(uint32_t addr) const;
    uint16_t read16(uint32_t addr) const;
    uint32_t read32(uint32_t addr) const;

    void write8(uint32_t addr, uint8_t val);
    void write16(uint32_t addr, uint16_t val);
    void write32(uint32_t addr, uint32_t val);

    // Expose memory for loading programs
    std::vector<uint8_t>& get_data() { return data; }

private:
    std::vector<uint8_t> data;
};

#endif // MEMORY_H
