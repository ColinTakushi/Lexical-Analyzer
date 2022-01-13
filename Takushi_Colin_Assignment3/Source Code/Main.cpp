/*
	Assignment 2 Syntax Analyzer
	Colin Takushi
	CPSC 323
	Professor Le
*/

#include "LexerHeader.h"
#include "SyntaxHeader.h"
#include "AssemblyHeader.h"


int main()
{
	int lexState = 1;
	int lineTracker = 0;
	string str;

	cout << "There are three test files: test1, test2, test3" << endl;

	
	while (str != "0")
	{	
		cout << "Enter file name or 0 to exit: " << endl;
		cin >> str;
		if(str != "0")
			str += ".txt";
		ifstream myfile(str);

		if (myfile.is_open())
		{
			cout << "TOKEN" << setw(24) << "LEXEMES" << endl;
			while (getline(myfile, str))
			{
				if (str != "")
				{
					strSplit(str, lexState, lineTracker);
					if (lexState == 1)
					{
						lineReader(lineTracker);
						lineTracker++;
					}
				}

				lexState = 1;
			}
			myfile.close();
			assemblyReader();
			printSyntax();		//used to print everthying in the vector pairs
			printAll();
		}

		else cout << "Can not open file." << endl;
	}

	

	return 0;
}

