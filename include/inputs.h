#ifndef inputs
#define inputs
#include <typeinfo>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <cstring>
#include <queue>
#include <stack>
#include <algorithm>
#include <string>
#include <list>
#include <sstream>
extern std::vector<std::string> split(std::string s, std::string ch = " ", std::string except = "");
extern std::string strip(std::string s, std::string ch = " ");


template <class T1, class T2>
std::istream& operator >> (std::istream& in, std::pair<T1,T2>& pair){
	std::string s;
    std::getline(in, s);
    if(s == "{}")
    	return in;
    std::string s1(s.begin()+1,s.end()-1);
    auto vals = split(s1, ",");
    std::stringstream st1, st2;
    st1 << vals[0];
    st2 << vals[1];
    T1 fir;
    T2 sec;
    st1 >> fir;
    st2 >> sec;
    pair.first = fir;
    pair.second = sec;
    return in;
}

template <class T>
std::istream& operator >> (std::istream& in, std::vector<T>& v){
    std::string inp;
    std::getline(in, inp);
    if(inp.size() == 0)
    	return in;
    auto vals = split(inp);
    for(int i = 0; i < vals.size(); ++i){
    	std::stringstream stream;
    	stream << vals[i];
    	T x;
    	stream >> x;
    	v.push_back(x);
    }
    return in;
}

#endif