#define _CRT_SECURE_NO_WARNINGS
#include"headers.h"
#include"Handler.h"
#include <ctime>

/**
* Created by Horoyoii on 19.04.02
*/



int main(void) {
	
	Handler *handler = new Handler();
	
	// ====================================
	// ������ ���丮�� ���� ���� get 

	// ���� interval���� �ش� ���丮�� ��ĳ��
	//handler->sendFile("path");
	//handler->StartScan();
	
		


	

	// ====================================
	
	if (handler->TryLogin()) {
		cout << "���� ����" << endl;


		// 1) ��ĵ �����ϱ� 
		handler->StartScan();

		// 2) File Trnasfer ===================================
		cout << "============= send the File =============" << endl;
		
		//sendFile();
		handler->ShowAllInfo();
	}
	else {
		cout << "���� ����" << endl;
	}









	system("pause");
	return 0;
}








