#include "uninstall.h"
void uninstall(){
    std::string cmd = "rm -rf " + root;
    system(cmd.c_str());
    removeAlias("mik32Load", root + "/loader");
    std::cout << "mik32Loader has been removed from your computer" << std::endl;
}