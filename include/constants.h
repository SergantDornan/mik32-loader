#include "BuilderFilework.h"
const std::string compileFlags = "-march=rv32imc_zicsr_zifencei -mabi=ilp32 -mcmodel=medlow -g3 -Wall -fsigned-char -ffunction-sections -DMIK32V2";
const std::string linkFlags = "-march=rv32imc_zicsr_zifencei -mabi=ilp32 -mcmodel=medlow -g3 -Wall -fsigned-char -ffunction-sections -DMIK32V2 -Wl,-Teeprom.ld -Xlinker --gc-sections -nostartfiles -L" + root + "/ldscripts";
const std::string elfFileName = "output.elf";
const std::string hexFileName = "output.hex";
const std::string Cstandart  = "-std=gnu11";
const std::string Cppstandart = "-std=gnu++17";
const std::string picocomFlags = "--omap crcrlf --echo";
const std::vector<std::string> IncFolders = {root + "/hardware/mik32-hal/core/Include",
root + "/hardware/mik32-hal/peripherals/Include", root + "/hardware/mik32-hal/utilities/Include",
root + "/hardware/mik32v2-shared/include", root + "/hardware/mik32v2-shared/libs",
root + "/hardware/mik32v2-shared/periphery", root + "/CompiledLibs"};
const std::vector<std::string> possibleFlags = {"-b", "-p", "--CC", "--CXX", "--ASM"}; 
const std::vector<std::string> Compilers = {
	getHomedir() + "/xpack-riscv-none-elf-gcc-14.2.0-3/bin/riscv-none-elf-gcc",
	getHomedir() + "/xpack-riscv-none-elf-gcc-14.2.0-3/bin/riscv-none-elf-g++",
	getHomedir() + "/xpack-riscv-none-elf-gcc-14.2.0-3/bin/riscv-none-elf-objcopy",
	getHomedir() + "/xpack-riscv-none-elf-gcc-14.2.0-3/bin/riscv-none-elf-as"};