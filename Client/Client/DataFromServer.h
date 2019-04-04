#pragma once
#include"headers.h"


using namespace std;

class DataFromServer {
private:
	map<string, string> FileData;



public:
	DataFromServer();
	void Init();
	string getDateInfo(string path);
	void setDateInfo(string path, string time); // MyUserTcpClient에서 호출되어, 로그인 성공 시 받게 되는 서버의 정보를 저장한다.
	void showAllData();

};