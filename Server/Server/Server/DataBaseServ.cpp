#include"DataBaseServ.h"

using namespace std;


DataBaseServ *DataBaseServ::instance; // ? 
std::mutex _mutex;
DataBaseServ* DataBaseServ::getInstance() {
	if (instance == nullptr) {
		std::lock_guard<std::mutex> lock(_mutex);
		if (instance == nullptr) {
			//cout << "db singleton 초기화" << endl;
			instance = new DataBaseServ();
		}
	}

	// _mutex는 scope밖으로 나가면 자동으로 release된다.
	return instance;
}


DataBaseServ::DataBaseServ(){
	file_table_name = "filelists";
	
	itemCnt = 0;
	mysql_init(&Conn);
	ConnPtr = mysql_real_connect(&Conn, "127.0.0.1", "root", "whdgus22", "mydb2", 3306, (char*)NULL, 0);

	if (ConnPtr == NULL) {
		fprintf(stderr, "MySQL connection error : %s", mysql_error(&Conn));
		exit(1);
	}
	printf("%s\n", "MySQL Connected");

}

string DataBaseServ::QeuryUserInfo(string id){
	string Query = "SELECT * FROM user where id = \'" + id+"\'";
	Stat = mysql_query(ConnPtr, Query.c_str());

	if (Stat != 0) {
		fprintf(stderr, "MySQL connection error : %s", mysql_error(&Conn));
		exit(1);
	}
	result = mysql_store_result(ConnPtr);
	
	string pw;
	while ((row = mysql_fetch_row(result)) != NULL) {
		printf("%s %s %s\n", row[0], row[1], row[2]);
		pw = row[2];
	}

	return pw;
}

void DataBaseServ::INSERT_FILE_INFO(string uid, string file_name, string last_update_time){
	string Query = "INSERT INTO "+file_table_name+"(name, lastupdatetime, uid) VALUES (\'" + file_name +"\', \'"+last_update_time +"\', "+uid + ")";
	Stat = mysql_query(ConnPtr, Query.c_str());

	if (Stat != 0) {
		fprintf(stderr, "MySQL connection error : %s", mysql_error(&Conn));
		exit(1);
	}

	result = mysql_store_result(ConnPtr);
}

void DataBaseServ::UPDATE_FILE_INFO(string uid, string file_name, string last_update_time){
	string Query = "UPDATE "+file_table_name+" SET lastupdatetime = \'" +last_update_time +"\' WHERE uid = \'"+uid+"\' and name = \'"+file_name+"\'" ;
	Stat = mysql_query(ConnPtr, Query.c_str());

	if (Stat != 0) {
		fprintf(stderr, "MySQL connection error : %s", mysql_error(&Conn));
		exit(1);
	}

	result = mysql_store_result(ConnPtr);
}

void DataBaseServ::INSERT(string fileName, string FileSize, string FilePath, string Time){
	outFile.open("db.txt", fstream::out | fstream::app); // 이어 쓰기
	outFile << fileName << endl; 
	outFile << FileSize << endl;
	outFile << FilePath << endl;
	outFile << Time << endl;
	outFile << "==" << endl;
	outFile.close();
}

void DataBaseServ::DELETES(){


}

int DataBaseServ::HowManyItem(string uid) {
	string Query = "SELECT count(*) from "+file_table_name+" where uid = " + uid;
	Stat = mysql_query(ConnPtr, Query.c_str());

	if (Stat != 0) {
		fprintf(stderr, "MySQL connection error : %s", mysql_error(&Conn));
		exit(1);
	}

	result = mysql_store_result(ConnPtr);
	int cnt;

	while ((row = mysql_fetch_row(result)) != NULL) {
		cnt = atoi(row[0]);
	}


	
	return cnt;

}

string DataBaseServ::getUserUid(string userID){
	string Query = "SELECT * FROM user where id = \'" + userID + "\'";
	Stat = mysql_query(ConnPtr, Query.c_str());

	if (Stat != 0) {
		fprintf(stderr, "MySQL connection error : %s", mysql_error(&Conn));
		exit(1);
	}

	result = mysql_store_result(ConnPtr);
	string uid;
	while ((row = mysql_fetch_row(result)) != NULL) {
		uid = row[0];
	}
	return uid;
}

void DataBaseServ::getAllItemInfo(ostream &requestStream, string uid ) {
	string Query = "SELECT * from "+file_table_name+" where uid = " + uid;
	Stat = mysql_query(ConnPtr, Query.c_str());

	if (Stat != 0) {
		fprintf(stderr, "MySQL connection error : %s", mysql_error(&Conn));
		exit(1);
	}

	result = mysql_store_result(ConnPtr);

	while ((row = mysql_fetch_row(result)) != NULL) {
		requestStream << row[1] << "\n";
		requestStream << row[2] << "\n";
	}

}


void DataBaseServ::UPDATE(string fileName, string FileSize, string FilePath, string Time){
	// db.txt에서 filename을 찾아서 time만 수정한다.

	string x;
	ifstream file("db.txt");
	ofstream ofile("db2.txt");
	int cnt = 0;
	while (!file.eof()){
		getline(file, x);
		if (x == fileName){
			ofile << x << endl;

			getline(file, x);
			ofile << x << endl;

			getline(file, x);
			ofile << x << endl;
			
			getline(file, x);
			ofile << Time << endl;
		}
		else
			ofile << x << endl;
	}

	file.close();
	ofile.close();
	remove("db.txt");
	rename("db2.txt", "db.txt");

	
}

void DataBaseServ::SHOWALL() {
	cout << "SHOW ALL ======" << endl;
}
