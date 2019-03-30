#include"Handler.h"

Handler::Handler()
{
}

bool Handler::TryLogin(){
	// 1) User Authentification ===================================
	string userPW;
	cout << "����� �̸� �Է� : ";
	getline(std::cin, userName, '\n');
	cout << "����� pw �Է� : ";
	getline(std::cin, userPW, '\n');
	sendUser(userName, userPW);


	return accResult;
}


void Handler::sendUser(string userName, string userPW) {
	try {
		cout << "== ����� ���� ���� ==" << endl;

		// 1) IP & PORT
		string *IP = new string("127.0.0.1:1000");

		// 2) ���� ���� ��ü ����
		io_context io_con;
		MyUserTcpClient client(io_con, *IP, userName, userPW);

		io_con.run();
		cout <<"io_context while ����"<< endl;
		// �ѹ��� ��� Ʈ������� ������... run�� ��ž�ǰ� �Ʒ��� ���ν����� ����ȴ�.

		accResult = client.getAccResult();
	
	}
	catch (exception& e) {
		cout << "== ���� ���� ==" << endl;
		cerr << e.what() << endl;
	}
}

