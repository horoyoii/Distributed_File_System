#include"tcpConnection.h"
#include"DataBaseServ.h"


using namespace std;


tcpConnection::tcpConnection(boost::asio::io_context& io_context)
	:mySocket(io_context) {
}


//클라이언트의 요청을 처리하기 시작한다.
void tcpConnection::start() {
	boost::asio::async_read_until(mySocket, requestBuf, "\n\n",
		boost::bind(&tcpConnection::handleReadRequest,
			shared_from_this(), boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));


}

void tcpConnection::handleResponse(const boost::system::error_code &err) {
	cout << "내용을 보냈습니다." << endl;
}

void tcpConnection::handleReadRequest(const boost::system::error_code& err, std::size_t bytesTransferred) {
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
		string userPW;
		// '\n' 을 딜리미터로 하여 클라이언트에서 보낸다.
		requestStream >> userName;
		requestStream >> userPW;
		requestStream.read(buf.c_array(), 2);
		cout << " == 사용자 접속 ==" << endl;
		cout << "이름 : " << userName << "  pw : " << userPW << endl;


		userUID = DataBaseServ::getInstance()->getUserUid(userName);
		// RESPONSE
		std::ostream requestStream(&AccessResultRespon);

		// 비밀번호 대조
		if (userPW == DataBaseServ::getInstance()->QeuryUserInfo(userName)) {
			requestStream << "true" << "\n";
			requestStream << DataBaseServ::getInstance()->HowManyItem(userUID) << "\n";

			DataBaseServ::getInstance()->getAllItemInfo(requestStream, userUID);
			requestStream << "\n\n";
		}
		else
			requestStream << "false" << "\n\n";


		// 성공 시 유저의 서버 디렉토리 정보를 보내준다.
		boost::asio::async_write(mySocket, AccessResultRespon,
			boost::bind(&tcpConnection::handleWrite, shared_from_this(),
				boost::asio::placeholders::error));

	}

	if (instruction == "fileSend" || instruction == "fileUpdate") {
		// 사용자 식별 
		

		cout << "파일을 다운받는 중 from Server" << endl;
		std::istream requestStream(&requestBuf);
		string filePath, LatestUpdateTime;
		string temp;
		requestStream >> userID;
		getline(requestStream, temp, '\n');
		requestStream >> filePath;
		getline(requestStream, temp, '\n');
		requestStream >> fileSize;
		getline(requestStream, temp, '\n');
		getline(requestStream, LatestUpdateTime, '\n');
		requestStream.read(buf.c_array(), 1); // header의 끝은 '\n\n' 였다. 그래서 '\n'을 한번 더 빼준다.

		// 파일 정보 출력
		cout << " == 파일 정보 == FROM " <<userID<<endl;
		cout << filePath << " 크기 : " << fileSize << "bytes" << endl;
		cout << "update time : " << LatestUpdateTime << endl;


		// 파일명 추출
		size_t pos = filePath.find_last_of('\\');
		if (pos != string::npos) {
			filePath = filePath.substr(pos + 1);
			cout << "파일명 추출 : " << filePath << endl;
		}
	

		userUID = DataBaseServ::getInstance()->getUserUid(userID);

		// == == 1) 파일 시스템에 저장
		// 특정 디렉토리에 저장하기
		string dir_path = "user/"+userID+"/" + filePath;
		outputFile.open(dir_path.c_str(), std::ios_base::binary);

		// == == 2) 데이터베이스에 저장
			//파일명 - 가장 최근 업데이트 시간 을 데이터베이스에 저장한다.
		if (instruction == "fileSend") {
			DataBaseServ::getInstance()->INSERT_FILE_INFO(userUID, filePath, LatestUpdateTime);
		}
		else {
			DataBaseServ::getInstance()->UPDATE_FILE_INFO(userUID, filePath, LatestUpdateTime);
		}

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

	if (instruction == "getDataList") {
		// DB에서 정보를 가져와 전송한다리


	}

}


void tcpConnection::handleWrite(const boost::system::error_code&) {
	cout << "보내기 완료" << endl;
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


