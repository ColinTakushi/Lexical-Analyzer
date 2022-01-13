#include "SyntaxHeader.h"
#include "LexerHeader.h"
#include "AssemblyHeader.h"

vector<vector<pair<string, string>>> syntaxVector;

void vecInitialize(string token, string lex)
{
	vector<pair<string, string>> holdVector;
	holdVector.push_back(make_pair(token, lex));

	syntaxVector.push_back(holdVector);
}

void addToVec(string token, string lex, int lineTracker)
{
	syntaxVector[lineTracker].push_back(make_pair(token, lex));
}

void lineReader(int lineTracker)
{
	int state = -1;
	
	if (syntaxVector[lineTracker][0].first == "IDENTIFIER")		//Assignemnet State
	{
		state = 1;

		for (int i = 0; i < syntaxVector[lineTracker].size(); i++)	//checking for just expression state a + 3;
		{
			if (syntaxVector[lineTracker][i].first == "OPERATOR")
				state = 5;
		}
	}
	else if (syntaxVector[lineTracker][0].first == "KEYWORD")	//Declarative State
	{
		if (syntaxVector[lineTracker][0].second == "while" ||
			syntaxVector[lineTracker][0].second == "While" ||
			syntaxVector[lineTracker][0].second == "if" 
			)
		{
			state = 4;
		}
		else if (syntaxVector[lineTracker][0].second == "else")
		{
			state = 4;
		}
		else
			state = 2;
	}
	else if (syntaxVector[lineTracker][0].first == "SEPARATOR")	//Seperator state we skip these lines
		state = 3;


	switch (state)
	{
	case 1:
		assignmentState(lineTracker);
		break;

	case 2:
		declarativeState(lineTracker);
		break;

	case 3:
		printLex(syntaxVector[lineTracker][0].first, syntaxVector[lineTracker][0].second);
		break;

	case 4:
		functionState(lineTracker);
		break;

	case 5:
		expressionState(0, lineTracker);

	default:
		break;
	}


}

void assignmentState(int lineTracker)
{
	int k = 0;

	while (syntaxVector[lineTracker][k].second != "=")
	{
		if (syntaxVector[lineTracker][k].second == ",")
			printLex(syntaxVector[lineTracker][k].first, syntaxVector[lineTracker][k].second);
		else
		{
			printLex(syntaxVector[lineTracker][k].first, syntaxVector[lineTracker][k].second);
			cout << syntaxRules[3][0] << endl;
			cout << syntaxRules[5][0] << endl;
		}

		k++;
	}

	printLex(syntaxVector[lineTracker][k].first, syntaxVector[lineTracker][k].second);		//this should always be  =
	k++;

	expressionState(k, lineTracker);
}

void expressionState(int k, int lineTracker)
{
	while (k != syntaxVector[lineTracker].size())
	{
		printLex(syntaxVector[lineTracker][k].first, syntaxVector[lineTracker][k].second);
		
		if (syntaxVector[lineTracker][k].first != "OPERATOR" && syntaxVector[lineTracker][k].first != "SEPARATOR")
		{
			if (syntaxVector[lineTracker][k + 1].first == "OPERATOR")
			{
				cout << syntaxRules[0][0] << syntaxRules[0][1] << syntaxVector[lineTracker][k + 1].second
					<< syntaxRules[0][3] << endl;

				cout << syntaxRules[0][0] << syntaxRules[0][3] << endl;
				cout << syntaxRules[1][0] << syntaxRules[1][3] << endl;

			}
			else
			{
				cout << syntaxRules[0][0] << syntaxRules[0][3] << endl;
				cout << syntaxRules[1][0] << syntaxRules[1][3] << endl;
			}

			if (syntaxVector[lineTracker][k].first == "INTEGER" ||
				syntaxVector[lineTracker][k].first == "REAL")
				cout << syntaxRules[2][0] << syntaxRules[2][3] << endl;
			else if (syntaxVector[lineTracker][k].first == "IDENTIFIER")
				cout << syntaxRules[2][0] << syntaxRules[2][2] << endl;
			else
				cout << syntaxRules[2][0] << syntaxRules[2][1] << endl;
		}
		k++;
	}
}

void declarativeState(int lineTracker)
{
	int k = 0;

	//Check if the declarative is also an expression int a = b+3;
	bool expression = false;
	
	for (int i = 0; i < syntaxVector[lineTracker].size(); i++)
	{
		if (syntaxVector[lineTracker][i].second == "=")
			expression = true;
	}
	
	if (expression)
	{
		printLex(syntaxVector[lineTracker][k].first, syntaxVector[lineTracker][k].second);
		cout << syntaxRules[6][0] << endl;
		cout << syntaxRules[7][0] << syntaxRules[7][2] << endl;
		cout << syntaxRules[8][0] << syntaxVector[lineTracker][k].second << endl;
		k++;
		expressionState(k, lineTracker);
	}
	else
	{

		printLex(syntaxVector[lineTracker][k].first, syntaxVector[lineTracker][k].second);
		cout << syntaxRules[6][0] << endl;
		cout << syntaxRules[7][0] << syntaxRules[7][1] << endl;
		cout << syntaxRules[8][0] << syntaxVector[lineTracker][k].second << endl;
		k++;

		while (syntaxVector[lineTracker][k].second != ";")
		{
			if (syntaxVector[lineTracker][k].second == ",")
				printLex(syntaxVector[lineTracker][k].first, syntaxVector[lineTracker][k].second);
			else
			{
				printLex(syntaxVector[lineTracker][k].first, syntaxVector[lineTracker][k].second);
				cout << syntaxRules[3][0] << endl;
			}

			k++;
		}
		printLex(syntaxVector[lineTracker][k].first, syntaxVector[lineTracker][k].second);
	}
}

void functionState(int lineTracker)
{
	int k = 0;
	
	printLex(syntaxVector[lineTracker][k].first, syntaxVector[lineTracker][k].second);

	cout << syntaxRules[0][0] << syntaxRules[0][3] << endl;
	cout << syntaxRules[1][0] << syntaxRules[1][3] << endl;
	cout << syntaxRules[2][0] << syntaxRules[2][1] << endl;

	if (syntaxVector[lineTracker][0].second != "else")
	{
		k++;
		printLex(syntaxVector[lineTracker][k].first, syntaxVector[lineTracker][k].second);
		k++;
		
		expressionState(k, lineTracker);
	}
}



void printSyntax()
{
	cout << "Everthing in the vectors: " << endl;

	for (int i = 0; i < syntaxVector.size(); i++)
	{
		cout << "Line " << i + 1 << ": ";
		for (int j = 0; j < syntaxVector[i].size(); j++)
		{
			cout << "(" << syntaxVector[i][j].first << ", " << syntaxVector[i][j].second << "), "; 
		}
		cout << endl;
	}
	
}
