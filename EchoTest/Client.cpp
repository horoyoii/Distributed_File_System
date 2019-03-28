#include<boost/bind.hpp>
#include<boost/asio.hpp>
#include<iostream>


using namespace std;


const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 31400;


class TCP_Client {
private:
	boost::asio::io_context& m_io_context;
	boost::asio::ip::tcp::socket m_Socket;
	int m_nSeqNumber;
	array<char, 128> m_ReceiveBuffer;
	boost::asio::streambuf buffers;
	string m_WriteMsg;

public:
	TCP_Client(boost::asio::io_context& io_context)
		:m_io_context(io_context),
		m_Socket(io_context),
		m_nSeqNumber(0){}

	void Connect(boost::asio::ip::tcp::endpoint& endpoint) {
		m_Socket.async_connect(endpoint,
			boost::bind(&TCP_Client::handle_connect,
				this,
				boost::asio::placeholders::error));
	}

private:
	void handle_connect(const boost::system::error_code& error) {
		if (error) {
			cout << "error : " << error.message() << endl;
		}
		else {
			cout << "Connected " << endl;
			PostWrite();
		}
	}

	void PostWrite() {
		if (m_Socket.is_open() == false) {
			return;
		}

		char szMessage[128] = { 0, };
		cin >> szMessage;
		m_WriteMsg = szMessage;

		boost::asio::async_write(m_Socket, boost::asio::buffer(m_WriteMsg),
			boost::bind(&TCP_Client::handle_write, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		
		PostReceive();


	}

	void PostReceive() {
		cout << "calle PostRec" << endl;
		
		memset(&m_ReceiveBuffer, '\0', sizeof(m_ReceiveBuffer));
		m_Socket.async_read_some(boost::asio::buffer(m_ReceiveBuffer),
			boost::bind(&TCP_Client::handle_receive, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
				
		/*
		boost::asio::async_read_until(m_Socket, buffers, "\n\n",
			boost::bind(&TCP_Client::handle_receive,
				this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
				*/


	}

	void handle_receive(const boost::system::error_code& error, size_t  bytes_transferred) {
		cout << "called handle_rec" << endl;
		if (error) {

		}
		else {
			string rcvMsg = m_ReceiveBuffer.data();
			cout << "받은 데이터 : " << rcvMsg << endl;
			/*
			std::istream respos(&buffers);
			std::istream respos(&buffers);
			string strr;
			respos >> strr;
			cout << "받은 데이터 : " << strr << endl;
			*/
			PostWrite();
		}
	}
	void handle_write(const boost::system::error_code, size_t) {

	}
};


int main(void) {
	boost::asio::io_context io_context;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(SERVER_IP), PORT_NUMBER);

	TCP_Client client(io_context);
	client.Connect(endpoint);
	io_context.run();

	cout << "네트워크 접속 종료" << endl;


	return 0;
}