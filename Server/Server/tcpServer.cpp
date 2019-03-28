#include"tcpServer.h"


unsigned short port = 21;
unsigned short userCount = 0;
std::array<std::string, 50> userList;


tcpServer::tcpServer(unsigned short port)
:acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port), true){
	cout << "대기 중 입니다." << endl;

	// 지정된 포트로 새로운 연결을 accept 처리한다.
	cout << "D : accept 대기중 1 " << endl;
	ptrTcpConnection newConnection(new tcpConnection(io_context));
	cout << "D : accept 대기중 2 " << endl;
	acceptor.async_accept(newConnection->socket(),
	boost::bind(&tcpServer::handleAccept, this, newConnection, boost::asio::placeholders::error));
	cout << "D : accept 대기중 3 " << endl;

	io_context.run();



}


void tcpServer::handleAccept(ptrTcpConnection currentConnection, const boost::system::error_code& e) {
	cout << " == 연결 처리 ==" << endl;
	cout << __FUNCTION__ << " " << e << " " << e.message() << endl;
	if (!e) {
		currentConnection->start();
	}
	else {
		cout << "awefawefawef" << endl;
	}
}

tcpServer::~tcpServer() {
	// 로그아웃 : 클라이언트에서 접속 종료 시 호출된다.
	io_context.stop();
}

// ====================================================
// ====================================================
// ====================================================
// ====================================================
// ====================================================


tcpConnection::tcpConnection(boost::asio::io_context& io_context)
	:mySocket(io_context) {
	cout << "tcpConnection 생성자 호출" << endl;
}


//클라이언트의 요청을 처리하기 시작한다.
void tcpConnection::start() {
	cout << "start called" << endl;
	boost::asio::async_read_until(mySocket, requestBuf, "\n\n",
		boost::bind(&tcpConnection::handleReadRequest,
			shared_from_this(), boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

	cout << "D : read 끗" << endl;

}

void tcpConnection::handleResponse(const boost::system::error_code &err) {
	cout << "내용을 보냈습니다." << endl;
}

void tcpConnection::handleReadRequest(const boost::system::error_code& err, std::size_t bytesTransferred) {
	cout << "D : handleReadRequest called" << endl;
	if (err) {
		cout << "D : Error in handleReadRequest" << endl;
		return handleError(__FUNCTION__, err);
	}
	
	cout << " == 요청 정보  from client == " << endl;
	cout << __FUNCTION__ << endl;
	cout << "bytesTransferred : " << bytesTransferred << endl;
	cout << "요청 버퍼 크기 : " << requestBuf.size() << endl;
	std::istream requestStream(&requestBuf);

	// 요청 정보 처리 : 헤더 처리 !!!!!!!!!!!!!!!!!
	string instruction;
	requestStream >> instruction;
	if (instruction == "enter") { // 사용자 접속의 경우
		string userName;
		string userInfo;
		// '\n' 을 딜리미터로 하여 클라이언트에서 보낸다.
		requestStream >> userName;
		requestStream >> userInfo;
		requestStream.read(buf.c_array(), 2);
		cout << " == 사용자 접속 ==" << endl;
		cout << "이름 : " << userName << "  정보 : " << userInfo << endl;

		userList[userCount++] = userName;
	
		//TODO: reply 보내기 ...? 
		
		boost::asio::streambuf response;
		std::ostream responseStream(&response);

		responseStream << "enter" << "\n" << "answer" << "\n" << "Good" << "\n\n";
		cout << "응답  크기 : " << response.size() << endl;


		boost::asio::async_write(mySocket, response, 
			boost::bind(&tcpConnection::handleResponse, shared_from_this(),
				boost::asio::placeholders::error));
	
	
	}

	if (instruction == "fileSend") {
		std::istream requestStream(&requestBuf);
		string filePath;
		string temp;

		requestStream >> filePath;
		getline(requestStream, temp, '\n');
		requestStream >> fileSize;
		requestStream.read(buf.c_array(), 2);

		// 파일 정보 출력
		cout << " == 파일 정보 ==" << endl;
		cout << filePath << " 크기 : " << fileSize << "bytes" << endl;

		// 파일명 추출
		size_t pos = filePath.find_last_of('\\');
		if (pos != string::npos) {
			filePath = filePath.substr(pos + 1);
			cout << "파일명 추출 : " << filePath << endl;
		}

		outputFile.open(filePath.c_str(), std::ios_base::binary);

		if (!outputFile) {
			cout << "파일 오류 " << endl;
			cout << "failed to open" << endl;
			return;
		}

		cout << " == 파일 다운로드 중 in server == " << endl;
		do {
			requestStream.read(buf.c_array(), (std::streamsize)buf.size());
			cout << __FUNCTION__ << "파일 쓰기" << requestStream.gcount() << "bytes" << endl;
			outputFile.write(buf.c_array(), requestStream.gcount());
		} while (requestStream.gcount() > 0);

		cout << "파일 내용 다운" << endl;
		boost::asio::async_read(mySocket, boost::asio::buffer(buf.c_array(), buf.size()),
			boost::bind(&tcpConnection::handleReadFileContent,
				shared_from_this(), boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

	}
}

// 파일의 내용을 읽기
void tcpConnection::handleReadFileContent(const boost::system::error_code& err, std::size_t bytesTransferred) {

	if (bytesTransferred > 0) {
		outputFile.write(buf.c_array(), (std::streamsize)bytesTransferred);
		cout << __FUNCTION__ << "다운로드" << outputFile.tellp() << "bytes" << endl;
		// tellp() : Returns the position of the current character in the output stream.
		if (outputFile.tellp() >= (std::streamsize)fileSize)
			return;
	}

	if (err) {
		cout << "오류발생" << endl;
		return handleError(__FUNCTION__, err);
	}

	// 버퍼를 읽는다. -> Recursive
	boost::asio::async_read(mySocket, boost::asio::buffer(buf.c_array(), buf.size()),
		boost::bind(&tcpConnection::handleReadFileContent,
			shared_from_this(), boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

}


boost::asio::ip::tcp::socket&  tcpConnection::socket() {
	return mySocket;
}

void tcpConnection::handleError(const std::string& functionName, const boost::system::error_code& err) {
	cout << "Error " << endl;
	cout << __FUNCTION__ << "Error : " << functionName << "\n Why? " << err << " " << err.message() << endl;

}


