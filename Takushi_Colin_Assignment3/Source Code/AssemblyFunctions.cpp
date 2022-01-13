#include "AssemblyHeader.h"
#include "StackHeader.h"

extern vector<vector<pair<string, string>>> syntaxVector;

//creating the assembly array
pair<string, string> assemblyArr[1000];
int arrPlace = 0;

//creating the symbol table
// Identifier, Memorylocation, Type
vector<tuple<string, int, string>> symbolTable;
int memLoc = 2000;

//global of where to jump
int jumpBeg = 0;
bool inFunction = false;
bool elseFunction = false;
int jumpEnd = 0;

void assemblyReader()
{
	for (int i = 0; i < syntaxVector.size(); i++)		//iterating though the lines
	{
		if (syntaxVector[i][0].first == "KEYWORD")
		{
			if (syntaxVector[i][0].second == "while" ||
				syntaxVector[i][0].second == "While" ||
				syntaxVector[i][0].second == "if")
			{
				funcState(i);
			}
			else if( syntaxVector[i][0].second != "else")
				declareState(i);


			if (syntaxVector[i].size() > 2)
			{
				if (syntaxVector[i][2].second == "=")		//case of assign and declare ie. int a = 2;
					assignState(i);
			}
		}
		else if (syntaxVector[i][0].first == "IDENTIFIER")		
		{
			assignState(i);
		}
		else if (syntaxVector[i][0].second == "}" &&
			inFunction && !elseFunction)
		{
			//end of a while function
			assemblyArr[arrPlace] = make_pair("JUMP", to_string(jumpBeg));
			arrPlace++;
		}
		
	}
}

bool delcareCheck(int i, string input)
{
	bool inTheTable = false;

	for (int j = 0; j < symbolTable.size(); j++)
	{
		if (get<0>(symbolTable[j]) == input)
		{
			cerr << "VALUE IS ALREADY IN THE TABLE" << endl;
			inTheTable = true;
		}
	}

	return inTheTable;
}

void declareState(int i)
{
	string type = syntaxVector[i][0].second;
	
	for (int j = 1; j < syntaxVector[i].size(); j++)
	{
		if (syntaxVector[i][j].first == "IDENTIFIER")
		{
			if (syntaxVector[i][j].second != "true" && 
				syntaxVector[i][j].second != "false"
				&& !declareCheck(i, syntaxVector[i][j].second))
			{
				symbolTable.push_back(make_tuple(syntaxVector[i][j].second, memLoc, type));
				memLoc++;
			}
		}
	}
}

int findMemLocation(int i, int position)
{
	int hold = -1;
	for (int j = 0; j < symbolTable.size(); j++)
	{
		if (get<0>(symbolTable[j]) == syntaxVector[i][position].second)
			hold = get<1>(symbolTable[j]);
	}

	return hold;
}

string returnValue(int i, int position)
{
	string hold = "NULL";
	for (int j = 0; j < symbolTable.size(); j++)
	{
		if (get<0>(symbolTable[j]) == syntaxVector[i][position].second)
			hold = get<2>(symbolTable[j]);
	}

	return hold;
}

