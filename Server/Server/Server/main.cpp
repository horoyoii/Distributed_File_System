#include"pch.h"
#include"tcpServer.h"

using namespace std;


void operate() {
	try {
		while (true) {
			cout << "���� ����" << endl;

			// ���� ������ ��� �����.
			/*
			cout << "���� ������ �� : " << userCount << endl;
			for (int i = 0; i < userCount; i++) {
				cout << "����� �̸� : " << userList[i] << endl;
			}
			*/
			// ���� ������ ���� ���� ���
			//for (boost::filesystem::directory_iterator iterator(boost::filesystem::current_path()); iterator != end; iterator++) {
			//cout << "���丮 ����" << endl;
			boost::filesystem::directory_iterator end;
			//for (boost::filesystem::directory_iterator iterator("user/eirc8260"); iterator != end; iterator++) {
				//cout << iterator->path().leaf() << endl;
			//}
			
			tcpServer *myTcpServer = new tcpServer(1000);
			delete myTcpServer;
		}


	}
	catch (exception &e) {
		cout << e.what() << endl;
	}


}


int main(void) {
	cout << "Server Run" << endl;
	/*
	const char *Query = "SELECT * FROM user";
	Stat = mysql_query(ConnPtr, Query);
	if (Stat != 0) {

	}

	result = mysql_store_result(ConnPtr);
	while ((row = mysql_fetch_row(result)) != NULL) {
		printf("%s %s %s\n", row[0], row[1], row[2]);
	}

	mysql_close(ConnPtr);

	system("pause");
	*/

	operate();



	system("pause");
	return 0;
}