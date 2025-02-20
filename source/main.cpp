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
const std::string picocomFlags = "--omap crcrlf --echo";
const std::vector<std::string> IncFolders = {root + "/hardware/mik32-hal/core/Include",
root + "/hardware/mik32-hal/peripherals/Include", root + "/hardware/mik32-hal/utilities/Include",
root + "/hardware/mik32v2-shared/include", root + "/hardware/mik32v2-shared/libs",
root + "/hardware/mik32v2-shared/periphery", root + "/CompiledLibs"};
const std::vector<std::string> possibleFlags = {"-b", "-p"}; 


// config file for project:
// entry file
// SERIAL_BOUDRATE
// USB port for picocom
int main(int argc, char* argv[]){
	std::vector<std::string> args;
	for(int i = 1; i < argc; ++i)
		args.push_back(std::string(argv[i]));
	if(argc > 1 && std::string(argv[1]) == "uninstall"){
		uninstall();
		return 0;
	}
	bool rebuild = (find(args, "-reb") != -1 || find(args, "--rebuild") != -1);
	bool read = (find(args,"read") != -1);
	std::string wd = createEssentials(rebuild);
	std::string configPath = wd + "/config"; 
	auto parameters = getParameters(args,configPath,cd);
	std::ofstream out(configPath);
	for(int i = 0; i < parameters.size(); ++i)
		out << parameters[i] << std::endl;
	std::string elfFile = wd + "/" + elfFileName;
	std::string hexFile = wd + "/" + hexFileName;
	std::string cmd = getHomedir() + "/builder/builder ";
	for(int i = 0; i < args.size(); ++i){
		if(find(possibleFlags, args[i]) == -1 && isFlag(args[i]) &&
			args[i] != "-o" && args[i] != "--CC" && args[i] != "--CXX")
			cmd += (args[i] + " ");
	}
	cmd += ("-o " + elfFile + " ");
	for(int i = 0; i < IncFolders.size(); ++i)
		cmd += ("-I" + IncFolders[i] + " ");
	cmd += ("-lmik32_hal -lmik32_shared ");
	cmd += ("// " + compileFlags + " // ");
	cmd += ("/// " + linkFlags + " /// ");
	cmd += ("--CC riscv-none-elf-gcc --CXX riscv-none-elf-g++ ");
	if(getExt(parameters[0]) == "cpp") cmd += (Cppstandart + " ");
	else cmd += (Cstandart + " ");
	std::cout << "========================== Launching belder ==========================" << std::endl;
	int beldercode = (system(cmd.c_str()) / 256);
	if(!exists(elfFile))
		return -1;
	if(beldercode != 10){
		cmd = "riscv-none-elf-objcopy -O ihex " + elfFile + " " + hexFile;
		system(cmd.c_str());
		cmd = "python3 " + root + "/mik32-uploader/mik32_upload.py " + hexFile + " --run-openocd --openocd-exec /usr/bin/openocd --openocd-target " + root + "/mik32-uploader/openocd-scripts/target/mik32.cfg --openocd-interface " + root + "/mik32-uploader/openocd-scripts/interface/ftdi/mikron-link.cfg --adapter-speed 500 --mcu-type MIK32V2";
		system(cmd.c_str());
	}
	else
		std::cout << "mik32Loader: nothing to upload" << std::endl;
	if(read){
		std::cout << "================ Launching picocom ================" << std::endl;
		auto devDir = getDirs("/dev");
		std::vector<std::string> USBports;
		cmd = "sudo picocom ";
		for(int i = 1; i < devDir.size(); ++i){
			if(devDir[i].find("/dev/ttyUSB") != std::string::npos)
				USBports.push_back(devDir[i]);
		}
		if(USBports.size() == 0){
			std::cout << std::endl;
			std::cout << "=================== ERROR ===================" << std::endl;
			std::cout << "Cannot find active USB port" << std::endl;
			std::cout << "Have you connected your device?" << std::endl;
			return -1;
		}
		else if(USBports.size() == 1){
			std::cout << "Single USB port found: " << USBports[0] << std::endl;
			cmd += (USBports[0] + " ");
		}
		else{
			std::cout << "Entered USB port: /dev/ttyUSB" << parameters[2] << std::endl;
			cmd += ("/dev/ttyUSB" + parameters[2] + " ");
		}
		cmd += ("-b " + parameters[1] + " ");
		cmd += picocomFlags;
		system(cmd.c_str());
	}
	return 0;
}