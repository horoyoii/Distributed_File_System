#include"userTcpClient.h"

using namespace std;

void userTcpClient::handleResolve(const boost::system::error_code& err, tcp::resolver::iterator myIterator) {
	cout << "D : handleResolve ����" << endl;
	if (!err) {
		cout << "D : not ���� in" << __FUNCTION__ << endl;
		// �Ѱܹ��� endpoint�� ����õ�
		tcp::endpoint endpoint = *myIterator;
		socket.async_connect(endpoint,
			boost::bind(&userTcpClient::handleConnect, this,
				boost::asio::placeholders::error, ++myIterator));
	}
	else {
		cout << "== ���� ���� ==" << endl;
		cout << "Error : " << err.message() << endl;
	}
}


void userTcpClient::handleConnect(const boost::system::error_code& err, tcp::resolver::iterator myIterator) {
	// ���� ���� �� ��û�� ������.
	cout << "D : handleConnect ���� " << endl;
	if (!err) {
		cout << "== ���� ���� == " << endl;
		boost::asio::async_write(socket, request,
			boost::bind(&userTcpClient::handleWriteUser, this,
				boost::asio::placeholders::error));
	}
	// ���� ���� �� ���� endpoint�� ����õ�
	else if (myIterator != tcp::resolver::iterator()) {
		socket.close();
		tcp::endpoint endpoint = *myIterator;
		socket.async_connect(endpoint,
			boost::bind(&userTcpClient::handleConnect, this,
				boost::asio::placeholders::error, ++myIterator));
	}
	else {
		cout << "== ���� ���� ==" << endl;
		cout << "Error : " << err.message() << endl;
	}
}


void userTcpClient::handleWriteUser(const boost::system::error_code& err) {
	if (!err) {

	}
	else {
		cout << "== ���� ���� ==" << endl;
		cout << "Error : " << err.message() << " in " << __FUNCTION__ << endl;
	}
}







userTcpClient::userTcpClient(boost::asio::io_context& io_context,
	const std::string& server, std::string userName, std::string userInfo)
:resolver(io_context), socket(io_context){

	size_t pos = server.find(':');
	if (pos == std::string::npos)
		return;
	string portString = server.substr(pos + 1);
	string serverIP = server.substr(0, pos);
	cout << serverIP << ":" << portString << endl;
	// ������ ����� �̸��� ���� ����
	// ** ���������ϴ� �����͸� io stream���� ���� (ostream���� ������ istream���� �޴´�.)
	cout << "D :  userTcpClient Called " << endl;
	ostream requestStream(&request);
	requestStream << "enter" << "\n" << userName << "\n" << userInfo << "\n\n";

	cout << "����� �̸� : " << userName << ", ����� ���� : " << userInfo << endl;



	// �񵿱��� resolving
	tcp::resolver::query query(serverIP, portString);
	resolver.async_resolve(query, boost::bind(&userTcpClient::handleResolve, this,
		boost::asio::placeholders::error, boost::asio::placeholders::iterator));


}