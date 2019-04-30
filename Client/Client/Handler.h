#pragma once
#include"headers.h"
#include"DataFromServer.h"

class Handler {
private:
	string userName;
	bool accResult;
	DataFromServer *dataFromServer;


public:
	Handler();

	bool TryLogin();
	void sendUser(string userName, string userPW);
	void sendFile(string file_path);
	void StartScan();
	void Scan();
	void SendAllFiles(); // ���� ����ȭ ��...


	// Test
	void SendOneFileForTest(string fileFullPath);
	void ShowAllInfo() { // �׽�Ʈ�Լ�
		dataFromServer->showAllData();
	}

};