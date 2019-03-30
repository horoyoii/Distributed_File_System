#include"tcpServer.h"

using namespace std;

tcpServer::tcpServer(unsigned short port)
	:acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port), true) {
	cout << "��� �� �Դϴ�." << endl;

	// ������ ��Ʈ�� ���ο� ������ accept ó���Ѵ�.
	ptrTcpConnection newConnection(new tcpConnection(io_context));
	acceptor.async_accept(newConnection->socket(),
		boost::bind(&tcpServer::handleAccept, this, 
			newConnection, 
			boost::asio::placeholders::error));

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
	cout << "���� �α׾ƿ�" << endl;
	io_context.stop();
}