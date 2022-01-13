#include "LexerHeader.h"
#include "SyntaxHeader.h"


string itemSearch(string input, int& lexState)		
{
	int column = -1; 

	if (isalpha(input[0]))
	{
		column = 1;
	}
	else if(isdigit(input[0]))
	{
		column = 2;
	}
	else //if input[0] is puncuation
	{
		column = 3;
	}

	//- 1 becuase arrays index starting at 0 and its eaiser to read
	//when just putting the column number 
	lexState = stateTable[lexState - 1][column - 1];

	switch (lexState)
	{
	case(-1):
		return "ERROR STATE NOT CHANGED";
		break;

	case(5):
		return IDState(input, lexState);
		break;

	case(2):
		return numberState(input, lexState);
		break;


	case(4):
		return puncState(input, lexState);
		break;

	default:
		return "UNKNOWN";
		break;
	}
}

string IDState(string input, int& lexState)
{
	int column; 

	//KEYWORD SEARCH							//START OF LETTER STATE
	for (int i = 0; i < keySize; i++)
	{
		if (input == KEYWORDS[i])			//START OF KEYWORD STATE
		{
			return "KEYWORD";				//END OF KEYWORD STATE
		}
	}

	//IDENTIFIER CHECK
	if (isalpha(input[0]))					//START OF IDENTIFIER STATE
	{
		for (int i = 0; i < input.length(); i++)
		{
			if (!isalpha(input[i]) && !isdigit(input[i]) && input[i] != '_' && input[i] != '$')
			{
				if (input[i] == '!')
					column = 5;
				else if (input[i] == '.')
					column = 6;
				else
					column = 3;

				lexState = stateTable[lexState - 1][column - 1];
				return "INVALID IDENTIFIER INPUT";			//END OF IDENTIFIER STATE ERROR
			}
		}
		
		column = 4;
		lexState = stateTable[lexState - 1][column - 1];
		return "IDENTIFIER";			//END OF IDENTIFIER STATE 
	}

	column = 3;
	lexState = stateTable[lexState - 1][column - 1];
	return "INVALID IDENTIFIER INPUT";		//END STATE ERROR
}

string numberState(string input, int& lexState)
{
	int column; 
	for (int i = 0; i < input.length(); i++)
	{

		if (!isdigit(input[i]) && input[i] != '.' && lexState != 9)
		{
			if(isalpha(input[i]))
				column = 1;
			else if (ispunct(input[i]))
			{
				if (input[i] == '!')
					column = 5;
				else if (input[i] == '.')
					column = 6;
				else
					column = 3;
			}

			lexState = stateTable[lexState - 1][column - 1];
			return "INVALID DIGIT INPUT";				//END OF DIGIT STATE ERROR
		}
		else if (!isdigit(input[i]) && lexState == 9)
		{
			if (isalpha(input[i]))
				column = 1;
			else if (ispunct(input[i]))
			{
				if (input[i] == '!')
					column = 5;
				else if (input[i] == '.')
					column = 6;
				else
					column = 3;
			}

			lexState = stateTable[lexState - 1][column - 1];

			return "INVALID DIGIT INPUT";			//END OF REAL STATE ERROR
		}
		else if (input[i] == '.')
		{
			column = 6;			//START OF REAL STATE
			lexState = stateTable[lexState - 1][column - 1];
		}
	}

	if (lexState == 9) 
		return "REAL";			//END OF REAL STATE
	else
		return "INTEGER";					//END OF DIGIT STATE
}

string puncState(string input, int& lexState)
{
	int column;

	//OPERATOR SEARCH
	for (int i = 0; i < opSize; i++)		//START OF OPERATOR STATE
	{
		if (input[0] == OPERATORS[i])
			return "OPERATOR";				//END OF OPERATOR STATE
	}

	//SEPERATOR SEARCH
	for (int i = 0; i < sepSize; i++)		//START OF SEPERATOR STATE
	{
		if (input[0] == SEPARATORS[i])
			return "SEPARATOR";				//END OF SEPERATOR STATE
	}


	if (input[0] == '!')
		column = 5;
	else if (input[0] == '.')
		column = 6;
	else if (input[0] == ' ')
	{
		column = 4;
		lexState = stateTable[lexState - 1][column - 1];

	}
	else
		column = 3;

	lexState = stateTable[lexState - 1][column - 1];

	return "INVALID PUNCUATION INPUT";
}

void commentState(string str, int& i, int& lexState)
{
	int column = 5; 
	lexState = stateTable[lexState - 1][column - 1];

	i++;
	while (str[i] != '!')
		i++;

}

//Splitting the string by space
void strSplit(string str, int& lexState, int& lineTracker)
{
	string hold;
	bool newLine = true;
	strSpacer(str);

	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '!' || str[i+1] == '!')			//COMMENT STATE
		{
			if (str[i + 1] == '!')
				i++;

			commentState(str, i, lexState);

		}
		else
		{
			hold += str[i];
			if (str[i] == ' ' || str[i + 1] == NULL)
			{
				spaceRemove(hold);
				string token = itemSearch(hold, lexState);

				if (newLine && token[2] != 'V' && hold != "\t" && hold.length() > 0)		// token[2] != 'V' is checking if the input is invalid 
				{
					vecInitialize(token, hold);
					newLine = false;
				}
				else if (hold.length() > 0 && hold != " ")
				{
					addToVec(token, hold, lineTracker);
				}
			
				//print(token, hold);		//For testing purposes
				hold = "";
				lexState = 1;
			}
		}
	}
}

//Adding space before and after puncuation and operators
void strSpacer(string& str)
{
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] != '.' && (!isdigit(str[i + 1]) || !isdigit(i - 1)))
		{
			for (int j = 0; j < sepSize; j++)
			{
				if (str[i] == SEPARATORS[j] || str[i] == OPERATORS[j])
				{
					str.insert(i, " ");

					if (str[i + 2] != NULL)
						i += 2;
					else
						i++;

					str.insert(i, " ");
					i++;
				}
			}
		}

	}
}

//removes any extra space
void spaceRemove(string& str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == ' ' || str[i] == '\t')
			str.erase(i);
	}
}

//prints output
void printLex(string token, string lex)
{
	
	if (lex.length() > 0)
	{
		cout << "------------------------------------------------------------" << endl;
		cout << left << setw(15) << token << setw(7) << " " << setw(7) << lex << endl;
	}
	
}