#include "Command.h"
#include<iostream>
#include<string>
#include<algorithm>
//using namespace std;

//Parse (set start Line,end Line and command) and returns the command object 
Command Command::parseCommand(string commandLine)
{
	string trimmedCommand;

	//remove spaces and tabs from command before parsing it
	trimmedCommand = removeSpace(commandLine);

	char command =	*(trimmedCommand.end()-1);

	//For commands 1,2,3 etc
	if (isdigit(trimmedCommand[0]) && !(isalpha(trimmedCommand.at(trimmedCommand.length()-1)))) {
		setCommandLine("p");
		command = 'p';
		if (trimmedCommand.find(',') != string::npos) {
			command = 'p';
			setStartLine(trimmedCommand.substr(0, trimmedCommand.find(',')));
			setEndLine(trimmedCommand.substr(trimmedCommand.find(',') + 1, trimmedCommand.length() - 1));
		}
		else {
			command = 'p';
			setStartLine(trimmedCommand);
			setEndLine(trimmedCommand);
		}
		setCommandLine("p");
	}

	//Switch case to set start and end Line for each of the commands
	switch (command) {
	case 'a':
		if (trimmedCommand.length() == 1) {
			setStartLine(".");
			}
		else if (trimmedCommand.find(',') != string::npos) {
			if (trimmedCommand.substr(trimmedCommand.find(',') + 1, (trimmedCommand.length() - 1) - (trimmedCommand.find(',') + 1)) == "") {
				setStartLine(".");
			}
			else {
				setStartLine(trimmedCommand.substr(trimmedCommand.find(',') + 1, (trimmedCommand.length() - 1) - (trimmedCommand.find(',') + 1)));
			}
		}
		else if (isdigit(trimmedCommand[0])) {
			setStartLine(trimmedCommand.substr(0, trimmedCommand.length() - 1));
		}
		else {
			cout << "Invalid command ,Line range is missing" << endl;
		}
		setCommandLine("a");
		break;
	case 'i':
		if (trimmedCommand.length() == 1) {
			setStartLine(".");
		}
		else if (trimmedCommand.find(',') != string::npos) {
			if (trimmedCommand.substr(trimmedCommand.find(',') + 1, (trimmedCommand.length() - 1) - (trimmedCommand.find(',') + 1)) == "") {
				setStartLine(".");
			}
			else {
				setStartLine(trimmedCommand.substr(trimmedCommand.find(',') + 1, (trimmedCommand.length() - 1) - (trimmedCommand.find(',') + 1)));
			}
		}
		else if (isdigit(trimmedCommand[0])) {
			setStartLine(trimmedCommand.substr(0, trimmedCommand.length() - 1));
		}
		else {
			cout << "Invalid command ,Line range is missing" << endl;
		}
		setCommandLine("i");
		break;

	case 'r':
		if (trimmedCommand.find(",") != string::npos) {
			if (trimmedCommand.substr(0, trimmedCommand.find(',')) == "") {
				setStartLine(".");
			}
			else {
				setStartLine(trimmedCommand.substr(0, trimmedCommand.find(',')));
			}
			if (trimmedCommand.substr(trimmedCommand.find(',') + 1, (trimmedCommand.length() - 1) - (trimmedCommand.find(',') + 1)) == "") {
				setEndLine(".");
			}
			else {
				setEndLine(trimmedCommand.substr(trimmedCommand.find(',') + 1, (trimmedCommand.length() - 1) - (trimmedCommand.find(',') + 1)));
			}

		}
		else {
			cout << "Invalid Line range must be specified to remove the lines" << endl;
			invalid(getCommandLine());
			break;
		}
		setCommandLine("r");
		break;

	case 'c':

		if (trimmedCommand.find(",") != string::npos) {
			if (trimmedCommand.substr(0, trimmedCommand.find(',')) == "") {
				setStartLine(".");
			}
			else {
				setStartLine(trimmedCommand.substr(0, trimmedCommand.find(',')));
			}
			if (trimmedCommand.substr(trimmedCommand.find(',') + 1, (trimmedCommand.length() - 1) - (trimmedCommand.find(',') + 1)) == "") {
				setEndLine(".");
			}
			else {
				setEndLine(trimmedCommand.substr(trimmedCommand.find(',') + 1, (trimmedCommand.length() - 1) - (trimmedCommand.find(',') + 1)));
			}
		}
		else if (isdigit(trimmedCommand[0])) {
			setStartLine(trimmedCommand.substr(0, trimmedCommand.length() - 1));
			setEndLine(trimmedCommand.substr(0, trimmedCommand.length() - 1));
		}
		else if (trimmedCommand.length() == 1) {
			setStartLine(".");
			setEndLine(".");
		}
		else {
			cout << "Invalid Line range must be specified to remove the lines" << endl;
		}
		setCommandLine("c");
		break;
	case 'u':
		setEndLine("");
			if (isdigit(trimmedCommand[0])) {
			setStartLine(trimmedCommand.substr(0, trimmedCommand.length() - 1));
			}
			else {
				setStartLine("1");
			}
		setCommandLine("u");
		break;
	case 'd':
			setEndLine("");
			if (isdigit(trimmedCommand[0])) {
			setStartLine(trimmedCommand.substr(0, trimmedCommand.length() - 1));
			}
		else {
			setStartLine("1");
		}
		setCommandLine("d");
		break;

	case 'p': 
		//1,3p
		if (trimmedCommand.find(",") != string::npos && (isalpha(trimmedCommand.at(trimmedCommand.length() - 1)))) {
			if (trimmedCommand.substr(0, trimmedCommand.find(',')) == "") {
				setStartLine(".");
			}
			else {
				setStartLine(trimmedCommand.substr(0, trimmedCommand.find(',')));
			}
			if (trimmedCommand.substr(trimmedCommand.find(',') + 1, (trimmedCommand.length() - 1) - (trimmedCommand.find(',') + 1)) == "") {
				setEndLine(".");
			}
			else {
				setEndLine(trimmedCommand.substr(trimmedCommand.find(',') + 1, (trimmedCommand.length() - 1) - (trimmedCommand.find(',') + 1)));
			}
		}
		//1p,2p
		else if (isdigit(trimmedCommand[0]) && (isalpha(trimmedCommand.at(trimmedCommand.length() - 1)))) {
			setStartLine(trimmedCommand.substr(0, trimmedCommand.length() - 1));
			setEndLine(trimmedCommand.substr(0, trimmedCommand.length() - 1));
		}
		//p
		else if (trimmedCommand.length() == 1 && isalpha(trimmedCommand.at(0))) {
			setStartLine(".");
			setEndLine(".");
		}
		//1,2,3
		else if (isdigit(trimmedCommand[0]) && !(isalpha(trimmedCommand.at(trimmedCommand.length() - 1)))) {
			if (trimmedCommand.find(',') != string::npos) {
				setStartLine(trimmedCommand.substr(0, trimmedCommand.find(',')));
				setEndLine(trimmedCommand.substr(trimmedCommand.find(',') + 1, trimmedCommand.length() - 1));
			}
			else {
				setStartLine(trimmedCommand);
				setEndLine(trimmedCommand);
			}
		}

		else {
			cout << "Invalid Line range must be specified to print the lines" << endl;
		}
		setCommandLine("p");
		setIsNumbered(false);
		break;

	case 'n': 
		if (trimmedCommand.find(",") != string::npos) {
		if (trimmedCommand.substr(0, trimmedCommand.find(',')) == "") {
			setStartLine(".");
		}
		else if (trimmedCommand.substr(0, trimmedCommand.find(',')) == ".") {
			cout << "invalid command" << endl;
			break;
		}
		else {
			setStartLine(trimmedCommand.substr(0, trimmedCommand.find(',')));
			}
		if (trimmedCommand.substr(trimmedCommand.find(',') + 1, (trimmedCommand.length() - 1) - (trimmedCommand.find(',') + 1)) == "") {
			setEndLine(".");
			}
		else {
			setEndLine(trimmedCommand.substr(trimmedCommand.find(',') + 1, (trimmedCommand.length() - 1) - (trimmedCommand.find(',') + 1)));
		}

	}
	else if (trimmedCommand.substr(0, trimmedCommand.length() - 1) == ".") {
			setEndLine(".");
			setStartLine(".");
		}
	else {
		cout << "Invalid Command" << endl;
	}
	setCommandLine("p");
	setIsNumbered(true);
	break;

	case '$': 
		if (trimmedCommand.find(",") != string::npos) {
			if (trimmedCommand.substr(0, trimmedCommand.find(',')) == "") {
				setStartLine(".");
			}
			else {
				setStartLine(trimmedCommand.substr(0, trimmedCommand.find(',')));
			}
		}
		else if (trimmedCommand.length() == 1 && trimmedCommand.at(0) == '$') {
			setStartLine("$");
			setEndLine("$");
		}
		else {
			cout << "invalid Command" << endl;
		}
		setEndLine("$");
		cout << getEndLine();
		setCommandLine("p");
		break;

	case ',':
		if (isdigit(trimmedCommand[0])) {
			setStartLine(trimmedCommand.substr(0, trimmedCommand.find(',')));
		setEndLine(".");
	}
			  else {
				  cout << "Invalid Command" << endl;
			  }

			  setCommandLine("p");
			  break;
	case '.': 
		if (trimmedCommand.find(",") != string::npos) {
		setStartLine(trimmedCommand.substr(0, trimmedCommand.find(',')));
		setEndLine(trimmedCommand.substr(trimmedCommand.find(',') + 1, trimmedCommand.length()-1));
	}
		else {
			cout << "Invalid Command" << endl;
		}
		setCommandLine("p");
		break;
	case'=': setCommandLine("=");
		break;
	case 'q':setCommandLine("q");
		break;
	case'w':setCommandLine("w");
		break;
	case 'z': setCommandLine("z");
		break;
	default:invalid("");
		break;
	}
	return Command();
}

//Display Invalid messgae if command or the Line range is wrong.
void Command::invalid(string command)
{
	if (command == "") {
		cout << "Invalid command" << endl;
	}
}

//Remove space and tabs in the command line.
string Command::removeSpace(string str)
{
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
	return str;
}

