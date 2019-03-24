#include"headers.h"



void sendFile();
void sendUser(string userName, string userInfo);




int main(void) {
	
	// 1) User Authentification ===================================
	string userName;
	string userInfo;
	cout << "����� �̸� �Է� : ";
	getline(std::cin, userName, '\n');
	cout << "����� ���� �Է� : ";
	getline(std::cin, userInfo, '\n');
	sendUser(userName, userInfo);

	// 2) File Trnasfer ===================================
	cout << "============= send the File =============" << endl;
	sendFile();




	system("pause");
	return 0;
}





void sendUser(string userName, string userInfo) {
	try {
		cout << "== ����� ���� ���� ==" << endl;
		string *IP = new string("127.0.0.1:1000");
		boost::asio::io_context io_context;
		userTcpClient(io_context, *IP, userName, userInfo);
		io_context.run();

		cout << " == ����� ���� ���� ���� == " << endl;

	}
	catch (exception& e) {
		cout << "== ���� ���� ==" << endl;
		cerr << e.what() << endl;
	}
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