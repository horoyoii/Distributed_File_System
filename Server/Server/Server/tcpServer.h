#pragma once
#include"pch.h"
#include"tcpConnection.h"

class tcpServer : private boost::noncopyable {
private:
	boost::asio::io_context  io_context;
	boost::asio::ip::tcp::acceptor acceptor;

public:
	typedef boost::shared_ptr<tcpConnection> ptrTcpConnection;
	tcpServer(unsigned short port);
	void handleAccept(ptrTcpConnection currentConnection, const boost::system::error_code& e);
	~tcpServer();

};