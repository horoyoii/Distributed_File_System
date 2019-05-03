#pragma once
#include"pch.h"

// MySQL port 3306

typedef std::string TIME_TYPE;
using namespace std;

class ITEM {
public:
	std::string fileName;
	std::string FileSize;
	std::string FilePath;
	std::string FileLatestUpdateTime;

	ITEM(string fileName, string FileSize, string FilePath, string Time)
		:fileName(fileName), FileSize(FileSize), FilePath(FilePath), FileLatestUpdateTime(Time){
		
	}


};



// �����ͺ��̽��� ���� �������̽��� �����ϴ� ���̴�.
class DataBaseServ {
private:
		


	//TODO : MySQL ����� �ؽ������� �ٲٱ�...
	ofstream outFile;
	ifstream inFile;

	// =====
	std::vector<ITEM> itemList;
	int itemCnt;



public:
	DataBaseServ();
	
	void INSERT(string fileName, string FileSize, string FilePath, string Time);
	void DELETES();
	void UPDATE(string fileName, string FileSize, string FilePath, string Time);
	void SHOWALL();
	void getAllItemInfo(ostream &requestStream);
	int HowManyItem();

};
