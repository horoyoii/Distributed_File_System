#include"headers.h"



void sendFile();



int main(void) {
	
	// 1) User Authentification ===================================


	// 2) File Trnasfer ===================================
	cout << "============= send the File =============" << endl;
	sendFile();




	system("pause");
	return 0;
}


void sendFile() {
	cout << "=================" << endl;
	cout << " 颇老 傈价 " << endl;
	cout << "=================" << endl;


	// 1) IP & PORT
	string *IP = new string("127.0.0.1:1000");


	// 2) File Path
	string FILE_PATH;
	getline(cin, FILE_PATH, '\n');
	
	
	// 3) 颇老 傈价 按眉 积己
	io_context io_con;
	FileTcpClient client(io_con, *IP, FILE_PATH);

	io_con.run();



}