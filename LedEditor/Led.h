#ifndef LED_H
#define LED_H
#include <iostream>
#include<string>
#include "Command.h";
#include<list>
//using namespace std;

class Led {

public: 
		
		list<string>buffer;
		bool modified =false;
		string fileName;
		std::list<string>::iterator currentLinePointer;
		Led();
		Led(const string&);
		void run();
private:
	
	void execute(Command&);
	void append(Command &);
	void print(Command&);
	void up(Command&);
	void down(Command&);
	void remove(Command&);
	void Change(Command &);
	void insert(Command&);
	void quit();
	void printLineNumber(Command&);
	void writeToFile();
	void nextLine(Command&);
};


#endif