void assignState(int i)
{
	bool expression = false;
	bool stateTF = false;

	for (int j = 0; j < syntaxVector[i].size(); j++)
	{
		//checks for expression
		if (syntaxVector[i][j].first == "OPERATOR" && syntaxVector[i][j].second != "=")
		{
			expressState(i);
			expression++;
		}

		//check for bool
		if (syntaxVector[i][j].second == "bool" || 
			syntaxVector[i][j].second == "true" ||
			syntaxVector[i][j].second == "false" ||
			returnValue(i, j) == "bool")
			stateTF++;
	}

	if (stateTF)
	{
		//boolean state 
		//find the value
		int valPlace = 0;
		int boolPlace = 0;
		bool secondBool = false;
		int secondPlace= 0;
		for (int j = 0; j < syntaxVector[i].size(); j++)
		{
			if (syntaxVector[i][j].second == "true" ||
				syntaxVector[i][j].second == "false")
				valPlace = j;
			else if (syntaxVector[i][j].first == "IDENTIFIER" && !secondBool)
			{
				boolPlace = j;
				secondBool++;
			}
			else if (syntaxVector[i][j].first == "IDENTIFIER" && secondBool)
			{
				secondPlace = j;
			}
		}

		if (secondBool)
		{
			int hold = findMemLocation(i, secondPlace);
			assemblyArr[arrPlace] = make_pair("PUSHM", to_string(hold));		//state of assigning bool to bool et flag = test; 
			PUSHM(hold);
		}
		else if (syntaxVector[i][valPlace].second == "true")
		{
			assemblyArr[arrPlace] = make_pair("PUSHM", "1");		//true state
			PUSHM(1);
		}
		else if(syntaxVector[i][valPlace].second == "false")
		{
			assemblyArr[arrPlace] = make_pair("PUSHM", "0");		//false state
			PUSHM(0);
		}
		arrPlace++;

		int hold = findMemLocation(i, boolPlace);

		assemblyArr[arrPlace] = make_pair("POPM", to_string(hold));
		arrPlace++;
		POPM();
		
	}
	else if (!expression)
	{
		//checking for multiple identifiers
		bool secondId = false;
		int count = 0;
		for (int j = 0; j < syntaxVector[i].size(); j++)
		{
			if (syntaxVector[i][j].first == "IDENTIFIER")
				count++;
		}

		if (count > 1)
			secondId++;

		int valPlace = 0;
		bool flag = false;

		if (secondId)
		{

			for (int j = 0; j < syntaxVector[i].size(); j++)
			{
				if (syntaxVector[i][j].first == "IDENTIFIER")
					valPlace = j;
			}

		}
		else
		{
		//finding the location of the value to push
			while (syntaxVector[i][valPlace].first != "INTEGER")
			{
			valPlace++;
			}
		}

		//getting loctaion of the identifer
		int namePlace = 0;
		while (syntaxVector[i][namePlace].first != "IDENTIFIER")
		{
			namePlace++;
		}

		//finding the loaciton of the address
		int dataPlace = -1;

		for (int k = 0; k < symbolTable.size(); k++)
		{
			if (get<0>(symbolTable[k]) == syntaxVector[i][namePlace].second)
				dataPlace = k;
		}

		//pushing to stack
		if (secondId)
		{
			//find location of second ID
			int hold = findMemLocation(i, valPlace);
			assemblyArr[arrPlace] = make_pair("PUSHM", to_string(hold));
			PUSHM(hold);
		}
		else
		{
			assemblyArr[arrPlace] = make_pair("PUSHI", syntaxVector[i][valPlace].second);
			PUSHI(stoi(syntaxVector[i][valPlace].second));
		}
		arrPlace++;

		//popping the palce to with place to store
		assemblyArr[arrPlace] = make_pair("POPM", to_string(get<1>(symbolTable[dataPlace])));
		arrPlace++;
		POPM();
	}
}

void expressState(int i)
{
	//find the two things adding
	//push add to stack
	//find the thing we are adding to 

	int firstInt = -1;
	int secondInt = -1;
	int thirdInt = -1;
	int typeOfOperation = -1;

	for (int j = syntaxVector[i].size() - 1; j >= 0; j--)
	{
		if (syntaxVector[i][j].first == "IDENTIFIER" ||
			syntaxVector[i][j].first == "INTEGER")
		{
			if (thirdInt == -1)
				thirdInt = j;
			else if (secondInt == -1)
				secondInt = j;
			else
				firstInt = j;
		}

		if (syntaxVector[i][j].first == "OPERATOR" &&
			syntaxVector[i][j].second != "=")
			typeOfOperation = j;
	}

	//pushing second Id to stack
	if (syntaxVector[i][secondInt].first == "IDENTIFIER")
	{

		int hold = findMemLocation(i, secondInt);

		assemblyArr[arrPlace] = make_pair("PUSHM", to_string(hold));
		PUSHM(hold);
	}
	else
	{
		//case of a value sucha as num = num1 + 2
		assemblyArr[arrPlace] = make_pair("PUSHI", syntaxVector[i][secondInt].second);
		PUSHI(stoi(syntaxVector[i][secondInt].second));
	}
	arrPlace++;

	//pushing third Id to stack
	if (syntaxVector[i][thirdInt].first == "IDENTIFIER")
	{
		int hold = findMemLocation(i, thirdInt);

		assemblyArr[arrPlace] = make_pair("PUSHM", to_string(hold));
		PUSHM(hold);
	}
	else
	{
		//case of a value sucha as num = num1 + 2;
		assemblyArr[arrPlace] = make_pair("PUSHI", syntaxVector[i][thirdInt].second);
		PUSHI(stoi(syntaxVector[i][thirdInt].second));
	}
	arrPlace++;

	//handeling the operation
	if (syntaxVector[i][typeOfOperation].second == "+")
	{
		assemblyArr[arrPlace] = make_pair("ADD", " ");
		ADD();
	}
	else if (syntaxVector[i][typeOfOperation].second == "-")
	{
		assemblyArr[arrPlace] = make_pair("SUB", " ");
		SUB();
	}
	else if (syntaxVector[i][typeOfOperation].second == "*")
	{
		assemblyArr[arrPlace] = make_pair("MUL", " ");
		MUL();
	}
	else if (syntaxVector[i][typeOfOperation].second == "/")
	{
		assemblyArr[arrPlace] = make_pair("DIV", " ");
		DIV();
	}
	arrPlace++;

	int hold = findMemLocation(i, firstInt);
	assemblyArr[arrPlace] = make_pair("POPM", to_string(hold));
	POPM();
	arrPlace++;
}

