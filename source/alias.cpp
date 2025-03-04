#include "alias.h"
std::string findConfig(){
	for(int i = 0; i < configFiles.size(); ++i){
		if(exists(configFiles[i]))
			return configFiles[i];
	}
	return "-1";
}
std::string getLine(const std::string& config, const std::string& alias,
	const std::string& cmd){
	std::string name = getName(config);
	if(name == ".bashrc" || name == ".zshrc")
		return "alias "+ alias + "='" + cmd + "'";
	else if(name == "config.fish")
		return "alias "+ alias + " '" + cmd + "'";
	return "-1";
}
void addAlias(const std::string& alias, const std::string& cmd){
	std::string config = findConfig();
	if(config == "-1"){
		std::cout << "======================= ERROR =======================" << std::endl;
		std::cout << "alias.cpp: addAlias: cannot find config file " << std::endl;
		std::cout << "probably does not work for your system" << std::endl;
		std::cout << "cannot add alias" << std::endl;
		return;
	}
	std::string line = getLine(config, alias, cmd);
	if(line == "-1"){
		std::cout << "======================= ERROR =======================" << std::endl;
		std::cout << "alias.cpp: addAlias: some error in: " << std::endl;
		std::cout << "alias.cpp: getLine" << std::endl;
		return;
	}
	std::ofstream bshrc(config, std::ios::app);
    bshrc << line << std::endl;
    bshrc.close();
}
void removeAlias(const std::string& alias, const std::string& cmd){
	std::string config = findConfig();
	if(config == "-1"){
		std::cout << "======================= ERROR =======================" << std::endl;
		std::cout << "alias.cpp: removeAlias: cannot find config file " << std::endl;
		std::cout << "probably does not work for your system" << std::endl;
		std::cout << "cannot remove alias" << std::endl;
		return;
	}
	std::string line = getLine(config, alias, cmd);
	if(line == "-1"){
		std::cout << "======================= ERROR =======================" << std::endl;
		std::cout << "alias.cpp: removeAlias: some error in: " << std::endl;
		std::cout << "alias.cpp: getLine" << std::endl;
		return;
	}
    std::string l;
    std::vector<std::string> v;
    std::ifstream file(config);
    while(std::getline(file,l)){
        if(l != line)
            v.push_back(line);
    }
    file.close();
    std::ofstream newfile(config);
    for(int i = 0; i < v.size(); ++i)
        newfile << v[i] << std::endl;
    newfile.close();
}