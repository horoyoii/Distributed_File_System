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
	// 지정된 디렉토리의 파일 정보 get 

	// 일정 interval마다 해당 디렉토리를 스캐닝
	//handler->sendFile("path");
	//handler->StartScan();
	
		


	

	// ====================================
	
	if (handler->TryLogin()) {
		cout << "인증 성공" << endl;


		// 1) 스캔 시작하기 
		handler->StartScan();

		// 2) File Trnasfer ===================================
		cout << "============= send the File =============" << endl;
		
		//sendFile();
		handler->ShowAllInfo();
	}
	else {
		cout << "인증 실패" << endl;
	}









	system("pause");
	return 0;
}








