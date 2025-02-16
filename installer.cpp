#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> 
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <filesystem>
#include <fstream>
#include <thread>
const size_t numThreads = std::thread::hardware_concurrency();
//sudo apt update
//sudo apt install -y --no-install-recommends ca-certificates git make python3 openocd picocom
const std::string mainDir = "mik32Loader";
std::string getHomedir(){
    const char* homeDir = getenv("HOME");
    if (homeDir) {
        return homeDir;
    } else {
        std::cout << "======================== ERROR ========================" << std::endl;
        std::cout << "==== some error in installer.cpp: getHomedir() ====" << std::endl;
        std::cout << std::endl;      
        return "";
    }
}
void uninstall(){
    std::string root = getHomedir() + "/" + mainDir;
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
std::vector<std::string> getDirs(const std::string &path) {
	std::vector<std::string> dirs;
	if(!std::filesystem::is_directory(path)){
		std::cout << "======================= ERROR =======================" << std::endl;
		std::cout << "=============== installer.cpp: getDirs ===============" << std::endl;
		std::cout << "path leads to a file, not directory" << std::endl;
		std::cout << path << std::endl;
		std::cout << "=====================================================" << std::endl;
		return dirs;
	}
  std::string back = path;
  while (back.back() != '/')
    {
      back.pop_back();
    }
  back.pop_back();
  dirs = {back};
  for (const std::filesystem::directory_entry &dir : std::filesystem::directory_iterator(path))
    {
      dirs.push_back(dir.path().string());
    }
  return dirs;
}
bool checkProgram(const std::string& programName) {
    std::string command = "which " + programName + " > /dev/null 2>&1";
    int result = system(command.c_str());
    return result == 0;
}
bool exists(const std::string& path){
	return std::filesystem::exists(path);
}
bool isRunningAsRoot() {
  return geteuid() == 0;
}
const std::string root = getHomedir() + "/" + mainDir;
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
    if(!checkProgram("riscv-none-elf-gcc")){
        std::cout << "====== installing riscv-none-elf-gcc ======" << std::endl;  
        cmd = "mkdir " + root + "/riscv-none-elf-gcc-installer";
        system(cmd.c_str()); 
        cmd = "git clone https://github.com/SergantDornan/riscv-none-elf-gcc-installer " + root + "/riscv-none-elf-gcc-installer";
        system(cmd.c_str());
        cmd = root + "/riscv-none-elf-gcc-installer/install " + root + "/riscv-none-elf-gcc-installer";
        system(cmd.c_str());
        cmd = "rm -rf " + root + "/riscv-none-elf-gcc-installer";
        system(cmd.c_str());
    }
    if(!exists(getHomedir() + "/builder/builder")){
        std::cout << "========= installing belder =========" << std::endl;
        cmd = "mkdir " + root + "/C-Cpp-builder";
        system(cmd.c_str());
        cmd = "git clone https://github.com/SergantDornan/C-Cpp-builder " + root + "/C-Cpp-builder";
        system(cmd.c_str());
        cmd = root + "/C-Cpp-builder/install --no-make";
        system(cmd.c_str());
        cmd = "make -C " + root + "/C-Cpp-builder -j " + std::to_string(numThreads);
        system(cmd.c_str());
        cmd = "rm -rf " + root + "/C-Cpp-builder";
        std::cout << "=============== builder has been installed ===============" << std::endl;
        system(cmd.c_str());
    }
    cmd = "cp -r ./ldscripts " + root;
    system(cmd.c_str());
    cmd = "mkdir " + root + "/mik32-uploader";
    system(cmd.c_str());
    cmd = "git clone https://github.com/MikronMIK32/mik32-uploader " + root + "/mik32-uploader";
    system(cmd.c_str());
    cmd = "cp -r ./mik32Include " + root;
    system(cmd.c_str());
    cmd = "cp libmik32_hal.a " + root + "/mik32Include";
    system(cmd.c_str());
    cmd = "cp libmik32_shared.a " + root + "/mik32Include";
    system(cmd.c_str());
    std::ifstream makefile("./Makefile");
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
        std::ofstream out("./Makefile");
        for(int i = 0; i < lines.size(); ++i)
            out << lines[i] << std::endl;
        out.close();
    }
    std::string bash = getHomedir() + "/" + ".bashrc";
    std::string alias = "alias mik32Load='" + root + "/loader'";
    std::ofstream bshrc(bash, std::ios::app);
    bshrc << alias << std::endl;
    bshrc.close();
    system(alias.c_str());
    cmd = "make -j " + std::to_string(numThreads);
    system(cmd.c_str());
    if(exists(root + "/loader"))
        std::cout << "================= mik32Loader has been installed =================" << std::endl;
    return 0;
}