int findEnd(int i)
{
	int count = 0;

	while (syntaxVector[i][0].second != "}")
	{
		bool flag = false;
		int j = syntaxVector[i].size() - 1;
		while(!flag && j >=0 )
		{
			if (syntaxVector[i][j].second == "+")
			{
				count += 4;
				flag++;
			}
			else if (syntaxVector[i][j].second == "=")
			{
				count += 2;
				flag++;
			}
			j--;
		}
		i++;
	}
	return count;
}

int findElse(int i)
{
	int count = 0;
	elseFunction++;

	while (syntaxVector[i][0].second != "else" &&
		i < syntaxVector.size())
	{		
		bool flag = false;
		int j = syntaxVector[i].size() - 1;
		while (!flag && j >= 0)
		{
			if (syntaxVector[i][j].second == "+")
			{
				count += 4;
				flag++;
			}
			else if (syntaxVector[i][j].second == "=")
			{
				count += 2;
				flag++;
			}
			j--;
		}
		i++;
	}

	return count;
}

void funcState(int i)
{
	int posOne = 0;
	int posTwo = 0;
	int operationPosOne = 0;
	int operationPosTwo = -1;
	bool operationFlag = false;
	bool idFlag = false;

	for (int j = 0; j < syntaxVector[i].size(); j++)
	{
		if (syntaxVector[i][j].first == "IDENTIFIER" &&
			!idFlag)
		{
			posOne = j;
			idFlag++;
		}
		else if (syntaxVector[i][j].first == "IDENTIFIER" &&
			idFlag)
		{
			posTwo = j;
		}
		else if (syntaxVector[i][j].first == "OPERATOR" &&
			!operationFlag)
		{
			operationPosOne = j;
			operationFlag++;
		}
		else if (syntaxVector[i][j].first == "OPERATOR" &&
			operationFlag)
		{
			operationFlag = j;
		}
	}
	
	if (syntaxVector[i][0].second == "while")
	{
		inFunction = true;
		jumpBeg = arrPlace + 1;

		assemblyArr[arrPlace] = make_pair("LABEL", "");
		arrPlace++;

	}
	
	int hold = findMemLocation(i, posOne);

	assemblyArr[arrPlace] = make_pair("PUSHM", to_string(hold));
	PUSHM(hold);
	arrPlace++;

	hold = findMemLocation(i, posTwo);

	assemblyArr[arrPlace] = make_pair("PUSHM", to_string(hold));
	PUSHM(hold);
	arrPlace++;

	if (operationPosTwo == -1)
	{
		if (syntaxVector[i][operationPosOne].second == "<")
		{
			assemblyArr[arrPlace] = make_pair("LES", "");
			LES();
			arrPlace++;
		}
		else if (syntaxVector[i][operationPosOne].second == ">")
		{
			assemblyArr[arrPlace] = make_pair("GRT", "");
			GRT();
			arrPlace++;
		}
	}
	else
	{
		if (syntaxVector[i][operationPosOne].second == "<")
		{
			assemblyArr[arrPlace] = make_pair("LEQ", "");
			LEQ();
			arrPlace++;
		}
		else if (syntaxVector[i][operationPosOne].second == ">")
		{
			assemblyArr[arrPlace] = make_pair("GEQ", "");
			GEQ();
			arrPlace++;
		}
		else if (syntaxVector[i][operationPosOne].second == "=")
		{
			assemblyArr[arrPlace] = make_pair("EQU", "");
			EQU();
			arrPlace++;
		}
		else if (syntaxVector[i][operationPosOne].second == "!")
		{
			assemblyArr[arrPlace] = make_pair("NEQ", "");
			NEQ();
			arrPlace++;
		}	
	}

	if (syntaxVector[i][0].second == "while")
	{
		int keep = findEnd(i);
		keep += arrPlace + 3;
		assemblyArr[arrPlace] = make_pair("JUMPZ", to_string(keep));
		arrPlace++;
	}
	else if (syntaxVector[i][0].second == "if")
	{
		int keep = findElse(i);

		keep += arrPlace + 2;

		assemblyArr[arrPlace] = make_pair("JUMPZ", to_string(keep));
		arrPlace++;

	}
}

void printAll()
{
	cout << "PRINTING ASSEMBLY ARRAY" <<
		"\n------------------------------------------" << endl;
	for (int i = 0; i < arrPlace; i++)
	{
		cout << i+1 << "         " << assemblyArr[i].first << "         " << assemblyArr[i].second << endl;
	}
	cout << "------------------------------------------" << endl;

	cout << "PRINTING SYMBOL TABLE" <<
		"\n------------------------------------------" << endl;
	for (int i = 0; i < symbolTable.size(); i++)
	{
		cout << i+1 << "         " << get<0>(symbolTable[i]) << "         " << get<1>(symbolTable[i]) << "         "
			<< get<2>(symbolTable[i]) << endl;
	}
	cout << "------------------------------------------" << endl;

}