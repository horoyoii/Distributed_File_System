#include"Handler.h"

Handler::Handler()
{
}

bool Handler::TryLogin(){
	// 1) User Authentification ===================================
	string userPW;
	cout << "사용자 이름 입력 : ";
	getline(std::cin, userName, '\n');
	cout << "사용자 pw 입력 : ";
	getline(std::cin, userPW, '\n');
	sendUser(userName, userPW);


	return accResult;
}


void Handler::sendUser(string userName, string userPW) {
	try {
		cout << "== 사용자 정보 전송 ==" << endl;

		// 1) IP & PORT
		string *IP = new string("127.0.0.1:1000");

		// 2) 파일 전송 객체 생성
		io_context io_con;
		MyUserTcpClient client(io_con, *IP, userName, userPW);

		io_con.run();
		cout <<"io_context while 종료"<< endl;
		// 한번의 통신 트랜잭션이 끝나면... run이 스탑되고 아래의 프로시져가 수행된다.

		accResult = client.getAccResult();
	
	}
	catch (exception& e) {
		cout << "== 전송 오류 ==" << endl;
		cerr << e.what() << endl;
	}
}

