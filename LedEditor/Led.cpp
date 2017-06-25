#include "Led.h"
#include <iostream>
#include<fstream>
#include<iterator>
#include<string>
#include<vector>
#include<algorithm>
//using namespace std;

static int count1 = 0;
Led::Led()
{
	
}

//Copy constructor to open,read and write the contents of File to buffer
Led::Led(const string &fileName)
{
	//count1 = 0;
	this->fileName = fileName;
	string line;

	std::ifstream myfile(fileName);
	if (myfile.is_open())
	{
		currentLinePointer = buffer.begin();
		while (getline(myfile, line))
		{
			count1++;
			buffer.push_back(line);
		}
		currentLinePointer = buffer.begin();
		advance(currentLinePointer, count1 - 1);
		myfile.close();

		cout << "\"" << fileName << "\"" << " ";
		cout << " " << count1 << "Lines" << endl;
	}

	else if (!myfile.is_open() && fileName!="") {
		cout << "\"" <<fileName <<"\"" <<" " ;
		cout << "[New File]" << endl;
	}
	else
	{
		cout << "\"" << "?" << "\"" << " " << "[New File]" << endl;
		currentLinePointer = buffer.begin();
	}
	
	cout << "Entering the command Mode" << endl;
	cout << ":";
	}

//Take the commands from console ,call parse of Command class and call execute method
void Led::run()
{
	string commandLine;
	Command cmd;
	do {
		cout << "Enter a command" << endl;
		cout << " :";
		getline(cin, commandLine);
		if (commandLine.empty()) {
			commandLine = "z";

		}
		cmd.parseCommand(commandLine);
		execute(cmd);
	} while (cmd.getCommandLine() != "q");

}

//discern the command and call its corresponding function 
void Led::execute(Command &command)
{
	string cmd=command.getCommandLine().c_str();
	switch (cmd[0]) {
	case 'a': append(command);
		break;
	case 'p': print(command);
		break;
	case 'u':up(command);
		break;
	case 'd':down(command);
		break;
	case'r':remove(command);
		break;
	case'c':Change(command);
		break;
	case'i':insert(command);
		break;
	case'=':printLineNumber(command);
		break;
	case'w':writeToFile();
		break;
	case'z': nextLine(command);
		break;
	case 'q' :
		quit();
		break;
	default: break;
	}
}

//Append a given line after the postion specified in the command and makes the last appened line as current line and prints it
void Led::append(Command &command)
{
	int position;
	if (command.getStartLine() == ".") {
		position = count1;
	}
	else {
		position = stoi(command.getStartLine());
	}
	
	int oldCount = buffer.size();
	std::list<string>::iterator startLinePointer;
	startLinePointer = buffer.begin();
	
	if (position>0&& position<= buffer.size()) {
		advance(startLinePointer, position);
		cout << "Entering Command Mode" << endl;
		string newLine;
		string nextLine;
		while (cin.good() && newLine != ".") {
			getline(cin, newLine);
			if (newLine!=".") {
				buffer.insert(startLinePointer, newLine);
				}
				else {
					break;
				}
			}

		if (count1 > position) {
			currentLinePointer = prev(currentLinePointer, count1 - position);
			cout << *currentLinePointer << endl;
			int difference = buffer.size() - oldCount;
			count1 = difference + position;
		}
		else if(count1<position ) {
			
			int difference = buffer.size() - oldCount;
			currentLinePointer = next(currentLinePointer, difference+position-1);
			count1 = difference + position;
			cout << *currentLinePointer << endl;
			}
		else if (count1 == position) {
			int difference = buffer.size() - oldCount;
			currentLinePointer = next(currentLinePointer, difference);
			count1 = difference+position;
			cout << *currentLinePointer << endl;
		}
		modified = true;
	}
	else {
		cout << "Invalid position" << endl;
	}
}

