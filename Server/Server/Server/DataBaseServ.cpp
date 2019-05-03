#include"DataBaseServ.h"

using namespace std;




DataBaseServ::DataBaseServ(){
	itemList;
	itemCnt = 0;
	mysql_init(&Conn);
	ConnPtr = mysql_real_connect(&Conn, "127.0.0.1", "root", "whdgus22", "MyDB", 3306, (char*)NULL, 0);

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

int DataBaseServ::HowManyItem() {
	inFile.open("db.txt", ios::in | ios::binary);
	string buffer;
	int cnt = 0;
	while (inFile.peek() != EOF) {
		getline(inFile, buffer);
		cnt++;
	}

	inFile.close();
	return cnt / 5;
}

void DataBaseServ::getAllItemInfo(ostream &requestStream) {
	inFile.open("db.txt", ios::in | ios::binary);
	string buffer;

	while (inFile.peek() != EOF) {
		getline(inFile, buffer);
		if(buffer != "==")
			requestStream << buffer << "\n";
	}


	inFile.close();
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
	vector<ITEM>::iterator it;
	int cnt = 1;
	for (it = itemList.begin(); it != itemList.end(); ++it) {		
		cout << cnt++ << " : " << (*it).fileName << " - " << (*it).FileLatestUpdateTime << endl;

	}
	cout << "===================" << endl;
}
