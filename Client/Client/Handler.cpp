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

void Handler::StartScan() {


	// Init 
   /**
   * �����κ��� ������ ���� �޾ƿ���
   */
	getFileListFromServer();



	// while(2�ð�) ����?? 
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


		// �� ���� �� update �Ǿ��ٸ� FileTcpClilent ����
		
		/*
		if (t != getFileTime(iterator->path().leaf().string())) {
			sendFile(this path~);
		}
		*/
		

		std::cout << "���ϰ�� : " << pathh << endl;
		std::cout << std::ctime(&t) << '\n';
	}
}



void Handler::sendFile(string file_path) {

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

void Handler::getFileListFromServer() {
	// �޾ƿ� ������ hash map � �����Ѵ�.
	dataFromServer = new DataFromServer();
	dataFromServer->Init();



}