#include "Flags.h"
#include "algs.h"
bool isFlag(const std::string& s){
	return ((s.size() >= 2 && s[0] == '-' && s[1] != '-') ||
	(s.size() >= 3 && s[0] == '-' && s[1] == '-' && s[2] != '-'));	
}
std::string findFile(const std::string& name, const std::string& dir){
	auto dirs = getDirs(dir);
	for(int i = 1; i < dirs.size(); ++i){
		if(name == getName(dirs[i]))
			return dirs[i];
		if(std::filesystem::is_directory(dirs[i])){
			std::string s = findFile(name,dirs[i]);
			if(s != "-1")
				return s;
		}
	}
	return "-1";
}
std::vector<std::string> getParameters(const std::vector<std::string>& args,
	const std::string& path, const std::string& cd){

	std::vector<std::string> parameters;
	std::ifstream in(path);
	std::string line;
	while(std::getline(in, line)) parameters.push_back(line);
	in.close();
	findLinkingScript(args,parameters[3]);
	findEntryFile(args,parameters[0],cd);
	getNameAfterFlag(args, "-b", parameters[1]);
	getNameAfterFlag(args,"-p",parameters[2]);
	return parameters;
}
void findLinkingScript(std::vector<std::string>& args,std::string& s){
	auto it = args.begin();
	while(it != args.end()){
		if((*it).size() >= 2 && (*it)[0] == '-' && 
			(*it)[1] == 'T')
		{
			s = std::string((*it).begin() + 2, (*it).end());
			args.erase(it);
			return;
		}
		it++;
	}
}
void getIdirs(const std::vector<std::string>& args, std::string& s){
	std::vector<std::string> AddInc;
	if (s != "-1") AddInc = split(s);
	for(int i = 0; i < args.size(); ++i){
		if(isFlag(args[i]) && args[i][1] == 'I'){
			std::string folder(args[i].begin() + 2, args[i].end());
			if(find(AddInc, folder) == -1) AddInc.push_back(folder);
		}
	}
	std::vector<std::string> NoInc;
	for(int i = 0; i < args.size(); ++i){
		if(args[i] == "--no-include"){
			if((i + 1) >= args.size() || ((i + 1) < args.size() &&
			isFlag(args[i + 1]))){
				std::cout << "======================== ERROR ========================" << std::endl;
				std::cout << "no folder after --no-include flag" << std::endl;
				return;
			}
			else if(find(NoInc, args[i+1]) == -1) NoInc.push_back(args[i+1]);
		}
	}
	auto it = AddInc.begin();
	while(it != AddInc.end()){
		if(find(NoInc, *it) != -1)
			AddInc.erase(it);
		else
			it++;
	}
	if(AddInc.size() == 0)
		s = "-1";
	else{
		s = "";
		for(int i = 0; i < AddInc.size(); ++i)
			s += (AddInc[i] + " ");
	}
}
void findEntryFile(const std::vector<std::string>& args, std::string& s,
	const std::string& cd){
	if(args.size() != 0 && args[0] != "run" && args[0] != "config" && 
		args[0] != "status" && args[0] != "read" && !isFlag(args[0])){

		std::string mainFile = findFile(args[0], cd);
		if(mainFile == "-1"){
			std::cout << "================== ERROR ==================" << std::endl;
			std::cout << "Cannot find file: " << args[0] << std::endl;
			return;
		}
		s = mainFile;
	}
	if(args.size() == 0 || (args.size() != 0 && isFlag(args[0]))){
		if(s == "-1"){
			std::string mainFile = findFile("main.cpp", cd);
			if(mainFile == "-1")
				mainFile = findFile("main.c", cd);
			if(mainFile == "-1"){
				std::cout << "================== ERROR ==================" << std::endl;
				std::cout << "Cannot find entry file" << std::endl;
				return;
			}
			s = mainFile;
		}
	}
}
void getNameAfterFlag(const std::vector<std::string>& args,
	const std::string& flag,std::string& s){
	int index = find(args, flag);
	if(index != -1){
		if((index + 1) >= args.size() || ((index + 1) < args.size() &&
			isFlag(args[index + 1]))){
			std::cout << "=================== ERROR ===================" << std::endl;
			std::cout << "no file name after " << flag << " flag" << std::endl;
			return;
		}
		s = args[index + 1];
	}
}