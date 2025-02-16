#ifndef filework_h
#define filework_h
#include <algs.h>
#include <fstream>
#include <inputs.h>
#include <filesystem>

std::vector<std::string> getDirs(const std::string&); 
// Выдает список файлов и папок, аналогично ls, только путь полный относительно папки, 
// откуда вызывается функция

extern void clear(std::string&);

template <class T>
void stringToVector(std::string& s, std::vector<T>& v){
	std::stringstream stream;
	stream << s;
	stream >> v;
}

template <class T>
void writeVectors(std::vector<std::vector<T>>& v, std::string path){
	std::ofstream out(path);
	if(!out.is_open()){
		std::string s = "filework.h : writeVector: Cannot open file " + path; 
		std::cout << s << std::endl; 
	}
	else{
		for(int i = 0; i < v.size(); ++i){
			for(int j = 0; j < v[i].size(); ++j)
				out << v[i][j] << ' ';
			out << '\n';
		}
	}
	out.close();
}

template <class T>
void writeVectors(std::vector<T>& v, std::string path){
	//std::cout << path << std::endl;
	std::ofstream out(path);
	if(!out.is_open()){
		std::string s = "filework.h : writeVector: Cannot open file " + path; 
		std::cout << s << std::endl; 
	}
	else{
		for(int i = 0; i < v.size(); ++i){
				out << v[i] << ' ';
		}
		out << '\n';
	}
	out.close();
}

template <class T>
void readVectors(std::vector<std::vector<T>>& v, std::string path){
	
	std::ifstream input(path);
	if(!input.is_open()){
		std::string s = "filework.h : readVectors: Cannot open file " + path; 
		std::cout << s << std::endl; 
	}
	else{
		std::string line;
		while (std::getline(input, line))
       	{
       		std::vector<T> curr;
       		stringToVector(line,curr);
       		v.push_back(curr);
       	}
	}
	input.close();
}

#endif