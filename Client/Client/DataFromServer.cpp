#include"DataFromServer.h"

DataFromServer::DataFromServer(){

}

void DataFromServer::Init(){
	
	


}

// key 값인 파일명은  "파일명" 으로 저장되어있다.
string DataFromServer::getDateInfo(string path, string UpdateTime){
	if (FileData.find(path) == FileData.end()) { // not found
		return "new";
	}
	else {
		string timeFromServer = FileData.find(path)->second;
		//timeFromServer = timeFromServer.substr(0, timeFromServer.size() - 1);

		if (!timeFromServer.compare(UpdateTime.substr(0, UpdateTime.size()-1)))
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

void DataFromServer::updateDateInfo(string name, string time){
	FileData.find(name)->second = time;

}
