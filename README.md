# riscv-feature-sandbox
An experimental RISC-V emulator focused on comparing execution results by enabling/disabling specific instruction features.

## User's Manual

### Prerequisites
* A C++17 compatible compiler (e.g., GCC, Clang)
* CMake 3.10 or higher
* Make

### Cloning the Repository
This project uses the RISC-V GNU toolchain as a submodule. Be sure to clone it with the `--recursive` flag, or initialize the submodules after cloning:
```bash
# Clone with submodules
git clone --recursive https://github.com/kmoritak/riscv-feature-sandbox.git
cd riscv-feature-sandbox

# If you already cloned without submodules:
git submodule update --init --recursive
```

### Compiling the Simulator
To build the `riscv_simulator` executable, use CMake:
```bash
mkdir build
cd build
cmake ..
make
```

### Running the Simulator
Once built, you can run the simulator.

**Running an ELF file:**
The simulator requires exactly one argument: the path to a 32-bit RISC-V ELF file. It will load the executable segments into memory, set the PC to the entry point, and begin execution. The simulator will output a rich execution trace for debugging.
```bash
./riscv_simulator path/to/your/program.elf
```

To compile a test program for the simulator using the included GNU toolchain (once built):
```bash
riscv32-unknown-elf-gcc -nostdlib -nostartfiles -Ttext=0x10000 test.s -o test.elf
```
