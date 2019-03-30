#pragma once
#include"pch.h"

class tcpConnection : public boost::enable_shared_from_this<tcpConnection> {
private:
	boost::asio::streambuf requestBuf;
	boost::asio::ip::tcp::socket mySocket;
	boost::array<char, 40960> buf;
	std::ofstream outputFile;
	size_t fileSize;
	boost::asio::streambuf AccessResultRespon;


public:

	tcpConnection(boost::asio::io_context& io_context);
	void start(void);
	boost::asio::ip::tcp::socket& socket();
	void handleError(const std::string& functionName, const boost::system::error_code& err);
	void handleReadRequest(const boost::system::error_code& err, std::size_t bytesTransferred);
	void handleReadFileContent(const boost::system::error_code& err, std::size_t bytesTransferred);
	void handleResponse(const boost::system::error_code &err);
	void handleWrite(const boost::system::error_code& err);
};