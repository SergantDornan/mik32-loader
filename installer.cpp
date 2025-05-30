#include "BuilderFilework.h"
#include "uninstall.h"
#include "alias.h"
#include <thread>
void rewriteLine(const std::string& path, 
    const std::string& oldLine, const std::string& newLine)
{
    std::vector<std::string> lines;
    std::string line;
    std::ifstream input(path);
    if(!input.is_open()){
        std::cout << "================= ERROR =================" << std::endl;
        std::cout << "installer.cpp: rewriteLine: cannot open file: " << std::endl;
        std::cout << path << std::endl;
        std::cout << "=========================================" << std::endl;
        return;
    }
    while(std::getline(input, line))
        lines.push_back(line);
    input.close();
    for(int i = 0; i < lines.size(); ++i){
        if(lines[i] == oldLine){
            lines[i] = newLine;
            break;
        }
    }
    std::ofstream out(path);
    for(int i = 0; i < lines.size(); ++i)
        out << lines[i] << std::endl;
    out.close();
}

const size_t numThreads = std::thread::hardware_concurrency();
bool checkProgram(const std::string& programName) {
    std::string command = "which " + programName + " > /dev/null 2>&1";
    int result = system(command.c_str());
    return result == 0;
}
int main(int argc, char* argv[]) {
    if(argc >= 2 && std::string(argv[1]) == "reinstall")
        uninstall();
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
        cmd = "make -C " + root + "/riscv-none-elf-gcc-installer install -j " + std::to_string(numThreads);
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
    cmd = "cp -r " + cd + "/ldscripts " + root;
    system(cmd.c_str());
    cmd = "cp -r " + cd + "/mik32-uploader " + root;
    system(cmd.c_str());
    // cmd = "mkdir " + root + "/mik32-uploader";
    // system(cmd.c_str());
    // cmd = "git clone https://github.com/MikronMIK32/mik32-uploader " + root + "/mik32-uploader";
    // system(cmd.c_str());
    cmd = "cp -r " + cd + "/hardware " + root;
    system(cmd.c_str());
    cmd = "mkdir " + root + "/CompiledLibs";
    system(cmd.c_str());
    cmd = "cp libmik32_hal.a " + root + "/CompiledLibs";
    system(cmd.c_str());
    cmd = "cp libmik32_shared.a " + root + "/CompiledLibs";
    system(cmd.c_str());
    rewriteLine(cd + "/Makefile","OUTPUT=",
        std::string("OUTPUT=" + root + "/loader"));
    rewriteLine(cd + "/source/main.cpp","const std::string SourceCode;",
        std::string("const std::string SourceCode = \"" + cd + "\";"));
    addAlias("mik32Load", root + "/loader");
    cmd = "make -C "+ cd + " -j " + std::to_string(numThreads);
    system(cmd.c_str());
    rewriteLine(cd + "/Makefile",std::string("OUTPUT=" + root + "/loader"),
        "OUTPUT=");
    rewriteLine(cd + "/source/main.cpp",std::string("const std::string SourceCode = \"" + cd + "\";"),
        "const std::string SourceCode;");
    if(exists(root + "/loader"))
        std::cout << "================= mik32Loader has been installed =================" << std::endl;
    return 0;
}