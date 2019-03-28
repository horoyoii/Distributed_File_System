#include "MyUserTcpClient.h"


MyUserTcpClient::MyUserTcpClient(boost::asio::io_context& io_context, const std::string& server, const std::string& FILE_PATH)
	:resolver(io_context), socket(io_context) {

	size_t pos = server.find(':');
	if (pos == string::npos) {
		return;
	}
	string Port = server.substr(pos + 1);
	string serverIP = server.substr(0, pos);

	
	string body = "bodyss";
	


	// 먼저 서버에 파일의 경로와 파일의 크기를 전송
	cout << "파일 정보 전송" << endl;
	std::ostream requestStream(&request);
	requestStream << "enter" << "\n" << FILE_PATH << "\n" << body << "\n\n";

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

		// 2) 결과값을 받고
		

	}
	else {
		cout << "전송오류" << endl;
		cout << err.message() << endl;
	}
}


void MyUserTcpClient::handleWrite(const boost::system::error_code &err) {
	// call back func이다. == 전부 전송하면 이 함수가 호출된다. 그렇다면 여기서 read를 쓰면 될거시다.ㅇ/??
	cout << "D : handleWrite called in MyUser" << endl;
	if (!err) {
		cout << "read from server" << endl;
		boost::asio::streambuf requestBuf;
		boost::asio::async_read_until(socket, requestBuf, "\n\n",
			boost::bind(&MyUserTcpClient::TestCallback,
				this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));



	}
	else {
		cout << "전송 오류" << endl;
		cout << "error : " << err.message() << endl;
	}
}

void MyUserTcpClient::TestCallback(const boost::system::error_code& err, std::size_t bytesTransferred) {
	cout << "내용을 받았습니다. from server." << endl;
}