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

void Handler::StartScan() {


	// Init 
   /**
   * 서버로부터 기존의 정보 받아오기
   */
	getFileListFromServer();



	// while(2시간) 마다?? 
		Scan();

	
}



void Handler::Scan() {
	boost::filesystem::directory_iterator end;
	int cnt = 0;

	for (boost::filesystem::directory_iterator iterator("C:\\Users\\user\\Desktop\\place"); iterator != end; iterator++) {
		cout << iterator->path().leaf() << "\n";
		string pathh = "C:\\Users\\user\\Desktop\\place\\";
		pathh += iterator->path().leaf().string();
		boost::filesystem::path p{ pathh };
		std::time_t t = boost::filesystem::last_write_time(p);


		// 비교 수행 후 update 되었다면 FileTcpClilent 수행
		
		/*
		if (t != getFileTime(iterator->path().leaf().string())) {
			sendFile(this path~);
		}
		*/
		

		std::cout << "파일경로 : " << pathh << endl;
		std::cout << std::ctime(&t) << '\n';
	}
}



void Handler::sendFile(string file_path) {

	cout << "=================" << endl;
	cout << " 파일 전송 " << endl;
	cout << "=================" << endl;


	// 1) IP & PORT
	string *IP = new string("127.0.0.1:1000");


	// 2) File Path
	string FILE_PATH;
	getline(cin, FILE_PATH, '\n');


	// 3) 파일 전송 객체 생성
	io_context io_con;
	FileTcpClient client(io_con, *IP, FILE_PATH);

	io_con.run();



}

void Handler::getFileListFromServer() {
	// 받아온 정보를 hash map 등에 저장한다.
	dataFromServer = new DataFromServer();
	dataFromServer->Init();



}