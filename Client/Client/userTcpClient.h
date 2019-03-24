#pragma once
#include"headers.h"
using namespace boost::asio::ip;

class userTcpClient {
private:
	tcp::resolver resolver;
	tcp::socket socket;
	boost::array<char, 8192> buf;
	boost::asio::streambuf request;
public:
	void handleResolve(const boost::system::error_code& err, tcp::resolver::iterator myIterator);
	void handleConnect(const boost::system::error_code& err, tcp::resolver::iterator myIterator);
	void handleWriteUser(const boost::system::error_code& err);
	userTcpClient(boost::asio::io_context& io_context,
		const std::string& server, std::string userName, std::string userInfo);

};