//Prints the content of buffer and makes the last line printed as current Line
void Led::print(Command &command) {
	
	string startLine =command.getStartLine();
	string endLine = command.getEndLine();
	std::list<string>::iterator endLinePointer;
	std::list<string>::iterator startLinePointer;
	startLinePointer = buffer.begin();
	endLinePointer = buffer.begin();
	int startLine1;
	int endLine1;
	if (startLine == ".") {
		startLine1 = count1;
	}
	if (endLine == ".") {
		 endLine1 = count1;
	}
	if (startLine == "." && endLine != "$") {
		int numbered = count1;
		if (command.getIsNumbered()) {
			cout << numbered++ << " " << *currentLinePointer << endl;
			count1 = endLine1;
		}
		else {
			cout << *currentLinePointer << endl;
		}
	}
	else if (endLine == ".") {
		startLine1 = stoi(command.getStartLine());
		int  numbered = startLine1;
		if (startLine1 >=1) {
			advance(startLinePointer, startLine1 - 1);
			advance(endLinePointer, count1-1);
				if (command.getIsNumbered()) {
				for (list<string>::iterator it = startLinePointer; it != endLinePointer; it++) {
					cout << numbered++ << " " << *it << endl;
				}
				cout << numbered++ << " " << *endLinePointer << endl;
			}
			else {
				for (list<string>::iterator it = startLinePointer; it != endLinePointer; it++) {
					cout <<  *it << endl;
				}
				cout << *endLinePointer << endl;
			}

		}
		else {
			cout << "Invalid range" << endl;
		}
		
	}
	else if (endLine == "$" && startLine!="$") {
		int numbered = count1;
		if (startLine == ".")
			startLine1 = count1;
			
		else {
			startLine1 = stoi(command.getStartLine());
			numbered = startLine1;
		}
		 endLine1 = buffer.size();
		if (startLine1 >= 1 && startLine1<=buffer.size()) {
			advance(startLinePointer, startLine1-1);
			advance(endLinePointer, endLine1);
			if (command.getIsNumbered()) {
				for (list<string>::iterator it = startLinePointer; it != endLinePointer; it++) {
					cout << numbered++ << " " << *it << endl;
				}
			}
			else {
				for (list<string>::iterator it = startLinePointer; it != endLinePointer; it++) {
					cout << *it << endl;
				}
			}
			if (count1<endLine1 || count1 == endLine1) {
				currentLinePointer = next(currentLinePointer, endLine1 - count1);
			}
			count1 = endLine1;
		}
		else {
			cout << "Start Line is out of range " << endl;
		}
		
	}
	else if (startLine == "$" && endLine == "$") {
		currentLinePointer = next(currentLinePointer, buffer.size() - count1);
		count1 = buffer.size();
		cout << *currentLinePointer << endl;
	}
	else {
		if (command.getStartLine() == "") {
			startLine1 = count1;
		}
		else {
			startLine1 = stoi(command.getStartLine());
		}
		if (command.getEndLine() == "") {
			endLine1 = count1;
		}
		else {
			endLine1 = stoi(command.getEndLine());
		}
		int numbered = startLine1;
		if (startLine1 >= 1 && endLine1 <= buffer.size()) {
			advance(startLinePointer, startLine1 - 1);
			advance(endLinePointer, endLine1);
			if (command.getIsNumbered()) {
				for (list<string>::iterator it = startLinePointer; it != endLinePointer; it++) {
					cout << numbered++ << " " << *it << endl;
				}
			}
			else {
				for (list<string>::iterator it = startLinePointer; it != endLinePointer; it++) {
					cout << *it << endl;
				}
			}	
			if (count1 > endLine1) {
				currentLinePointer = prev(currentLinePointer, count1 - endLine1);
			}
			else if (count1<endLine1 || count1 == endLine1) {
				currentLinePointer = next(currentLinePointer, endLine1 - count1);
			}
			count1 = endLine1;
		}
		
		else {
			cout << "Line range is invalid" << endl;
		}
		
	}
	
}

//Moves the current Line up by specificed postion , if the position to move by is not specified then it moves up by one line
//Moves untill begining of file is reached 
void Led::up(Command &command)
{
	int moveUpBy = stoi(command.getStartLine());
	std::list<string>::iterator topIterator;
	topIterator = buffer.begin();
	int dist = distance(topIterator, currentLinePointer);
	if (dist >= moveUpBy) {
		currentLinePointer = prev(currentLinePointer, moveUpBy);
		count1 = count1 - moveUpBy;
		cout << *currentLinePointer << endl;
	}
	else {
		cout << "BOF reached " << endl;
	}
	
}

