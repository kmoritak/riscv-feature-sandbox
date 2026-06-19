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

**Running the built-in dummy program:**
If you execute the simulator without any arguments, it will automatically run a built-in test program that executes a few simple instructions and prints the final state of the registers.
```bash
./riscv_simulator
```

**Running an external binary:**
You can also provide a raw binary file to load into the simulator's memory and execute:
```bash
./riscv_simulator path/to/your/program.bin
```
