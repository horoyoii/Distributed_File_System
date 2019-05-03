#define _CRT_SECURE_NO_WARNINGS
#include"headers.h"
#include"Handler.h"
#include <ctime>

/**
* Created by Horoyoii on 19.04.02
*/



int main(void) {
	
	Handler *handler = new Handler();
		
	if (handler->TryLogin()) {
		cout << "인증 성공" << endl;
		handler->StartScan();

		


	}
	else {
		cout << "인증 실패" << endl;
	}








	cout << "시스템 종료" << endl;
	system("pause");
	return 0;
}








