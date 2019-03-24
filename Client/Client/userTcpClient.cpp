#include"userTcpClient.h"

using namespace std;

void userTcpClient::handleResolve(const boost::system::error_code& err, tcp::resolver::iterator myIterator) {
	cout << "D : handleResolve 진입" << endl;
	if (!err) {
		cout << "D : not 오류 in" << __FUNCTION__ << endl;
		// 넘겨받은 endpoint로 연결시도
		tcp::endpoint endpoint = *myIterator;
		socket.async_connect(endpoint,
			boost::bind(&userTcpClient::handleConnect, this,
				boost::asio::placeholders::error, ++myIterator));
	}
	else {
		cout << "== 연결 오류 ==" << endl;
		cout << "Error : " << err.message() << endl;
	}
}


void userTcpClient::handleConnect(const boost::system::error_code& err, tcp::resolver::iterator myIterator) {
	// 연결 성공 시 요청을 보낸다.
	cout << "D : handleConnect 진입 " << endl;
	if (!err) {
		cout << "== 정보 전송 == " << endl;
		boost::asio::async_write(socket, request,
			boost::bind(&userTcpClient::handleWriteUser, this,
				boost::asio::placeholders::error));
	}
	// 연결 실패 시 다음 endpoint로 연결시도
	else if (myIterator != tcp::resolver::iterator()) {
		socket.close();
		tcp::endpoint endpoint = *myIterator;
		socket.async_connect(endpoint,
			boost::bind(&userTcpClient::handleConnect, this,
				boost::asio::placeholders::error, ++myIterator));
	}
	else {
		cout << "== 전송 오류 ==" << endl;
		cout << "Error : " << err.message() << endl;
	}
}


void userTcpClient::handleWriteUser(const boost::system::error_code& err) {
	if (!err) {

	}
	else {
		cout << "== 전송 오류 ==" << endl;
		cout << "Error : " << err.message() << " in " << __FUNCTION__ << endl;
	}
}







userTcpClient::userTcpClient(boost::asio::io_context& io_context,
	const std::string& server, std::string userName, std::string userInfo)
:resolver(io_context), socket(io_context){

	size_t pos = server.find(':');
	if (pos == std::string::npos)
		return;
	string portString = server.substr(pos + 1);
	string serverIP = server.substr(0, pos);
	cout << serverIP << ":" << portString << endl;
	// 서버에 사용자 이름과 정보 전송
	// ** 보내고자하는 데이터를 io stream으로 만들어서 (ostream으로 보내서 istream으로 받는다.)
	cout << "D :  userTcpClient Called " << endl;
	ostream requestStream(&request);
	requestStream << "enter" << "\n" << userName << "\n" << userInfo << "\n\n";

	cout << "사용자 이름 : " << userName << ", 사용자 정보 : " << userInfo << endl;



	// 비동기적 resolving
	tcp::resolver::query query(serverIP, portString);
	resolver.async_resolve(query, boost::bind(&userTcpClient::handleResolve, this,
		boost::asio::placeholders::error, boost::asio::placeholders::iterator));


}