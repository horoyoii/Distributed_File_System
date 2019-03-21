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
	cout << "���� ���� ũ�� :  " << fileSize <<"bytes"<< endl;
	sourceFile.seekg(0); // Set position in input sequence


	// ���� ������ ������ ��ο� ������ ũ�⸦ ����
	cout << "���� ���� ����" << endl;
	std::ostream requestStream(&request);
	requestStream << "fileSend" << "\n" << FILE_PATH << "\n" << fileSize << "\n\n";

	cout << "��Ÿ������ ��û ũ�� : " << request.size() << endl;

	// procedure : resolving -> binding -> connecting -> file sending
	boost::asio::ip::tcp::resolver::query query(serverIP, Port);
	resolver.async_resolve(query, boost::bind(&FileTcpClient::handleResolve, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator));






}

void  FileTcpClient::handleResolve(const boost::system::error_code& err, tcp::resolver::iterator  myIterator) {
	
	// ���� �߻����� ���� ���
	if (!err) {
		tcp::endpoint endpoint = *myIterator; // endpoint���� "127.0.0.1:1000" �� ����ȴ�.
		socket.async_connect(endpoint,
			boost::bind(&FileTcpClient::handleConnect, this, boost::asio::placeholders::error, ++myIterator));

	}
	else {
		cout << " ���� ����  " << endl;
		cout << err.message() << endl;
	}
}


void FileTcpClient::handleConnect(const boost::system::error_code& err, tcp::resolver::iterator myIterator) {
	if (!err) {

		cout << "���� ����" << endl;
		async_write(socket, request,
			boost::bind(&FileTcpClient::handleWrite, this, boost::asio::placeholders::error));

	}
	else {
		cout << "���ۿ���" << endl;
		cout << err.message() << endl;
	}
}


void FileTcpClient::handleWrite(const boost::system::error_code &err) {
	// ������ ������ �����Ѵ�.
	if (!err) {
		
		if (sourceFile.eof() == false) {
			// ���ۿ� ��´�.
			sourceFile.read(buf.c_array(), (std::streamsize)buf.size());
			// Excetion handling
			if (sourceFile.gcount() <= 0) {
				//Returns the number of characters extracted by the last unformatted input operation performed on the object
				cout << "���� ����" << endl;
				cout << "read file error" << endl;
				return;
			}

			cout << sourceFile.gcount() << "bytes ���� �Ϸ�" << endl;
			cout << "��ü : " << sourceFile.tellg() << "bytes" << endl;

			// ���� ���� ���� ����
			// ����� �ݺ�
			async_write(socket,
				boost::asio::buffer(buf.c_array(), sourceFile.gcount()),
				bind(&FileTcpClient::handleWrite, this, boost::asio::placeholders::error)
			);

			if (err) {
				cout << " ����  ����" << endl;
				cout << "send error : " << err << endl;
				return;
			}
		}
		else
			return;
	}
	else {
		cout << "���� ����" << endl;
		cout << "error : " << err.message() << endl;
	}
}