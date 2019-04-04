#include"DataFromServer.h"

DataFromServer::DataFromServer(){

}

void DataFromServer::Init(){
	
	


}

// key 값인 파일명은  \"파일명\" 으로 저장되어있다.
string DataFromServer::getDateInfo(string path){
	if (FileData.find("\""+path+ "\"") == FileData.end()) { // not found
		return "none";
	}
	else {
		return "ok";
	}

}

void DataFromServer::setDateInfo(string path, string time){
	FileData.insert(make_pair(path, time));
}

void DataFromServer::showAllData(){
	for (auto it = FileData.begin(); it != FileData.end(); ++it) {
		cout << "key : " << it->first << " // " << it->second << endl;
	}
}
