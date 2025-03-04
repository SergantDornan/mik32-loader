#ifndef ALIAS
#define ALIAS

#include "BuilderFilework.h"
const std::vector<std::string> configFiles = {getHomedir() + "/.bashrc", 
	getHomedir() + "/.zshrc", getHomedir() + "/.config/fish/config.fish"};
std::string findConfig();
std::string getLine(const std::string&, const std::string&, const std::string&);
void addAlias(const std::string&, const std::string&);
void removeAlias(const std::string&, const std::string&);


#endif