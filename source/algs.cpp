#include <algs.h>
int find(const std::string& s,const char s0){
    for(int i = 0; i < s.size(); ++i){
        if(s[i] == s0)
            return i;
    }
    return -1;
}
int find(const std::string& s,const std::string& s0){
    if(s.size() < s0.size())
        return -1;
    for(int i = 0; i < s.size() - s0.size() + 1; ++i){
        std::string tmp(s.begin() + i, s.begin() + i + s0.size());
        if(tmp == s0)
            return i;
    }
    return -1;
}
int find(const std::vector<std::string>& v, const char* s){
	for(int i = 0; i < v.size(); ++i){
		if(v[i] == s)
			return i;
	}
	return -1;
}