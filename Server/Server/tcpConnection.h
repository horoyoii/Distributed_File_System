#pragma once
#include"header.h"

unsigned short port = 1000;
unsigned short userCount = 0;
std::array<std::string, 50> userList;


class tcpConnection : public boost::enable_shared_from_this<tcpConnection> {
private:
	boost::asio::streambuf requestBuf;
	boost::asio::ip::tcp::socket mySocket;
	boost::array<char, 40960> buf;
	std::ofstream outputFile;
	size_t fileSize;

public:

	tcpConnection(boost::asio::io_context& io_context);
	void start(void);
	boost::asio::ip::tcp::socket& socket();
	void handleError(const std::string& functionName, const boost::system::error_code& err);
	void handleReadRequest(const boost::system::error_code& err, std::size_t bytesTransferred);
	void handleReadFileContent(const boost::system::error_code& err, std::size_t bytesTransferred);
};



