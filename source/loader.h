#pragma once
#include <string>
#include <unordered_map>
#include <memory>

template<typename Data>
class Loader {
protected:
	using Map = std::unordered_map<std::string, Data>;
	Map map;
public:
	Data& operator[](std::string name) {
		auto& data = map.find(name);
		if (data == map.end())
			return map.emplace(name, Data(name.c_str())).first->second;
		else return data->second;
	}

	void clear(){
		map.clear();
	}

};
