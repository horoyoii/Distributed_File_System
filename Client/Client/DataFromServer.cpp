#include"DataFromServer.h"

DataFromServer::DataFromServer(){

}

void DataFromServer::Init(){
	
	


}

// key 값인 파일명은  \"파일명\" 으로 저장되어있다.
string DataFromServer::getDateInfo(string path, string UpdateTime){
	if (FileData.find(path+"\r") == FileData.end()) { // not found
		return "none";
	}
	else {
		string time1 = FileData.find(path+"\r")->second +"\r";
		if (!time1.compare(UpdateTime))
			return "ok";
		else
			return "none";
	}

}

void DataFromServer::setDateInfo(string name, string time){
	FileData.insert(make_pair(name, time));
}

void DataFromServer::showAllData(){
	for (auto it = FileData.begin(); it != FileData.end(); it++) {
		cout << "key : " << it->first << endl << it->second << endl;
	}
}
