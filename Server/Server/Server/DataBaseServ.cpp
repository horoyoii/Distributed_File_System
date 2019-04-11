#include"DataBaseServ.h"

using namespace std;




DataBaseServ::DataBaseServ(){
	itemList;
	itemCnt = 0;
	cout << "DB con called" << endl;
	//inFile.open("db.txt", ios::in | ios::binary);

}

void DataBaseServ::INSERT(string fileName, string FileSize, string FilePath, string Time){
	//ITEM item(fileName, FileSize, FilePath, Time);
	//itemList.push_back(item);
	//itemCnt++;
	outFile.open("db.txt", fstream::out | fstream::app); // 이어 쓰기
	outFile << fileName << endl; 
	outFile << FileSize << endl;
	outFile << FilePath << endl;
	outFile << Time << endl;
	outFile << "==" << endl;
	outFile.close();
}

void DataBaseServ::DELETES(){


}

int DataBaseServ::HowManyItem() {
	inFile.open("db.txt", ios::in | ios::binary);
	string buffer;
	int cnt = 0;
	while (inFile.peek() != EOF) {
		getline(inFile, buffer);
		cnt++;
	}

	inFile.close();
	return cnt / 5;
}

void DataBaseServ::getAllItemInfo(ostream &requestStream) {
	inFile.open("db.txt", ios::in | ios::binary);
	string buffer;
	while (inFile.peek() != EOF) {
		getline(inFile, buffer);
		if(buffer != "==")
			requestStream << buffer << "\n";
	}

	inFile.close();
}


void DataBaseServ::UPDATE(string fileName, string FileSize, string FilePath, string Time){

	vector<ITEM>::iterator it;
	for (it = itemList.begin(); it != itemList.end(); ++it) {
		if ((*it).fileName == fileName) {
			(*it).FileSize = FileSize;
			(*it).FileLatestUpdateTime = Time;

			cout << fileName<<" : 데이터베이스 업데이터 완료" << endl;
			break;
		}

	}
	
}

void DataBaseServ::SHOWALL() {
	cout << "SHOW ALL ======" << endl;
	vector<ITEM>::iterator it;
	int cnt = 1;
	for (it = itemList.begin(); it != itemList.end(); ++it) {		
		cout << cnt++ << " : " << (*it).fileName << " - " << (*it).FileLatestUpdateTime << endl;

	}
	cout << "===================" << endl;
}
