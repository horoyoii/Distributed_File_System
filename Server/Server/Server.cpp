#include"header.h"


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
			/*
			cout << "���丮 ����" << endl;
			boost::filesystem::directory_iterator end;
			for (boost::filesystem::directory_iterator iterator(boost::filesystem::current_path()); iterator != end; iterator++) {
				cout << iterator->path().leaf() << endl;
			}
			*/
			tcpServer *myTcpServer = new tcpServer(1000);
			delete myTcpServer;
			//system("pause");

		}


	}
	catch (exception &e) {
		cout << e.what() << endl;
	}


}


int main(void) {

	cout << "Server Run" << endl;
	operate();



	system("pause");
	return 0;
}