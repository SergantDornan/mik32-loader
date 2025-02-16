#include "Flags.h"
#include "essentials.h"
const std::string compileFlags = "-march=rv32imc_zicsr_zifencei -mabi=ilp32 -mcmodel=medlow -g3 -Wall -fsigned-char -ffunction-sections -DMIK32V2";
const std::string linkFlags = "-march=rv32imc_zicsr_zifencei -mabi=ilp32 -mcmodel=medlow -g3 -Wall -fsigned-char -ffunction-sections -DMIK32V2 -Wl,-Teeprom.ld -Xlinker --gc-sections -nostartfiles -L" + root + "/ldscripts";
const std::string elfFileName = "output.elf";
const std::string hexFileName = "output.hex";
int main(int argc, char* argv[]){
	std::vector<std::string> args;
	for(int i = 1; i < argc; ++i)
		args.push_back(std::string(argv[i]));
	bool rebuild = (find(args, "-reb") != -1 || find(args, "--rebuild") != -1);
	std::string wd = createEssentials(rebuild);
	std::string elfFile = elfFileName;
	std::string hexFile = wd + "/" + hexFileName;
	std::string cmd = "/home/sergantdornan/builder/builder ";
	for(int i = 0; i < args.size(); ++i)
		cmd += (args[i] + " ");
	cmd += ("-o " + elfFile + " ");
	cmd += ("-I" + root + "/mik32Include ");
	cmd += ("-lmik32_hal -lmik32_shared ");
	cmd += ("// " + compileFlags + " // ");
	cmd += ("/// " + linkFlags + " /// ");
	cmd += ("--CC riscv-none-elf-gcc --CXX riscv-none-elf-g++ ");
	std::cout << "========================== Launching belder ==========================" << std::endl;
	system(cmd.c_str());
	if(!exists(elfFile)){
		std::cout << "============== mik32Loader: some error in belder ==============" << std::endl;
		return -1;
	}
	elfFile = cd + "/" + elfFileName;
	cmd = "riscv-none-elf-objcopy -O ihex " + elfFile + " " + hexFile;
	system(cmd.c_str());
	cmd = "python3 " + root + "/mik32-uploader/mik32_upload.py " + hexFile + " --run-openocd --openocd-exec /usr/bin/openocd --openocd-target " + root + "/mik32-uploader/openocd-scripts/target/mik32.cfg --openocd-interface " + root + "/mik32-uploader/openocd-scripts/interface/ftdi/mikron-link.cfg --adapter-speed 500 --mcu-type MIK32V2";
	system(cmd.c_str());
}