#pragma once
#include"headers.h"

using namespace boost::asio::ip;

class FileTcpClient {
private:	
	boost::asio::ip::tcp::resolver resolver; // provides the ability to resolve a query to a list of endpoints.
	boost::asio::ip::tcp::socket socket;
	std::ifstream sourceFile; // input file stream in c++
	boost::asio::streambuf request;
	boost::array<char, 8192> buf; // 한번에 보내는 용량 8192 byte

public:
	FileTcpClient(boost::asio::io_context& io_context, const std::string& server, const std::string& FILE_PATH);
	void handleResolve(const boost::system::error_code& err, tcp::resolver::iterator  myIterator);
	void handleConnect(const boost::system::error_code& err, tcp::resolver::iterator myIterator);
	void handleWrite(const boost::system::error_code &err);
};