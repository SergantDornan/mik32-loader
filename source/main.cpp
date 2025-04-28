#include "Flags.h"
#include "essentials.h"
#include "constants.h"
#include "uninstall.h"


const std::string SourceCode;

// config file for project:
// entry file
// SERIAL_BOUDRATE
// USB port for picocom
// ld script
int main(int argc, char* argv[]){
	std::vector<std::string> args;
	for(int i = 1; i < argc; ++i)
		args.push_back(std::string(argv[i]));
	if(argc > 1 && std::string(argv[1]) == "uninstall"){
		uninstall();
		return 0;
	}
	if(argc > 1 && std::string(argv[1]) == "reinstall"){
		uninstall();
		std::string cmd = "make -C " + SourceCode;
		system(cmd.c_str());
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
		if(find(possibleFlags, args[i]) == -1){
			if(i == 0 || find(possibleFlags, args[i-1]) == -1)
				cmd += (args[i] + " ");
		}
	}
	cmd += ("-o " + elfFile + " ");
	for(int i = 0; i < IncFolders.size(); ++i)
		cmd += ("-I" + IncFolders[i] + " ");
	cmd += ("-lmik32_hal -lmik32_shared ");
	if(!exists(Compilers[0]) || !exists(Compilers[1]) || !exists(Compilers[3]))
		cmd += ("--CC riscv-none-elf-gcc --CXX riscv-none-elf-g++ --preproc riscv-none-elf-cpp ");
	else
		cmd += ("--CC " + Compilers[0] + " --CXX " + Compilers[1] + " --preproc " + Compilers[3] + " ");
	if(getExt(parameters[0]) == "cpp") cmd += (Cppstandart + " ");
	else cmd += (Cstandart + " ");
	cmd += ("--compile-flags " + compileFlags + " ");
	cmd += ("--link-flags " + linkFlags + " ");
	cmd += ("-Wl,-T" + parameters[3] + " ");
	std::cout << "========================== Launching belder ==========================" << std::endl;
	int beldercode = (system(cmd.c_str()) / 256);
	if(!exists(elfFile))
		return -1;
	if(beldercode != 10){
		if(exists(Compilers[2]))
			cmd = Compilers[2] + " -O ihex " + elfFile + " " + hexFile;
		else
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