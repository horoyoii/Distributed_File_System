#include"DataFromServer.h"

DataFromServer::DataFromServer(){

}

void DataFromServer::Init(){
	// �������� �����޾ƿ���
	


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
