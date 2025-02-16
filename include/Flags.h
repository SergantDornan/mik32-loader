#include "BuilderFilework.h"
bool isFlag(const std::string&);
std::string findFile(const std::string&,const std::string&);
std::vector<std::string> getParameters(const std::vector<std::string>&,
	const std::string& , const std::string&);
void getIdirs(const std::vector<std::string>&, std::string&);
void findEntryFile(const std::vector<std::string>&, std::string&,
	const std::string&);
void getNameAfterFlag(const std::vector<std::string>&,
	const std::string&,std::string&);