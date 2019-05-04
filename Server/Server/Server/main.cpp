#include"pch.h"
#include"tcpServer.h"

using namespace std;


void operate() {
	try {
		while (true) {
			cout << "서버 시작" << endl;
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
	
	

	operate();



	system("pause");
	return 0;
}