//Moves the current Line down by specificed postion , if the position to move by is not specified then it moves down by one line
//Moves untill end of file is reached 
void Led::down(Command &command)
{
	int moveDownBy = stoi(command.getStartLine());
	std::list<string>::iterator downIterator;
	downIterator = buffer.begin();
	advance(downIterator, buffer.size()-1);
	int dist = distance(currentLinePointer, downIterator);
	//cout << "distance is " << dist << endl;
	if (dist >= moveDownBy) {
		currentLinePointer = next(currentLinePointer, moveDownBy);
		count1 = count1+moveDownBy;
		
		cout << *currentLinePointer << endl;
	}
	else {
		cout << "EoF reached" << endl;
	}
}


//Removes the line specified in the range from the buffer , making the current line point line next to the line removed.
//if the line removed is the last line ,then line above is made the current line
//if its the last element is being removed ,it forces user to either quit or add new content to buffer

void Led::remove(Command &command)
{
	int startLine = stoi(command.getStartLine());
	int endLine = stoi(command.getEndLine());
	list<string>::iterator startIterator;
	list<string>::iterator endIterator;
	startIterator = endIterator = buffer.begin();
	advance(startIterator, startLine-1);
	advance(endIterator, endLine-1);
	int size = buffer.size();
	if (count1 < endLine) {
		if (endLine < size) {
			if (count1 == startLine) {
				currentLinePointer = next(currentLinePointer, (endLine+1)-count1);
				count1 =startLine;
				buffer.erase(startIterator, endIterator);
				buffer.erase(endIterator);
				}
			 else {
				 currentLinePointer = next(currentLinePointer, (endLine + 1) - count1);
				 count1 = startLine;
				 buffer.erase(startIterator, endIterator);
				 buffer.erase(endIterator);
			 }
		}
		else if (endLine == size) {
				if (count1 == startLine - 1) {
				buffer.erase(startIterator, endIterator);
				buffer.erase(endIterator);
			}
			else {
				currentLinePointer = next(currentLinePointer, ((startLine -1)-count1));
				buffer.erase(startIterator, endIterator);
				buffer.erase(endIterator);
				count1 = startLine - 1;
			}

		}
	}
	else if (count1 == endLine ) {
		char choice;
		if (buffer.size() == 1) {
			cout << "This is the Only Line left " << endl;
			cout << "Do you prefer to insert a new Line or Quit led" << endl;
			cin >> choice;
			switch (choice) {
			case 'i': command.setCommandLine("i");
					  command.setStartLine("1");
					  insert(command);
					  break;
			case 'q':quit();
					break;
			default: break;
			}
		}
		
		else{
			currentLinePointer = prev(currentLinePointer, (endLine - startLine) + 1);
			buffer.erase(startIterator, endIterator);
			buffer.erase(endIterator);
			count1 = startLine - 1;
		}
		
	}
	else if (count1 > endLine) {
		if (count1 == endLine + 1) {
			buffer.erase(startIterator, endIterator);
			buffer.erase(endIterator);
			currentLinePointer = prev(currentLinePointer, 0);
			count1 = 1;
			cout << count1 << endl;
		}
		else {
			buffer.erase(startIterator, endIterator);
			buffer.erase(endIterator);
			currentLinePointer  = prev(currentLinePointer, count1-(endLine+1));
			count1 = startLine;
			}
	}
	modified = true;
	cout << *currentLinePointer << endl;
}


//replace a given string with new string in the specified range and current line points to changed line
void Led::Change(Command & command)
{
	string toChange;
	string replaceText;
	string findText;
	int startLine1;
	int endLine1;
	std::list<string>::iterator startLineiterator;
	startLineiterator = buffer.begin();
	std::list<string>::iterator endLineiterator;
	endLineiterator = buffer.begin();
	if (command.getStartLine() == ".") {
		startLine1 = count1;
		advance(startLineiterator, count1-1);
	}
	else {
		startLine1 = stoi(command.getStartLine());
		advance(startLineiterator, startLine1-1);
	}
	if (command.getEndLine() == "."){
		endLine1 = count1;
		advance(endLineiterator, count1);
	}
	else {
		endLine1 = stoi(command.getEndLine());
		advance(endLineiterator, endLine1);
	}
	cout << "Change What? " << endl;
	getline(cin, toChange);
	cout << "To what? " << endl;
	getline(cin, replaceText);
	
	for (list<string>::iterator it = startLineiterator; it != endLineiterator; it++) {
		findText = *it;
		int position = 0;
		while ((position = findText.find(toChange, position)) != std::string::npos) {
			findText.replace(position, toChange.length(), replaceText);
			position += replaceText.length();
		}
		*it = findText;
	}
	if (count1 > endLine1) {
		currentLinePointer = prev(currentLinePointer, count1 - endLine1);
	}
	else if (count1 < endLine1) {
		currentLinePointer = next(currentLinePointer, endLine1-count1);
	}
	count1 = endLine1;
	cout << *currentLinePointer<<endl;
	modified = true;
	//cin.ignore();
	}

