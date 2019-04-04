#include"Handler.h"

Handler::Handler(){
	dataFromServer = new DataFromServer();

}

bool Handler::TryLogin(){
	// 1) User Authentification ===================================
	string userPW;
	cout << "����� �̸� �Է� : ";
	getline(std::cin, userName, '\n');
	cout << "����� pw �Է� : ";
	getline(std::cin, userPW, '\n');
	sendUser(userName, userPW);

	if (accResult) {

	}

	return accResult;
}


void Handler::sendUser(string userName, string userPW) {
	try {
		cout << "== ����� ���� ���� ==" << endl;

		// 1) IP & PORT
		string *IP = new string("127.0.0.1:1000");

		// 2) ���� ���� ��ü ����
		io_context io_con;
		MyUserTcpClient client(io_con, *IP, userName, userPW, dataFromServer);

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

	// while(2�ð�) ����?? 
	
	Scan();

	
}



void Handler::Scan() {
	boost::filesystem::directory_iterator end;
	int cnt = 0;


	// Handler::dataFromServer �� ������ ��ĵ�� ���� ���丮�� ���� ������ ���Ͽ� 
	// ���ε������� �Ǵ��Ѵ�.

	dataFromServer->showAllData();
	

	cout << " == Ŭ���̾�Ʈ �� ���丮 ���� == " << endl;
	for (boost::filesystem::directory_iterator iterator("C:\\Users\\user\\Desktop\\place"); iterator != end; iterator++) {
		cout << iterator->path().leaf() << "\n";
		string pathh = "C:\\Users\\user\\Desktop\\place\\";
		pathh += iterator->path().leaf().string();
		boost::filesystem::path p{ pathh };
		std::time_t t = boost::filesystem::last_write_time(p);


		// ���ϸ� ����
		string fileName;
		size_t pos = pathh.find_last_of('\\');
		if (pos != string::npos) {
			fileName = pathh.substr(pos + 1);
			cout << "���ϸ� ���� : " << fileName << endl;
		}

		// �� ���� �� update �Ǿ��ٸ� FileTcpClilent ����
		if (dataFromServer->getDateInfo(fileName) == "none") { // �������� �޾ƿ� ������ ��ġx
			cout << fileName << "�� ������ ����ȭ���� �ʾҴ�." << endl;
			sendFile(pathh);
		}
		else {
			cout << fileName << "�� ������ ����ȭ�Ǿ��ִ�." << endl;
		}



		/*
		if (t != getFileTime(iterator->path().leaf().string())) {
			sendFile(this path~);
		}
		*/
		

		std::cout << "���ϰ�� : " << pathh << endl;
		std::cout << std::ctime(&t) << '\n';
	}
	cout << " =======================================" << endl;



}



void Handler::sendFile(string file_path) {

	cout << "=================" << endl;
	cout << " ���� ���� " << endl;
	cout << "=================" << endl;


	// 1) IP & PORT
	string *IP = new string("127.0.0.1:1000");


	// 2) File Path
	string FILE_PATH;
	//getline(cin, FILE_PATH, '\n');


	// 3) ���� ���� ��ü ����
	io_context io_con;
	FileTcpClient client(io_con, *IP, file_path);

	io_con.run();



}

