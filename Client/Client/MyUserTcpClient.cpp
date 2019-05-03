#include "MyUserTcpClient.h"



MyUserTcpClient::MyUserTcpClient(boost::asio::io_context& io_context, const std::string& server, const std::string& userName, const std::string& userPW, DataFromServer *dataFromServer)
	:resolver(io_context), socket(io_context) {

	this->dataFromServer = dataFromServer;

	size_t pos = server.find(':');
	if (pos == string::npos) {
		return;
	}
	string Port = server.substr(pos + 1);
	string serverIP = server.substr(0, pos);


	// ���� ������ ������ ��ο� ������ ũ�⸦ ����
	cout << "���� ���� ����" << endl;
	std::ostream requestStream(&request);
	requestStream << "enter" << "\n" << userName << "\n" << userPW << "\n\n";

	cout << "��Ÿ������ ��û ũ�� : " << request.size() << endl;

	// procedure : resolving -> binding -> connecting -> file sending
	boost::asio::ip::tcp::resolver::query query(serverIP, Port);
	resolver.async_resolve(query, boost::bind(&MyUserTcpClient::handleResolve, this,
		boost::asio::placeholders::error, boost::asio::placeholders::iterator));






}

void  MyUserTcpClient::handleResolve(const boost::system::error_code& err, tcp::resolver::iterator  myIterator) {
	cout << "D : handleResolve called in MyUser" << endl;
	// ���� �߻����� ���� ���
	if (!err) {
		tcp::endpoint endpoint = *myIterator; // endpoint���� "127.0.0.1:1000" �� ����ȴ�.
		socket.async_connect(endpoint,
			boost::bind(&MyUserTcpClient::handleConnect, this, boost::asio::placeholders::error, ++myIterator));

	}
	else {
		cout << " ���� ����  " << endl;
		cout << err.message() << endl;
	}
}


void MyUserTcpClient::handleConnect(const boost::system::error_code& err, tcp::resolver::iterator myIterator) {
	cout << "D : handleConnect called in MyUser" << endl;
	if (!err) {

		// 1) ���� ���� �����ϰ�
		cout << "���� ����" << endl;

		    // async_write() : ������ ���빰�� ���� �����ϸ� callback�Լ��� ȣ���Ѵ�.(���⼭�� handlewrite)
		async_write(socket, request, 
			boost::bind(&MyUserTcpClient::handleWrite, this,
				boost::asio::placeholders::error));

		

	}
	else {
		cout << "���ۿ���" << endl;
		cout << err.message() << endl;
	}
}


void MyUserTcpClient::handleWrite(const boost::system::error_code &err) {
	if (!err) {
		boost::asio::async_read_until(socket, ResponseBuf, "\n\n",
			boost::bind(&MyUserTcpClient::ResponseListFromServer,
				this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));



	}
	else {
		cout << "���� ����" << endl;
		cout << "error : " << err.message() << endl;
	}
}

void MyUserTcpClient::ResponseListFromServer(const boost::system::error_code& err, std::size_t bytesTransferred) {

	std::istream requestStream(&ResponseBuf);

	// ��û ���� ó�� : ��� ó�� !!!!!!!!!!!!!!!!!
	string AccessResult;
	requestStream >> AccessResult;
	string responseTOKEN;
	int cnt;
	cout << "[��������]������ �޾ҽ��ϴ�. from server  : " << AccessResult << endl;
	
	if (AccessResult == "true") {
		accResult = true;
		requestStream >> responseTOKEN;
		//getline(requestStream, responseTOKEN, '\n');
		cnt = atoi(responseTOKEN.c_str()); // �Ѹ���� �켱 �˾Ƴ���.
		getline(requestStream, responseTOKEN, '\n'); // '\n' �ϳ� ���ش�.
		// �޾ƿ� ������ ���� �����Ѵ�.
		string name, UpdateDate;
		//cout << "========== ���� ���丮 ���� cnt : "<<cnt<<"======================" << endl;
		for(int i=1;i<=cnt;i++) {
				getline(requestStream, responseTOKEN, '\n');
				//cout << " name : " << responseTOKEN << endl;
				name = responseTOKEN;
				getline(requestStream, responseTOKEN, '\n');
				//cout << " size : " << responseTOKEN << endl;
				getline(requestStream, responseTOKEN, '\n');
				//cout << " path : " << responseTOKEN << endl;
				getline(requestStream, responseTOKEN, '\n');
				//cout << " date : " << responseTOKEN << endl;
				UpdateDate = responseTOKEN;
				getline(requestStream, responseTOKEN, '\n');
				
				dataFromServer->setDateInfo(name, UpdateDate);
		}
	
	}
	else
		accResult = false;
}

bool MyUserTcpClient::getAccResult()
{
	cout << "================�����κ��� ���� ������ ���================" << endl;
	dataFromServer->showAllData();
	return accResult;
}
