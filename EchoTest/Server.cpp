#include<iostream>
#include<boost/asio.hpp>
#include<boost/bind.hpp>


using namespace std;

/*
�񵿱��� ���ΰ��� io_context�̴�.
��û�� �۾�(���ӹޱ�, ������ �ޱ�, ������ ������ ��)�� �Ϸ�Ǹ�
������ �Ϸ� �Լ�(�ݹ��Լ�)�� �ٷ� io_context�� ȣ���Ѵ�.

io_context.run()�Լ� ȣ�� �� ���� �Լ����� ���� ��� ���°� �ȴ�.

run()�Լ��� �񵿱� ��û�� ���� ��� ��û�� ���� ������ ������ ����ϴ�, ���� ��û�� �ִ��� ����
������ run()�Լ��� �������� ���� �۾��� �����Ѵ�.

void run(){
	while(true){
		if(CompleteQueue.empty())
			break;
		if(CompleteQueue.IsComplete) // ��û�� �۾� �Ϸ� ��
			��ϵ� �Լ� ȣ��
		....
	}
}
�׷��� �񵿱� ��û�� �ϱ� ���� run�� ȣ���ϸ� �� �ٷ� ����ȴ�.

*/

class Session {
	// ������ ������ Ŭ���̾�Ʈ
private:
	array<char, 128> m_receiveBuffer;
	string m_WriteMessage;
	boost::asio::ip::tcp::socket m_socket;

public:
	Session(boost::asio::io_context& io_context)
		:m_socket(io_context) {

	}

	boost::asio::ip::tcp::socket& Socket() {
		return m_socket;
	}

	void PostReceive() {
		cout << "called PostRecve" << endl;
		memset(&m_receiveBuffer, '\0', sizeof(m_receiveBuffer));
		m_socket.async_read_some( // ������ Ŭ���̾�Ʈ�� ������ �����͸� �񵿱�� �޴´�.
			boost::asio::buffer(m_receiveBuffer), // �����͸� �޴� ����
			boost::bind(&Session::handle_receive, this, // read�Ϸ� �� ȣ���� �Լ� 
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)
		);

	}
private:
	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred) {
		if (error) {
			if (error == boost::asio::error::eof) {
				cout << "Ŭ���̾�Ʈ�� ���� ����" << endl;
			}
			else {
				cout << "���� �߻�" << error.message() << endl;
			}
		}
		else {
			string rcvMsg = m_receiveBuffer.data();
			cout << "���� �޼��� from Client : " << rcvMsg << endl;
			cout << "���� ũ�� : " << bytes_transferred << "bytes" << endl;

			char sndMsg[128] = { 0, };
			sprintf_s(sndMsg, 128 - 1, "Re : %s", rcvMsg.c_str());
			m_WriteMessage = sndMsg;
			boost::asio::async_write(m_socket,  // 1) ����
				boost::asio::buffer(m_WriteMessage), // 2) ���� ����
				boost::bind(&Session::handle_write, this, // 3) callback �Լ�
					boost::asio::placeholders::error,			// 4) ���� �߻� ����
					boost::asio::placeholders::bytes_transferred) // 5) ���� ������ ũ��
			);

			PostReceive(); // �ٽ� ȣ���Ͽ� �ٽ� �޴´�.
		}
	}

	void handle_write(const boost::system::error_code&, size_t) {
		cout << "call handle_write - �ٽ��." << endl;
	}


};

const unsigned short PORT_NUMBER = 31400;

class TCP_Server {
private:
	int m_nSeqNumber;
	boost::asio::ip::tcp::acceptor m_acceptor;
	Session *m_pSession;

public:
	TCP_Server(boost::asio::io_context &io_context)
		:m_acceptor(io_context,
			boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
			PORT_NUMBER))
	{
		m_pSession = nullptr;
		StartAccept();

	}
	~TCP_Server() {
		if (m_pSession != nullptr)
			delete m_pSession;
	}

private:
	void StartAccept() {
		cout << "Ŭ���̾�Ʈ ���� ���" << endl;
		m_pSession = new Session(m_acceptor.get_io_context());
		m_acceptor.async_accept(m_pSession->Socket(), // ������ Ŭ���̾�Ʈ�� �Ҵ��� ���� Ŭ����
			boost::bind(&TCP_Server::handle_accept, // bind�� Ȱ���Ͽ� �񵿱� accept�� �Ϸ�Ǹ� ȣ���� Ŭ������ �����Ͽ���.(callback) /
				this,
				m_pSession,
				boost::asio::placeholders::error));

	}

	// handle_accpet�� ȣ��Ǹ� ���� ó�� �۾��� �Ϸ�Ǿ��ٴ� ���̴�.
	void handle_accept(Session* pSession, const boost::system::error_code& error) {
		if (!error) {
			cout << "Ŭ���̾�Ʈ ���� ����" << endl;

			pSession->PostReceive();
		}
	}


};


int main(void) {
	
	boost::asio::io_context io_context;
	TCP_Server server(io_context);
	io_context.run();

	cout << "��Ʈ��ũ ���� ����" << endl;




	return 0;
}