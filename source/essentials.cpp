#include "essentials.h"
std::string createEssentials(const bool reb){
	auto mainDirs = getDirs(root);
	bool isConfig = false;
	for(int i = 1; i < mainDirs.size(); ++i){
		std::string name = getName(mainDirs[i]);
		if(name == configFile){
			isConfig = true;
			break;
		}
	}
	if(!isConfig){
		for(int i = 1; i < mainDirs.size(); ++i){
			if(std::filesystem::is_directory(mainDirs[i]) && getName(mainDirs[i]) != "ldscripts" &&
				getName(mainDirs[i]) != "mik32-uploader" && getName(mainDirs[i]) != "hardware" && 
				getName(mainDirs[i]) != "CompiledLibs"){
				std::string cmd = "rm -rf " + mainDirs[i];
				system(cmd.c_str()); 
 			}
		}
		std::string cmd = "touch " + root + "/" + configFile;
		system(cmd.c_str()); 
	}
	std::string configPath = root + "/" + configFile;
	std::ifstream config(configPath);
	std::string line;
	bool isDir = false;
	std::vector<std::string> projectList;
	while(std::getline(config,line))
		projectList.push_back(line);
	config.close();

	auto it = projectList.begin();
	while(it != projectList.end()){
		std::string p = split(*it, "*")[0];
		if(!exists(p)){
			projectList.erase(it);
			std::string cmd = "rm -rf " + root + "/" + split(*it, "*")[1];
			system(cmd.c_str());
			std::ofstream f(configPath);
			for(int i = 0; i < projectList.size(); ++i)
				f << projectList[i] << std::endl;
			f.close();
		}
		else
			it++;
	}


	std::string index;
	for(int i = 0; i < projectList.size(); ++i){
		auto s = split(projectList[i], "*");
		if(s[0] == cd){
			index = s[1];
			std::string folder = root + "/" + index;
			if(!exists(folder)){
				std::cout << "================= FATAL ERROR =================" << std::endl;
				std::cout << "Main config file is corrupted and it is probably your fault" << std::endl;
				std::cout << "rebuilding all projects and configs" << std::endl;
				std::cout << std::endl;
				std::string cmd = "rm " + configPath;
				system(cmd.c_str());
				return createEssentials(false);
			} 
			else
				isDir = true;
			break;
		}
	}
	if(isDir && reb){
		std::string cmd = "rm -rf " + root + "/" + index;
		system(cmd.c_str());
		cmd = "mkdir " + root + "/" + index;
		system(cmd.c_str());
	}
	else if(!isDir){
		index = std::to_string(projectList.size() + 1);
		std::ofstream config(configPath,std::ios::app);
		config << cd << '*' << index << std::endl;
		config.close();
		std::string cmd = "mkdir " + root + "/" + index;
		system(cmd.c_str());
	}
	std::string folder = root + "/" + index;
	auto inDir = getDirs(folder);
	auto inDirNames = inDir;
	for(int i = 0; i < inDirNames.size(); ++i)
		inDirNames[i] = getName(inDirNames[i]);
	bool rebuild = false;
	for(int i = 0; i < reqFolders.size(); ++i){
		if(find(inDirNames,reqFolders[i]) == -1){
			rebuild = true;
			break;
		}
	}
	if(find(inDirNames, configFile) == -1)
		rebuild = true;
	if(rebuild){
		for(int i = 1; i < inDir.size(); ++i){
			std::string cmd = "rm -rf " + inDir[i];
			system(cmd.c_str());
		}
		for(int i = 0; i < reqFolders.size(); ++i){
			std::string cmd = "mkdir " + folder + "/" + reqFolders[i];
			system(cmd.c_str());
		}
		std::string cmd = "touch " + folder + "/" + configFile;
		std::ofstream out(folder + "/" + configFile);
		out << "-1" << std::endl;
		out << "115200" << std::endl;
		out << "0" << std::endl;
		out.close();
		system(cmd.c_str());
	}
	for(int i = 0; i < reqFolders.size(); ++i){
		std::string subFolder = folder + "/" + reqFolders[i];
		auto innerDir = getDirs(subFolder);
		auto innerDirNames = innerDir;
		for(int j = 0; j < innerDirNames.size(); ++j)
			innerDirNames[j] = getName(innerDirNames[j]);
		if(i == 0 && find(innerDirNames, subFolders[0]) == -1){
			std::string cmd = "mkdir " + subFolder + "/" + subFolders[0];
			system(cmd.c_str());
		}
		else if(i != 0){
			bool rebuildSubFolder = false;
			for(int j = 0; j < subFolders.size(); ++j){
				if(find(innerDirNames, subFolders[j]) == -1){
					rebuildSubFolder = true;
					break;
				}
			}
			if(rebuildSubFolder){
				for(int j = 1; j < innerDir.size(); ++j){
					std::string cmd = "rm -rf " + innerDir[j];
					system(cmd.c_str());
				}
				for(int j = 0; j < subFolders.size(); ++j){
					std::string cmd = "mkdir " + subFolder + "/" + subFolders[j];
					system(cmd.c_str());
				}
			}
		}
	}
	return folder;
}