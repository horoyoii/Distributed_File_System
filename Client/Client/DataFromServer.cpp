#include"DataFromServer.h"

DataFromServer::DataFromServer(){

}

void DataFromServer::Init(){
	// 서버에서 정보받아오기
	


}

string DataFromServer::getDateInfo(string path)
{
	return string();
}

void DataFromServer::setDateInfo(string path, string time){
	FileData.insert(make_pair(path, time));
}

void DataFromServer::showAllData(){

}
