/*
Purpose: simple command line interface for connecting to
          the news database.
*/
#include <iostream>
#include <mysql++.h>
// To compile you must link the libraries, included in makefile
//  g++ front.cpp -o test -I/usr/include/mysql++ -I/usr/include/mysql -lmysqlpp
using namespace std;

void interface();
void fetch_news();
void print_news();

int main(){
  interface();
}

void interface(){
  int command=0;
  // run until stopped
  while(true){
    cout << "Command: ";
    cin >> command;
    // switch on command
    switch(command){
      case 0: cout << "hello\n"; break;
      case 1: cout << "goodbye\n"; break;
			case 2: cout << "fetching\n"; fetch_news(); break;
    }
    //exit case
    if(command == 1){
      cout << "end.\n";
      break;
    }
  }
}

void fetch_news(){
  cout << "fetching\n";

	//database information
	user = 'username';
	pass = 'password';
	server = 'servername';
	database = 'db_name';

	//connect to database
	mysqlpp::Connection conn(false);
	if(conn.connect(database,server,user,pass)){
		//retrieve database table column
		mysqlpp::Query query = conn.query('select item');
		if(mysqlpp::SotreQueryResult res = query.store()){
			cout << "News:\n";
			mysqlpp::StoreQueryResult::const_iterator it;
			for(it=res.begin(); it!=res.end(); ++it){
				mysqlpp::Row row = *it;
				cout << '\t' << row[0] <<end;
			}
		}
		else{
			cerr << "Failed to retrieve news:" << query.e{rror() << endl;
			return 1;
		}
	}
	else{
		cerr << "DB connection failed: " << conn.error() << endl;
		return 1;
	}
}

void print_news(){
  cout << "printing\n";
}
