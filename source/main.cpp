#include "Flags.h"
#include "essentials.h"
void uninstall(){
    std::string root = getHomedir() + "/mik32Loader";
    std::string cmd = "rm -rf " + root;
    system(cmd.c_str());
    std::string alias = "alias mik32Load='" + root + "/loader'";
    std::string bash = getHomedir() + "/" + ".bashrc";
    std::string line;
    std::vector<std::string> v;
    std::ifstream file(bash);
    while(std::getline(file,line)){
        if(line != alias)
            v.push_back(line);
    }
    file.close();
    std::ofstream newfile(bash);
    for(int i = 0; i < v.size(); ++i)
        newfile << v[i] << std::endl;
    newfile.close();
    std::cout << "mik32Loader has been removed from your computer" << std::endl;
}
const std::string compileFlags = "-march=rv32imc_zicsr_zifencei -mabi=ilp32 -mcmodel=medlow -g3 -Wall -fsigned-char -ffunction-sections -DMIK32V2";
const std::string linkFlags = "-march=rv32imc_zicsr_zifencei -mabi=ilp32 -mcmodel=medlow -g3 -Wall -fsigned-char -ffunction-sections -DMIK32V2 -Wl,-Teeprom.ld -Xlinker --gc-sections -nostartfiles -L" + root + "/ldscripts";
const std::string elfFileName = "output.elf";
const std::string hexFileName = "output.hex";
const std::string Cstandart  = "-std=gnu11";
const std::string Cppstandart = "-std=gnu++17";
int main(int argc, char* argv[]){
	std::vector<std::string> args;
	for(int i = 1; i < argc; ++i)
		args.push_back(std::string(argv[i]));
	if(argc > 1 && std::string(argv[1]) == "uninstall"){
		uninstall();
		return 0;
	}
	bool rebuild = (find(args, "-reb") != -1 || find(args, "--rebuild") != -1);
	std::string wd = createEssentials(rebuild);
	std::string entryFile;
	findEntryFile(args, entryFile,cd);
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
	if(getExt(entryFile) == "cpp") cmd += (Cppstandart + " ");
	else cmd += (Cstandart + " ");
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