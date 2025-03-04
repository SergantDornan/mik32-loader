#include "alias.h"
std::vector<std::string> findConfig(){
	std::vector<std::string> v;
	for(int i = 0; i < configFiles.size(); ++i){
		if(exists(configFiles[i]))
			v.push_back(configFiles[i]);
	}
	return v;
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
	std::vector<std::string> config = findConfig();
	if(config.size() == 0){
		std::cout << "======================= ERROR =======================" << std::endl;
		std::cout << "alias.cpp: addAlias: cannot find config file " << std::endl;
		std::cout << "probably does not work for your system" << std::endl;
		std::cout << "cannot add alias" << std::endl;
		return;
	}
	for(int i = 0; i < config.size(); ++i){
		std::string line = getLine(config[i], alias, cmd);
		if(line == "-1"){
			std::cout << "======================= ERROR =======================" << std::endl;
			std::cout << "alias.cpp: addAlias: some error in: " << std::endl;
			std::cout << "alias.cpp: getLine" << std::endl;
			return;
		}
		std::ofstream bshrc(config[i], std::ios::app);
    	bshrc << line << std::endl;
    	bshrc.close();
	}
}
void removeAlias(const std::string& alias, const std::string& cmd){
	std::vector<std::string> config = findConfig();
	if(config.size() == 0){
		std::cout << "======================= ERROR =======================" << std::endl;
		std::cout << "alias.cpp: removeAlias: cannot find config file " << std::endl;
		std::cout << "probably does not work for your system" << std::endl;
		std::cout << "cannot remove alias" << std::endl;
		return;
	}
	for(int i = 0; i < config.size(); ++i){
		std::string line = getLine(config[i], alias, cmd);
		if(line == "-1"){
			std::cout << "======================= ERROR =======================" << std::endl;
			std::cout << "alias.cpp: removeAlias: some error in: " << std::endl;
			std::cout << "alias.cpp: getLine" << std::endl;
			return;
		}
    	std::string l;
    	std::vector<std::string> v;
    	std::ifstream file(config[i]);
    	while(std::getline(file,l)){
        	if(l != line)
            	v.push_back(l);
    	}
    	file.close();
    	std::ofstream newfile(config[i]);
    	for(int i = 0; i < v.size(); ++i)
        	newfile << v[i] << std::endl;
    	newfile.close();
	}
}