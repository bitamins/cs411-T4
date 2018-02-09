/*
Purpose: simple command line interface for connecting to
          the news database.
*/
//libraries
#include <iostream>
#include <time.h>
#include <pthread.h>
#include <mysql++.h>

//Global variables
const char* serverIP = "54.241.100.64";
const char* databaseName = "aggregator";

//prototypes
void interface(mysqlpp::Connection&);
int fetch_news(mysqlpp::Connection&);
mysqlpp::Connection db_connect(const char*,const char*,const char*,const char*);

//default namespace
using namespace std;

/*
	Function definitions
*/
int main(int argc, char* argv[]){
	//establish connection to database
	mysqlpp::Connection db_conn = db_connect(argv[1],argv[2],serverIP,databaseName);
	if(db_conn == NULL){
		cout << "Exiting...\n"; return 0;
	}
	//launch the user interface with the database connection
  interface(db_conn);
}

void interface(mysqlpp::Connection& conn){
  int command=0;
  // run until stopped
	cout << "Welcome to Real News, press (2) to exit the program.\n";
  while(true){
    cout << "Command: ";
    cin >> command;
    // switch on command
    switch(command){
      case 0: cout << "Hello\n"; break;
			case 1: cout << "Fetching News\n";fetch_news(conn); break;
			case 2: cout << "Goodbye\n"; break;
			default: command = 2;
    }
    //exit case
    if(command == 2){
      cout << "end.\n";
      break;
    }
  }
}

mysqlpp::Connection db_connect(const char* user,const char* pass,const char* server,const char* dbase){
	mysqlpp::Connection conn(false);
	if(conn.connect(dbase,server,user,pass)){
		return conn;
	}
	else{
		cerr << "DB connection failed: " << conn.error() << endl;
	}
}

int fetch_news(mysqlpp::Connection& conn){
		//query the database
		mysqlpp::Query query = conn.query("SELECT * FROM Business;");
		//store results
		if(mysqlpp::StoreQueryResult results = query.store()){
			cout << "\nNews:\n";
			//create query iterator
			mysqlpp::StoreQueryResult::const_iterator it;
			//iterate through the rows
			for(it=results.begin(); it!=results.end(); ++it){
				mysqlpp::Row row = *it;
				//print the columns of each row
				for(int i=0;i<6;i++){
					cout << '\t' << row[i];
				}
				cout << endl;
			}
			cout << endl;
		}
		else{
			cerr << "Failed to retrieve news:" << query.error() << endl;
			return 1;
		}
}
