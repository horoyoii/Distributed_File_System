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
		cout << "���� ����" << endl;
		handler->StartScan();

		


	}
	else {
		cout << "���� ����" << endl;
	}








	cout << "�ý��� ����" << endl;
	system("pause");
	return 0;
}








