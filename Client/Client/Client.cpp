#define _CRT_SECURE_NO_WARNINGS
#include"headers.h"
#include"Handler.h"
#include <ctime>

void sendFile();



int main(void) {
	
	Handler *handler = new Handler();
	
	// ====================================
	// ������ ���丮�� ���� ���� get 
	//TODO : �̸� �������� �ֱ������� ���ε� to ����
	boost::filesystem::directory_iterator end;
	int cnt = 0;
	for (boost::filesystem::directory_iterator iterator("C:\\Users\\user\\Desktop\\place"); iterator != end; iterator++) {
		cout << iterator->path().leaf() << "\n";
		string pathh = "C:\\Users\\user\\Desktop\\place\\";
		pathh += iterator->path().leaf().string();
		boost::filesystem::path p{ pathh };
		std::time_t t = boost::filesystem::last_write_time(p);
		std::cout << std::ctime(&t) << '\n';
	}

	system("pause");

	// ====================================
	
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