#pragma once
#include"headers.h"
#include"DataFromServer.h"

using namespace boost::asio::ip;

class DataFromServer;

class MyUserTcpClient {
private:
	boost::asio::ip::tcp::resolver resolver; // provides the ability to resolve a query to a list of endpoints.
	boost::asio::ip::tcp::socket socket;
	std::ifstream sourceFile; // input file stream in c++
	boost::asio::streambuf request;
	boost::asio::streambuf ResponseBuf;
	boost::array<char, 8192> buf; // 한번에 보내는 용량 8192 byte
	bool accResult;
	DataFromServer *dataFromServer;

public:
	MyUserTcpClient(boost::asio::io_context& io_context, const std::string& server, const std::string& userName, const std::string& userPw, DataFromServer *dataFromServer);
	void handleResolve(const boost::system::error_code& err, tcp::resolver::iterator  myIterator);
	void handleConnect(const boost::system::error_code& err, tcp::resolver::iterator myIterator);
	void handleWrite(const boost::system::error_code &err);
	void TestCallback(const boost::system::error_code& err, std::size_t bytesTransferred);
	bool getAccResult();
};