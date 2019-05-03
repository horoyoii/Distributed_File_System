#include "MyUserTcpClient.h"



MyUserTcpClient::MyUserTcpClient(boost::asio::io_context& io_context, const std::string& server, const std::string& userName, const std::string& userPW, DataFromServer *dataFromServer)
	:resolver(io_context), socket(io_context) {

	this->dataFromServer = dataFromServer;

	size_t pos = server.find(':');
	if (pos == string::npos) {
		return;
	}
	string Port = server.substr(pos + 1);
	string serverIP = server.substr(0, pos);


	// 먼저 서버에 파일의 경로와 파일의 크기를 전송
	cout << "유저 정보 전송" << endl;
	std::ostream requestStream(&request);
	requestStream << "enter" << "\n" << userName << "\n" << userPW << "\n\n";

	cout << "메타데이터 요청 크기 : " << request.size() << endl;

	// procedure : resolving -> binding -> connecting -> file sending
	boost::asio::ip::tcp::resolver::query query(serverIP, Port);
	resolver.async_resolve(query, boost::bind(&MyUserTcpClient::handleResolve, this,
		boost::asio::placeholders::error, boost::asio::placeholders::iterator));






}

void  MyUserTcpClient::handleResolve(const boost::system::error_code& err, tcp::resolver::iterator  myIterator) {
	cout << "D : handleResolve called in MyUser" << endl;
	// 에러 발생하지 않은 경우
	if (!err) {
		tcp::endpoint endpoint = *myIterator; // endpoint에는 "127.0.0.1:1000" 가 저장된다.
		socket.async_connect(endpoint,
			boost::bind(&MyUserTcpClient::handleConnect, this, boost::asio::placeholders::error, ++myIterator));

	}
	else {
		cout << " 세팅 오류  " << endl;
		cout << err.message() << endl;
	}
}


void MyUserTcpClient::handleConnect(const boost::system::error_code& err, tcp::resolver::iterator myIterator) {
	cout << "D : handleConnect called in MyUser" << endl;
	if (!err) {

		// 1) 유저 정보 전송하고
		cout << "파일 전송" << endl;

		    // async_write() : 버퍼의 내용물을 전부 전송하면 callback함수를 호출한다.(여기서는 handlewrite)
		async_write(socket, request, 
			boost::bind(&MyUserTcpClient::handleWrite, this,
				boost::asio::placeholders::error));

		

	}
	else {
		cout << "전송오류" << endl;
		cout << err.message() << endl;
	}
}


void MyUserTcpClient::handleWrite(const boost::system::error_code &err) {
	if (!err) {
		boost::asio::async_read_until(socket, ResponseBuf, "\n\n",
			boost::bind(&MyUserTcpClient::ResponseListFromServer,
				this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));



	}
	else {
		cout << "전송 오류" << endl;
		cout << "error : " << err.message() << endl;
	}
}

void MyUserTcpClient::ResponseListFromServer(const boost::system::error_code& err, std::size_t bytesTransferred) {

	std::istream requestStream(&ResponseBuf);

	// 요청 정보 처리 : 헤더 처리 !!!!!!!!!!!!!!!!!
	string AccessResult;
	requestStream >> AccessResult;
	string responseTOKEN;
	int cnt;
	cout << "[인증성공]내용을 받았습니다. from server  : " << AccessResult << endl;
	
	if (AccessResult == "true") {
		accResult = true;
		requestStream >> responseTOKEN;
		//getline(requestStream, responseTOKEN, '\n');
		cnt = atoi(responseTOKEN.c_str()); // 총몇개인지 우선 알아낸다.
		getline(requestStream, responseTOKEN, '\n'); // '\n' 하나 빼준다.
		// 받아온 정보를 따로 저장한다.
		string name, UpdateDate;
		//cout << "========== 나의 디렉토리 정보 cnt : "<<cnt<<"======================" << endl;
		for(int i=1;i<=cnt;i++) {
				getline(requestStream, responseTOKEN, '\n');
				//cout << " name : " << responseTOKEN << endl;
				name = responseTOKEN;
				getline(requestStream, responseTOKEN, '\n');
				//cout << " size : " << responseTOKEN << endl;
				getline(requestStream, responseTOKEN, '\n');
				//cout << " path : " << responseTOKEN << endl;
				getline(requestStream, responseTOKEN, '\n');
				//cout << " date : " << responseTOKEN << endl;
				UpdateDate = responseTOKEN;
				getline(requestStream, responseTOKEN, '\n');
				
				dataFromServer->setDateInfo(name, UpdateDate);
		}
	
	}
	else
		accResult = false;
}

bool MyUserTcpClient::getAccResult()
{
	cout << "================서버로부터 받은 데이터 목록================" << endl;
	dataFromServer->showAllData();
	return accResult;
}
