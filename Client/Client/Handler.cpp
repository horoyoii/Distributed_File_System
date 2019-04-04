#include"Handler.h"

Handler::Handler(){
	dataFromServer = new DataFromServer();

}

bool Handler::TryLogin(){
	// 1) User Authentification ===================================
	string userPW;
	cout << "사용자 이름 입력 : ";
	getline(std::cin, userName, '\n');
	cout << "사용자 pw 입력 : ";
	getline(std::cin, userPW, '\n');
	sendUser(userName, userPW);

	if (accResult) {

	}

	return accResult;
}


void Handler::sendUser(string userName, string userPW) {
	try {
		cout << "== 사용자 정보 전송 ==" << endl;

		// 1) IP & PORT
		string *IP = new string("127.0.0.1:1000");

		// 2) 파일 전송 객체 생성
		io_context io_con;
		MyUserTcpClient client(io_con, *IP, userName, userPW, dataFromServer);

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
	/*
	int cnt = 0;
	while (true) {
		if (cnt % 100000)
			Scan();

		cnt++;
		Sleep(1000);
	}
	*/

	// while(2시간) 마다?? 
	
	Scan();

	
}



void Handler::Scan() {
	boost::filesystem::directory_iterator end;
	int cnt = 0;


	// Handler::dataFromServer 의 정보와 스캔된 현재 디렉토리의 파일 정보를 비교하여 
	// 업로드할지를 판단한다.

	dataFromServer->showAllData();
	

	cout << " == 클라이언트 측 디렉토리 정보 == " << endl;
	for (boost::filesystem::directory_iterator iterator("C:\\Users\\user\\Desktop\\place"); iterator != end; iterator++) {
		cout << iterator->path().leaf() << "\n";
		string pathh = "C:\\Users\\user\\Desktop\\place\\";
		pathh += iterator->path().leaf().string();
		boost::filesystem::path p{ pathh };
		std::time_t t = boost::filesystem::last_write_time(p);


		// 파일명만 추출
		string fileName;
		size_t pos = pathh.find_last_of('\\');
		if (pos != string::npos) {
			fileName = pathh.substr(pos + 1);
			cout << "파일명 추출 : " << fileName << endl;
		}

		// 비교 수행 후 update 되었다면 FileTcpClilent 수행
		if (dataFromServer->getDateInfo(fileName) == "none") { // 서버에서 받아온 정보와 일치x
			cout << fileName << "이 서버에 동기화되지 않았다." << endl;
			sendFile(pathh);
		}
		else {
			cout << fileName << "이 서버에 동기화되어있다." << endl;
		}



		/*
		if (t != getFileTime(iterator->path().leaf().string())) {
			sendFile(this path~);
		}
		*/
		

		std::cout << "파일경로 : " << pathh << endl;
		std::cout << std::ctime(&t) << '\n';
	}
	cout << " =======================================" << endl;



}



void Handler::sendFile(string file_path) {

	cout << "=================" << endl;
	cout << " 파일 전송 " << endl;
	cout << "=================" << endl;


	// 1) IP & PORT
	string *IP = new string("127.0.0.1:1000");


	// 2) File Path
	string FILE_PATH;
	//getline(cin, FILE_PATH, '\n');


	// 3) 파일 전송 객체 생성
	io_context io_con;
	FileTcpClient client(io_con, *IP, file_path);

	io_con.run();



}

