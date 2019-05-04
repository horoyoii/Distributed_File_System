#pragma once
#include"pch.h"

// MySQL port 3306

typedef std::string TIME_TYPE;
using namespace std;


// db 콘솔 접근 : C:\Program Files\MySQL\MySQL Server 8.0\bin>mysql -u root -p mydb

/*
DB name : MyDB
Table Info 

 == Table User ==
uid pk, 
id varchar(20), 
pw varchar(20)

 == Table FileInfo == 
uid fk references User,
name varchar(50),
lastudpatetime varchar(50)





*/


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





// 데이터베이스에 대한 인터페이스를 제공하는 것이다.
class DataBaseServ {
private:
	MYSQL Conn;
	MYSQL* ConnPtr = NULL;
	MYSQL_RES* result;
	MYSQL_ROW row;
	int Stat;



	// =====
	std::vector<ITEM> itemList;
	int itemCnt;
	ofstream outFile;
	ifstream inFile;



public:
	DataBaseServ();
	string QeuryUserInfo(string id); // pw를 반환
	
	void INSERT_FILE_INFO(string uid, string file_name, string last_update_time);
	void UPDATE_FILE_INFO(string uid, string file_name, string last_update_time);

	
	void INSERT(string fileName, string FileSize, string FilePath, string Time);
	void DELETES();
	void UPDATE(string fileName, string FileSize, string FilePath, string Time);
	void SHOWALL();
	void getAllItemInfo(ostream &requestStream, string uid);
	int HowManyItem(string uid);

};
