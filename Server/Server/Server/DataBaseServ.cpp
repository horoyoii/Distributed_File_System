#include"DataBaseServ.h"

using namespace std;




DataBaseServ::DataBaseServ(){
	itemList;
	itemCnt = 0;

}

void DataBaseServ::INSERT(string fileName, string FileSize, string FilePath, string Time){
	ITEM item(fileName, FileSize, FilePath, Time);
	itemList.push_back(item);
	itemCnt++;
}

void DataBaseServ::DELETES(){


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
