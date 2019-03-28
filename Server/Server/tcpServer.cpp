#include"tcpServer.h"


unsigned short port = 21;
unsigned short userCount = 0;
std::array<std::string, 50> userList;


tcpServer::tcpServer(unsigned short port)
:acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port), true){
	cout << "��� �� �Դϴ�." << endl;

	// ������ ��Ʈ�� ���ο� ������ accept ó���Ѵ�.
	cout << "D : accept ����� 1 " << endl;
	ptrTcpConnection newConnection(new tcpConnection(io_context));
	cout << "D : accept ����� 2 " << endl;
	acceptor.async_accept(newConnection->socket(),
	boost::bind(&tcpServer::handleAccept, this, newConnection, boost::asio::placeholders::error));
	cout << "D : accept ����� 3 " << endl;

	io_context.run();



}


void tcpServer::handleAccept(ptrTcpConnection currentConnection, const boost::system::error_code& e) {
	cout << " == ���� ó�� ==" << endl;
	cout << __FUNCTION__ << " " << e << " " << e.message() << endl;
	if (!e) {
		currentConnection->start();
	}
	else {
		cout << "awefawefawef" << endl;
	}
}

tcpServer::~tcpServer() {
	// �α׾ƿ� : Ŭ���̾�Ʈ���� ���� ���� �� ȣ��ȴ�.
	io_context.stop();
}

// ====================================================
// ====================================================
// ====================================================
// ====================================================
// ====================================================


tcpConnection::tcpConnection(boost::asio::io_context& io_context)
	:mySocket(io_context) {
	cout << "tcpConnection ������ ȣ��" << endl;
}


//Ŭ���̾�Ʈ�� ��û�� ó���ϱ� �����Ѵ�.
void tcpConnection::start() {
	cout << "start called" << endl;
	boost::asio::async_read_until(mySocket, requestBuf, "\n\n",
		boost::bind(&tcpConnection::handleReadRequest,
			shared_from_this(), boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

	cout << "D : read ��" << endl;

}

void tcpConnection::handleResponse(const boost::system::error_code &err) {
	cout << "������ ���½��ϴ�." << endl;
}

void tcpConnection::handleReadRequest(const boost::system::error_code& err, std::size_t bytesTransferred) {
	cout << "D : handleReadRequest called" << endl;
	if (err) {
		cout << "D : Error in handleReadRequest" << endl;
		return handleError(__FUNCTION__, err);
	}
	
	cout << " == ��û ����  from client == " << endl;
	cout << __FUNCTION__ << endl;
	cout << "bytesTransferred : " << bytesTransferred << endl;
	cout << "��û ���� ũ�� : " << requestBuf.size() << endl;
	std::istream requestStream(&requestBuf);

	// ��û ���� ó�� : ��� ó�� !!!!!!!!!!!!!!!!!
	string instruction;
	requestStream >> instruction;
	if (instruction == "enter") { // ����� ������ ���
		string userName;
		string userInfo;
		// '\n' �� �������ͷ� �Ͽ� Ŭ���̾�Ʈ���� ������.
		requestStream >> userName;
		requestStream >> userInfo;
		requestStream.read(buf.c_array(), 2);
		cout << " == ����� ���� ==" << endl;
		cout << "�̸� : " << userName << "  ���� : " << userInfo << endl;

		userList[userCount++] = userName;
	
		//TODO: reply ������ ...? 
		
		boost::asio::streambuf response;
		std::ostream responseStream(&response);

		responseStream << "enter" << "\n" << "answer" << "\n" << "Good" << "\n\n";
		cout << "����  ũ�� : " << response.size() << endl;


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

		// ���� ���� ���
		cout << " == ���� ���� ==" << endl;
		cout << filePath << " ũ�� : " << fileSize << "bytes" << endl;

		// ���ϸ� ����
		size_t pos = filePath.find_last_of('\\');
		if (pos != string::npos) {
			filePath = filePath.substr(pos + 1);
			cout << "���ϸ� ���� : " << filePath << endl;
		}

		outputFile.open(filePath.c_str(), std::ios_base::binary);

		if (!outputFile) {
			cout << "���� ���� " << endl;
			cout << "failed to open" << endl;
			return;
		}

		cout << " == ���� �ٿ�ε� �� in server == " << endl;
		do {
			requestStream.read(buf.c_array(), (std::streamsize)buf.size());
			cout << __FUNCTION__ << "���� ����" << requestStream.gcount() << "bytes" << endl;
			outputFile.write(buf.c_array(), requestStream.gcount());
		} while (requestStream.gcount() > 0);

		cout << "���� ���� �ٿ�" << endl;
		boost::asio::async_read(mySocket, boost::asio::buffer(buf.c_array(), buf.size()),
			boost::bind(&tcpConnection::handleReadFileContent,
				shared_from_this(), boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

	}
}

// ������ ������ �б�
void tcpConnection::handleReadFileContent(const boost::system::error_code& err, std::size_t bytesTransferred) {

	if (bytesTransferred > 0) {
		outputFile.write(buf.c_array(), (std::streamsize)bytesTransferred);
		cout << __FUNCTION__ << "�ٿ�ε�" << outputFile.tellp() << "bytes" << endl;
		// tellp() : Returns the position of the current character in the output stream.
		if (outputFile.tellp() >= (std::streamsize)fileSize)
			return;
	}

	if (err) {
		cout << "�����߻�" << endl;
		return handleError(__FUNCTION__, err);
	}

	// ���۸� �д´�. -> Recursive
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


