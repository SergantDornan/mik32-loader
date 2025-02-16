#include <filework.h>
extern void clear(std::string& path){
	std::ofstream out(path);
	if(!out.is_open()){
		std::string s = "filework.h : clear : Cannot open file " + path;
		std::cout << s << std::endl;
	}
	else{
		out << "";
	}
	out.close();
}
std::vector<std::string> getDirs(const std::string &path) {
	std::vector<std::string> dirs;
	if(!std::filesystem::is_directory(path)){
		std::cout << "======================= ERROR =======================" << std::endl;
		std::cout << "=============== filework.cpp: getDirs ===============" << std::endl;
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