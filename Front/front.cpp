/*
Purpose: simple command line interface for connecting to
          the news database.
*/
#include <iostream>
#include <time.h>
#include <pthread.h>
#include <mysql++.h>
// To compile you must link the libraries, included in makefile
//  g++ front.cpp -o test -I/usr/include/mysql++ -I/usr/include/mysql -lmysqlpp
using namespace std;

void interface();
int fetch_news(mysqlpp::Connection,clock_t);
void print_news();

int main(){
  interface();
}

void interface(){

	mysqlpp::Connection conn(false);
	clock_t timer;

  int command=0;
  // run until stopped
  while(true){
    cout << "Command: ";
    cin >> command;
    // switch on command
    switch(command){
      case 0: cout << "hello\n"; break;
			case 1: timer=clock(); fetch_news(conn,timer); break;
			case 2: cout << "goodbye\n"; break;
			default: command = 1;
    }
    //exit case
    if(command == 1){
      cout << "end.\n";
      break;
    }
  }
}

int fetch_news(mysqlpp::Connection conn, clock_t timer){
  cout << "Fetching News\n";

	//database information
	const char* user = "cs441backendAccess";
	const char* pass = "t4back441";
	const char* server = "54.241.100.64";
	const char* database = "aggregator";


	//connect to database
	cout << "Initialize connection: ";
	  cout << "Fetching News\n";
	// timer = clock();

	// timer = clock() - timer;
	// cout << ((float)timer/CLOCKS_PER_SEC) << endl;

	cout << "Establish connection: ";
		  cout << "Fetching News\n";

	if(conn.connect(database,server,user,pass)){
			  cout << "Fetching News\n";


		//retrieve database table column
		mysqlpp::Query query = conn.query("SELECT * FROM Business;");
		cout << "Create query" << endl;
		if(mysqlpp::StoreQueryResult res = query.store()){
			cout << "Store query" << endl;
			cout << "\nNews:\n";
			mysqlpp::StoreQueryResult::const_iterator it;
			for(it=res.begin(); it!=res.end(); ++it){
				mysqlpp::Row row = *it;
				for(int i=0;i<6;i++){
					cout << '\t' << row[i];
				}
				cout << endl;
			}
			cout << endl;
			timer = clock() - timer;
			cout << ((float)timer/CLOCKS_PER_SEC) << endl;
		}
		else{
			cerr << "Failed to retrieve news:" << query.error() << endl;
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
