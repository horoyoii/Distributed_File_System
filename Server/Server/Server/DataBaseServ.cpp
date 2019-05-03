#include"DataBaseServ.h"

using namespace std;




DataBaseServ::DataBaseServ(){
	itemList;
	itemCnt = 0;
	cout << "DB con called" << endl;
	//inFile.open("db.txt", ios::in | ios::binary);

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
