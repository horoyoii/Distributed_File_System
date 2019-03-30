#include"tcpServer.h"

using namespace std;

tcpServer::tcpServer(unsigned short port)
	:acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port), true) {
	cout << "대기 중 입니다." << endl;

	// 지정된 포트로 새로운 연결을 accept 처리한다.
	ptrTcpConnection newConnection(new tcpConnection(io_context));
	acceptor.async_accept(newConnection->socket(),
		boost::bind(&tcpServer::handleAccept, this, 
			newConnection, 
			boost::asio::placeholders::error));

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
	cout << "유저 로그아웃" << endl;
	io_context.stop();
}