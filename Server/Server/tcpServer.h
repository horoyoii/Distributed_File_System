#pragma once
#include"header.h"

class tcpServer {
private:
	boost::asio::io_context  io_context;
	boost::asio::ip::tcp::acceptor acceptor;

public:
	typedef boost::shared_ptr<tcpConnection> ptrTcpConnection;


	tcpServer(unsigned short port);



};