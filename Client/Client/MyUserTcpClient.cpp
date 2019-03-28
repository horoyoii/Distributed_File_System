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
	


	// ���� ������ ������ ��ο� ������ ũ�⸦ ����
	cout << "���� ���� ����" << endl;
	std::ostream requestStream(&request);
	requestStream << "enter" << "\n" << FILE_PATH << "\n" << body << "\n\n";

	cout << "��Ÿ������ ��û ũ�� : " << request.size() << endl;

	// procedure : resolving -> binding -> connecting -> file sending
	boost::asio::ip::tcp::resolver::query query(serverIP, Port);
	resolver.async_resolve(query, boost::bind(&MyUserTcpClient::handleResolve, this,
		boost::asio::placeholders::error, boost::asio::placeholders::iterator));






}

void  MyUserTcpClient::handleResolve(const boost::system::error_code& err, tcp::resolver::iterator  myIterator) {
	cout << "D : handleResolve called in MyUser" << endl;
	// ���� �߻����� ���� ���
	if (!err) {
		tcp::endpoint endpoint = *myIterator; // endpoint���� "127.0.0.1:1000" �� ����ȴ�.
		socket.async_connect(endpoint,
			boost::bind(&MyUserTcpClient::handleConnect, this, boost::asio::placeholders::error, ++myIterator));

	}
	else {
		cout << " ���� ����  " << endl;
		cout << err.message() << endl;
	}
}


void MyUserTcpClient::handleConnect(const boost::system::error_code& err, tcp::resolver::iterator myIterator) {
	cout << "D : handleConnect called in MyUser" << endl;
	if (!err) {

		// 1) ���� ���� �����ϰ�
		cout << "���� ����" << endl;

		    // async_write() : ������ ���빰�� ���� �����ϸ� callback�Լ��� ȣ���Ѵ�.(���⼭�� handlewrite)
		async_write(socket, request, 
			boost::bind(&MyUserTcpClient::handleWrite, this,
				boost::asio::placeholders::error));

		// 2) ������� �ް�
		

	}
	else {
		cout << "���ۿ���" << endl;
		cout << err.message() << endl;
	}
}


void MyUserTcpClient::handleWrite(const boost::system::error_code &err) {
	// call back func�̴�. == ���� �����ϸ� �� �Լ��� ȣ��ȴ�. �׷��ٸ� ���⼭ read�� ���� �ɰŽô�.��/??
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
		cout << "���� ����" << endl;
		cout << "error : " << err.message() << endl;
	}
}

void MyUserTcpClient::TestCallback(const boost::system::error_code& err, std::size_t bytesTransferred) {
	cout << "������ �޾ҽ��ϴ�. from server." << endl;
}