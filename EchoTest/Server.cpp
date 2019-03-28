#include<iostream>
#include<boost/asio.hpp>
#include<boost/bind.hpp>


using namespace std;

/*
비동기의 주인공은 io_context이다.
요청한 작업(접속받기, 데이터 받기, 데이터 보내기 등)이 완료되면
지정한 완료 함수(콜백함수)를 바로 io_context가 호출한다.

io_context.run()함수 호출 시 메인 함수에서 무한 대기 상태가 된다.

run()함수는 비동기 요청이 있을 경우 요청이 끝날 때까지 무한히 대기하다, 다음 요청이 있는지 보고
없으면 run()함수를 빠져나와 다음 작업을 진행한다.

void run(){
	while(true){
		if(CompleteQueue.empty())
			break;
		if(CompleteQueue.IsComplete) // 요청된 작업 완료 시
			등록된 함수 호출
		....
	}
}
그래서 비동기 요청을 하기 전에 run을 호출하면 곧 바로 종료된다.

*/

class Session {
	// 서버에 접속한 클라이언트
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
		m_socket.async_read_some( // 접속한 클라이언트가 보내는 데이터를 비동기로 받는다.
			boost::asio::buffer(m_receiveBuffer), // 데이터를 받는 버퍼
			boost::bind(&Session::handle_receive, this, // read완료 시 호출할 함수 
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)
		);

	}
private:
	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred) {
		if (error) {
			if (error == boost::asio::error::eof) {
				cout << "클라이언트와 연결 종료" << endl;
			}
			else {
				cout << "에러 발생" << error.message() << endl;
			}
		}
		else {
			string rcvMsg = m_receiveBuffer.data();
			cout << "받은 메세지 from Client : " << rcvMsg << endl;
			cout << "받은 크기 : " << bytes_transferred << "bytes" << endl;

			char sndMsg[128] = { 0, };
			sprintf_s(sndMsg, 128 - 1, "Re : %s", rcvMsg.c_str());
			m_WriteMessage = sndMsg;
			boost::asio::async_write(m_socket,  // 1) 소켓
				boost::asio::buffer(m_WriteMessage), // 2) 보낼 버퍼
				boost::bind(&Session::handle_write, this, // 3) callback 함수
					boost::asio::placeholders::error,			// 4) 에러 발생 여부
					boost::asio::placeholders::bytes_transferred) // 5) 보낸 데이터 크기
			);

			PostReceive(); // 다시 호출하여 다시 받는다.
		}
	}

	void handle_write(const boost::system::error_code&, size_t) {
		cout << "call handle_write - 다썼다." << endl;
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
		cout << "클라이언트 접속 대기" << endl;
		m_pSession = new Session(m_acceptor.get_io_context());
		m_acceptor.async_accept(m_pSession->Socket(), // 접속한 클라이언트에 할당할 소켓 클래스
			boost::bind(&TCP_Server::handle_accept, // bind를 활용하여 비동기 accept이 완료되면 호출할 클래스를 래핑하였다.(callback) /
				this,
				m_pSession,
				boost::asio::placeholders::error));

	}

	// handle_accpet가 호출되면 접속 처리 작업이 완료되었다는 뜻이다.
	void handle_accept(Session* pSession, const boost::system::error_code& error) {
		if (!error) {
			cout << "클라이언트 접속 성공" << endl;

			pSession->PostReceive();
		}
	}


};


int main(void) {
	
	boost::asio::io_context io_context;
	TCP_Server server(io_context);
	io_context.run();

	cout << "네트워크 접속 종료" << endl;




	return 0;
}