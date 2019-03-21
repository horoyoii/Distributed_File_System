#include"FileTcpClient.h"



FileTcpClient::FileTcpClient(boost::asio::io_context& io_context, const std::string& server, const std::string& FILE_PATH)
:resolver(io_context), socket(io_context){

	size_t pos = server.find(':');
	if (pos == string::npos) {
		return;
	}
	string Port = server.substr(pos + 1);
	string serverIP = server.substr(0, pos);

	// open the file
	sourceFile.open(FILE_PATH.c_str(), std::ios_base::binary | std::ios_base::ate);
	if (!sourceFile) {
		std::cout << "file open error" << endl;
		return;
	}
	size_t fileSize = sourceFile.tellg(); // Returns the position of the current character in the input stream.
	cout << "현재 파일 크기 :  " << fileSize <<"bytes"<< endl;
	sourceFile.seekg(0); // Set position in input sequence


	// 먼저 서버에 파일의 경로와 파일의 크기를 전송
	cout << "파일 정보 전송" << endl;
	std::ostream requestStream(&request);
	requestStream << "fileSend" << "\n" << FILE_PATH << "\n" << fileSize << "\n\n";

	cout << "메타데이터 요청 크기 : " << request.size() << endl;

	// procedure : resolving -> binding -> connecting -> file sending
	boost::asio::ip::tcp::resolver::query query(serverIP, Port);
	resolver.async_resolve(query, boost::bind(&FileTcpClient::handleResolve, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator));






}

void  FileTcpClient::handleResolve(const boost::system::error_code& err, tcp::resolver::iterator  myIterator) {
	
	// 에러 발생하지 않은 경우
	if (!err) {
		tcp::endpoint endpoint = *myIterator; // endpoint에는 "127.0.0.1:1000" 가 저장된다.
		socket.async_connect(endpoint,
			boost::bind(&FileTcpClient::handleConnect, this, boost::asio::placeholders::error, ++myIterator));

	}
	else {
		cout << " 세팅 오류  " << endl;
		cout << err.message() << endl;
	}
}


void FileTcpClient::handleConnect(const boost::system::error_code& err, tcp::resolver::iterator myIterator) {
	if (!err) {

		cout << "파일 전송" << endl;
		async_write(socket, request,
			boost::bind(&FileTcpClient::handleWrite, this, boost::asio::placeholders::error));

	}
	else {
		cout << "전송오류" << endl;
		cout << err.message() << endl;
	}
}


void FileTcpClient::handleWrite(const boost::system::error_code &err) {
	// 실제로 파일을 전송한다.
	if (!err) {
		
		if (sourceFile.eof() == false) {
			// 버퍼에 담는다.
			sourceFile.read(buf.c_array(), (std::streamsize)buf.size());
			// Excetion handling
			if (sourceFile.gcount() <= 0) {
				//Returns the number of characters extracted by the last unformatted input operation performed on the object
				cout << "파일 오류" << endl;
				cout << "read file error" << endl;
				return;
			}

			cout << sourceFile.gcount() << "bytes 전송 완료" << endl;
			cout << "전체 : " << sourceFile.tellg() << "bytes" << endl;

			// 실제 파일 전송 시작
			// 재귀적 반복
			async_write(socket,
				boost::asio::buffer(buf.c_array(), sourceFile.gcount()),
				bind(&FileTcpClient::handleWrite, this, boost::asio::placeholders::error)
			);

			if (err) {
				cout << " 전송  오류" << endl;
				cout << "send error : " << err << endl;
				return;
			}
		}
		else
			return;
	}
	else {
		cout << "전송 오류" << endl;
		cout << "error : " << err.message() << endl;
	}
}