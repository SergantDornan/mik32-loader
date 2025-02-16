#include <inputs.h>
extern std::vector<std::string> split(std::string s, std::string ch, std::string except){
	std::vector<std::string> res;
	std::string curr;
	bool push = true;
	for(int i = 0; i < s.size(); ++i){
		if(ch.find(s[i]) != std::string::npos && i != (s.size() - 1) && push){
			if(i != 0)
				res.push_back(curr);
			curr.clear();
			if(except.find(s[i]) != std::string::npos){
				curr += s[i];
			}
			push = false;
		}
		else if(ch.find(s[i]) != std::string::npos && i != (s.size() - 1) && !push && except.find(s[i]) != std::string::npos){
			curr += s[i];
		}
		else if(i == (s.size() - 1)){
		//	std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
			if(ch.find(s[i]) == std::string::npos)
				curr+=s[i];
			res.push_back(curr);
		}
		else if(ch.find(s[i]) == std::string::npos){
			push = true;
			curr += s[i];
		}
	}
	return res;
}

extern std::string strip(std::string s, std::string ch){
	std::string res;
	for(int i = 0; i < s.size(); ++i){
		if(ch.find(s[i]) == std::string::npos)
			res+=s[i];
	}
	return res;
}