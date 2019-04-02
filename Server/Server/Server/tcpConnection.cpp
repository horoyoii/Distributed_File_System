#include"tcpConnection.h"


using namespace std;

tcpConnection::tcpConnection(boost::asio::io_context& io_context)
	:mySocket(io_context) {
	cout << "tcpConnection ������ ȣ��" << endl;
}


//Ŭ���̾�Ʈ�� ��û�� ó���ϱ� �����Ѵ�.
void tcpConnection::start() {
	boost::asio::async_read_until(mySocket, requestBuf, "\n\n",
		boost::bind(&tcpConnection::handleReadRequest,
			shared_from_this(), boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));


}

void tcpConnection::handleResponse(const boost::system::error_code &err) {
	cout << "������ ���½��ϴ�." << endl;
}

void tcpConnection::handleReadRequest(const boost::system::error_code& err, std::size_t bytesTransferred) {
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
		string userPW;
		// '\n' �� �������ͷ� �Ͽ� Ŭ���̾�Ʈ���� ������.
		requestStream >> userName;
		requestStream >> userPW;
		requestStream.read(buf.c_array(), 2);
		cout << " == ����� ���� ==" << endl;
		cout << "�̸� : " << userName << "  pw : " << userPW << endl;



		// RESPONSE
		std::ostream requestStream(&AccessResultRespon);

		// ��й�ȣ ����
		if (userPW == "12345") {
			requestStream << "true" << "\n";
			boost::filesystem::directory_iterator end;
			int cnt = 0;
			for (boost::filesystem::directory_iterator iterator("user/eirc8260"); iterator != end; iterator++) {
				cnt++;
			}
			requestStream << cnt << "\n";

			for (boost::filesystem::directory_iterator iterator("user/eirc8260"); iterator != end; iterator++) {
				requestStream << iterator->path().leaf() << "\n";
			}
			requestStream << "\n\n";
		
		}
		else
			requestStream << "false" << "\n\n";

		boost::asio::async_write(mySocket, AccessResultRespon,
			boost::bind(&tcpConnection::handleWrite, shared_from_this(),
				boost::asio::placeholders::error));

	}

	if (instruction == "fileSend") {
		std::istream requestStream(&requestBuf);
		string filePath, LatestUpdateTime;
		string temp;

		requestStream >> filePath;
		getline(requestStream, temp, '\n');
		requestStream >> fileSize;
		getline(requestStream, temp, '\n');
		//requestStream >> LatestUpdateTime;
		getline(requestStream, LatestUpdateTime, '\n');
		requestStream.read(buf.c_array(), 1); // header�� ���� '\n\n' ����. �׷��� '\n'�� �ѹ� �� ���ش�.

		// ���� ���� ���
		cout << " == ���� ���� ==" << endl;
		cout << filePath << " ũ�� : " << fileSize << "bytes" << endl;
		cout << "update time : " << LatestUpdateTime << endl;

		//TODO : ���ϸ� - ���� �ֱ� ������Ʈ �ð� �� �����ͺ��̽��� �����Ѵ�.
	


		// ���ϸ� ����
		size_t pos = filePath.find_last_of('\\');
		if (pos != string::npos) {
			filePath = filePath.substr(pos + 1);
			cout << "���ϸ� ���� : " << filePath << endl;
		}
		
		// Ư�� ���丮�� �����ϱ�
		string dir_path = "user/eirc8260/" + filePath;
		outputFile.open(dir_path.c_str(), std::ios_base::binary);
		//outputFile.open(filePath.c_str(), std::ios_base::binary);

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

	if (instruction == "getDataList") {
		// DB���� ������ ������ �����Ѵٸ�


	}

}


void tcpConnection::handleWrite(const boost::system::error_code&) {
	cout << "������ �Ϸ�" << endl;
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


