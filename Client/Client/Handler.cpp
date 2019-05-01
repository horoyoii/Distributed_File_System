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
	while (true) {
		cout << "\n\n=== == == = == = 스캔 시작 = = === = == = ===  =" << endl << endl;
		Scan();

		Sleep(10000); // 10초 단위로 스캔한다.
	}

	
}



void Handler::Scan() {
	boost::filesystem::directory_iterator end;
	int cnt = 0;


	// Handler::dataFromServer 의 정보와 스캔된 현재 디렉토리의 파일 정보를 비교하여 
	// 업로드할지를 판단한다.

	//dataFromServer->showAllData();
	

	cout << " == 클라이언트 측 디렉토리 정보 == " << endl;
	for (boost::filesystem::directory_iterator iterator("C:\\Users\\user\\Desktop\\place"); iterator != end; iterator++) {
		//cout << iterator->path().leaf() << "\n";
		string pathh = "C:\\Users\\user\\Desktop\\place\\";
		pathh += iterator->path().leaf().string();
		boost::filesystem::path p{ pathh };
		std::time_t last_update_time = boost::filesystem::last_write_time(p);
		string last_update_time_toString = ctime(&last_update_time);

		// 파일명만 추출
		string fileName;
		size_t pos = pathh.find_last_of('\\');
		if (pos != string::npos) {
			fileName = pathh.substr(pos + 1);
		}

		// 비교 수행 후 update 되었다면 FileTcpClilent 수행
		string result = dataFromServer->getDateInfo(fileName, last_update_time_toString);

		if (result == "none") { // 서버에서 받아온 정보와 일치x
			cout << fileName << "이 서버에 동기화되지 않았다." << endl;
			sendFile(pathh, true);

			// 동기화되지 않았을 때의 트랜젝션은 ...??
			
			// 1) 기존 파일이 업데이트된 경우
			dataFromServer->updateDateInfo(fileName, last_update_time_toString);
		}
		else if(result == "ok"){
			cout << fileName << "이 서버에 동기화되어있다." << endl;
		}
		else if (result == "new") {
			cout << fileName <<"새로운 파일이 추가되었다." << endl;
			sendFile(pathh);
			dataFromServer->setDateInfo(fileName+"\r", last_update_time_toString);
		}



		/*
		if (t != getFileTime(iterator->path().leaf().string())) {
			sendFile(this path~);
		}
		*/
		

		//std::cout << "파일경로 : " << pathh << endl;
		std::cout << std::ctime(&last_update_time) << '\n';
	}
	cout << " =======================================" << endl;



}

void Handler::SendAllFiles(){

	boost::filesystem::directory_iterator end;
	int cnt = 0;


	// Handler::dataFromServer 의 정보와 스캔된 현재 디렉토리의 파일 정보를 비교하여 
	// 업로드할지를 판단한다.

	dataFromServer->showAllData();


	cout << " == 모든 파일 보내기 == " << endl;
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
		sendFile(pathh);
		
	}
	cout << " =======================================" << endl;



}



void Handler::sendFile(string file_path, bool ForUpdate) {

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
	FileTcpClient client(io_con, *IP, file_path, ForUpdate);

	io_con.run();



}



void Handler::SendOneFileForTest(string fileFullPath) {
	cout << "파일 하나 전송" << endl;
	string path;
	cin >> path;
	sendFile(path);
}