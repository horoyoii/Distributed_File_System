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
	void setDateInfo(string path, string time); // MyUserTcpClient���� ȣ��Ǿ�, �α��� ���� �� �ް� �Ǵ� ������ ������ �����Ѵ�.
	void showAllData();

};