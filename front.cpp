/*
Purpose: simple command line interface for connecting to
          the news database.
*/
#include <iostream>
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
}

void print_news(){
  cout << "printing\n";
}
