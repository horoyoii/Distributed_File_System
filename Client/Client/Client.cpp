#include"headers.h"
#include"Handler.h"


void sendFile();



int main(void) {
	
	Handler *handler = new Handler();
	
	
	if (handler->TryLogin()) {
		cout << "���� ����" << endl;
		// 2) File Trnasfer ===================================
		cout << "============= send the File =============" << endl;
		sendFile();
	}
	else {
		cout << "���� ����" << endl;
	}









	system("pause");
	return 0;
}








void sendFile() {

	cout << "=================" << endl;
	cout << " ���� ���� " << endl;
	cout << "=================" << endl;


	// 1) IP & PORT
	string *IP = new string("127.0.0.1:1000");


	// 2) File Path
	string FILE_PATH;
	getline(cin, FILE_PATH, '\n');
	
	
	// 3) ���� ���� ��ü ����
	io_context io_con;
	FileTcpClient client(io_con, *IP, FILE_PATH);

	io_con.run();



}