//Insert a new Line/Lines to before the specified position and makes the current Line point newly inserted Line.
void Led::insert(Command &command) {

	int position;
	int difference;
	int oldCount = buffer.size();
	if (command.getStartLine() == "." && buffer.size() == 0)
	{
		if (buffer.size() == 0)
		{
			 position = 0;
			std::list<string>::iterator startLinePointer;
			startLinePointer = buffer.begin();
			cout << "Entering Command Mode" << endl;
			string newLine;
			string nextLine;
			while (cin.good() && newLine != ".")
			{
				getline(cin, newLine);
				if (newLine != ".")
				{
					
					buffer.insert(startLinePointer, newLine);
				}

				else {
					break;
				}
			}
				currentLinePointer = buffer.begin();
				count1 = buffer.size();
			    currentLinePointer = next(currentLinePointer, count1-1);
			    cout << *currentLinePointer << endl;
		}
	}
	else 
	{
		if (command.getStartLine() == ".") {
			position = count1;
		}
		else {
			position = stoi(command.getStartLine());
		}
		
		std::list<string>::iterator startLinePointer;
		startLinePointer = buffer.begin();
		if (position > 0 && position <= buffer.size())
		{
			advance(startLinePointer, position - 1);
			cout << "Entering Command Mode" << endl;
			string newLine;
			string nextLine;
			while (cin.good() && newLine != ".") {
				getline(cin, newLine);
				if (newLine != ".") {
					buffer.insert(startLinePointer, newLine);
				}
				else {
					break;
				}

			}
			difference = buffer.size() - oldCount;
			if (count1 > position) {
				currentLinePointer = prev(currentLinePointer, (count1 - (position + 1)) + 2);
				cout << *currentLinePointer << endl;
				count1 = (position - 1) + difference;
			}
			else if (count1 == position) {
				//currentLinePointer = prev(currentLinePointer, position - 4);
				currentLinePointer = prev(currentLinePointer, 1);
				cout << *currentLinePointer << endl;
				count1 = (difference+position)-1;
			}
			else if (count1 < position) {
				currentLinePointer = next(currentLinePointer, ((position - count1) - 1) + difference);
				cout << *currentLinePointer << endl;
				count1 = (position - 1) + difference;
			}
		}
	}
	modified = true;
}

//Quits form Led , and prompts for saving the contents of buffer to file before quitting.
void Led::quit()
{
	char choice;
	cout << "Save changes to " << fileName << " (y/n) " << endl;
	cin >> choice;
	if (choice == 'y' && modified==true) {
		writeToFile();
	}
	else if(choice=='n') {
		cout << "Changes not saved to File" << endl;
	}
	else {
		cout << "Invalid answer : " << choice << endl;
	}
	cout << "Quitting Led . . . bye." << endl;
	exit(0);	
}

//Prints the current Line  " = "
void Led::printLineNumber(Command &command)
{
	cout << count1 << endl;
}

//Writes the conents of  buffer to file and prompts for the filename if the file name were found to be missing
void Led::writeToFile()
{
	
	ofstream myfile(fileName);

	if (myfile.is_open()) {
		for (list<string>::iterator it = buffer.begin(); it != buffer.end(); it++) {
			myfile << *it << endl;
		}
		myfile.close();
		
	}
	
	else {
		string newFileName;
		cout << "Enter a file Name to save the contents of Buffer" << endl;
		cin >> newFileName;
		ofstream myNewfile(newFileName);

		if (myNewfile.is_open()) {
			for (list<string>::iterator it = buffer.begin(); it != buffer.end(); it++) {
				myNewfile << *it << endl;
			}
			myNewfile.close();
		}
		fileName = newFileName;
	}
	cout << fileName << buffer.size() << "  written" << endl;
	//myfile << "Writing this to a file.\n";
	cin.ignore();

}

//Moves down by 1 line , each time enter key is hit ,untill the end of file is reached
void Led::nextLine(Command &command)
{
	command.setStartLine("1");
	down(command);
}







