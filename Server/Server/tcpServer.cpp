#include"tcpServer.h"


tcpServer::tcpServer(unsigned short port)
:acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port), true){
	cout << "��� �� �Դϴ�." << endl;

}