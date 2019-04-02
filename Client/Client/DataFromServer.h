#pragma once
#include"headers.h"
#include<hash_map>


class DataFromServer {
private:




public:
	DataFromServer();
	void Init();
	string getDateInfo(string path);
	void setDateInfo(string path, time_t time);

};