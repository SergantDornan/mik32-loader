#include "BuilderFilework.h"
#include "uninstall.h"
#include "alias.h"
#include <thread>
const size_t numThreads = std::thread::hardware_concurrency();
bool checkProgram(const std::string& programName) {
    std::string command = "which " + programName + " > /dev/null 2>&1";
    int result = system(command.c_str());
    return result == 0;
}
const std::vector<std::string> reqPacks = {"ca-certificates", "git", "make",
 "python3", "openocd", "picocom"};
int main(int argc, char* argv[]) {
    if(argc >= 2 && std::string(argv[1]) == "reinstall")
        uninstall();
    std::cout << "!!!!!!! Required packages: !!!!!" << std::endl;
    for(int i = 0;i < reqPacks.size(); ++i)
        std::cout << reqPacks[i] << " ";
    std::cout << std::endl;
    if(exists(root)){
    	std::cout << "====================== ERROR ======================" << std::endl;
    	std::cout <<  root << " already exists" << std::endl;
        std::cout << "Cannot install mik32Loader" << std::endl;
    	std::cout << "Remove " << root << " before installation" << std::endl;
        return -1;
    }
    std::string cmd = "mkdir " + root;    
    system(cmd.c_str());
    if(!checkProgram("riscv-none-elf-gcc") && 
        !exists(getHomedir() + "/xpack-riscv-none-elf-gcc-14.2.0-3/bin/riscv-none-elf-gcc")){
        std::cout << "====== installing riscv-none-elf-gcc ======" << std::endl;  
        cmd = "mkdir " + root + "/riscv-none-elf-gcc-installer";
        system(cmd.c_str()); 
        cmd = "git clone https://github.com/SergantDornan/riscv-none-elf-gcc-installer " + root + "/riscv-none-elf-gcc-installer";
        system(cmd.c_str());
        cmd = root + "/riscv-none-elf-gcc-installer/install " + root + "/riscv-none-elf-gcc-installer";
        system(cmd.c_str());
        cmd = "rm -rf " + root + "/riscv-none-elf-gcc-installer";
        system(cmd.c_str());
        if(!exists(getHomedir() + "/xpack-riscv-none-elf-gcc-14.2.0-3/bin/riscv-none-elf-gcc")){
            std::cout << "================== ERROR ==================" << std::endl;
            std::cout << "Something wrong with riscv-none-elf-gcc installation" << std::endl;
            std::cout << "riscv-none-elf-gcc is not installed" << std::endl;
            return -1;
        }
    }
    if(!exists(getHomedir() + "/builder/builder")){
        std::cout << "========= installing belder =========" << std::endl;
        cmd = "mkdir " + root + "/C-Cpp-builder";
        system(cmd.c_str());
        cmd = "git clone https://github.com/SergantDornan/C-Cpp-builder " + root + "/C-Cpp-builder";
        system(cmd.c_str());
        cmd = "make -C " + root + "/C-Cpp-builder/ install -j " + std::to_string(numThreads);
        system(cmd.c_str());
        cmd = "rm -rf " + root + "/C-Cpp-builder";
        system(cmd.c_str());
        if(!exists(getHomedir() + "/builder/builder")){
            std::cout << "================== ERROR ==================" << std::endl;
            std::cout << "Something wrong with builder installation" << std::endl;
            std::cout << "Builder is not installed" << std::endl;
            return -1;
        }
    }
    cmd = "cp -r ./ldscripts " + root;
    system(cmd.c_str());
    cmd = "mkdir " + root + "/mik32-uploader";
    system(cmd.c_str());
    cmd = "git clone https://github.com/MikronMIK32/mik32-uploader " + root + "/mik32-uploader";
    system(cmd.c_str());
    cmd = "cp -r ./hardware " + root;
    system(cmd.c_str());
    cmd = "mkdir " + root + "/CompiledLibs";
    system(cmd.c_str());
    cmd = "cp libmik32_hal.a " + root + "/CompiledLibs";
    system(cmd.c_str());
    cmd = "cp libmik32_shared.a " + root + "/CompiledLibs";
    system(cmd.c_str());
    std::ifstream makefile(cd + "/Makefile");
    std::vector<std::string> lines;
    std::string line;
    std::getline(makefile,line);
    lines.push_back(line);
    if(line != std::string("OUTPUT=" + root + "/loader")){
        while(std::getline(makefile, line))
            lines.push_back(line);
        lines[0] = "OUTPUT=" + root + "/loader";
    }
    makefile.close();
    if(lines.size() > 1){
        std::ofstream out(cd + "/Makefile");
        for(int i = 0; i < lines.size(); ++i)
            out << lines[i] << std::endl;
        out.close();
    }
    addAlias("mik32Load", root + "/loader");
    cmd = "make -C "+cd+" -j " + std::to_string(numThreads);
    system(cmd.c_str());
    if(exists(root + "/loader"))
        std::cout << "================= mik32Loader has been installed =================" << std::endl;
    return 0;
}