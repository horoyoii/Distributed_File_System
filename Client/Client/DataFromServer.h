#pragma once
#include"headers.h"
//#include<hash_map>


class FileInfo {
	string FileName;
	string UpdateTime;
};

class DataFromServer {
private:
	map<string, string> FileData;



public:
	DataFromServer();
	void Init();
	string getDateInfo(string path);
	void setDateInfo(string path, string time);
	void showAllData();

};