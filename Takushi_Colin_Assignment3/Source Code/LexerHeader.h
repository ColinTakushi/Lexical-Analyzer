#ifndef LEXERHEADER_H
#define	LEXERHEADER_H	

#include <iostream>
#include <ostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <filesystem>
#include <map>

using namespace std;

const string KEYWORDS[] = { "int", "float", "bool", "True", "False", "if", "else", "then", "endif", 
	"endelse", "while", "whileend", "do", "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not" };
const int keySize = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]);

const char SEPARATORS[] = { '(', ')', '{', '}','[', ']', ',', '.', ':', ';', '!' };
const int sepSize = sizeof(SEPARATORS) / sizeof(SEPARATORS[0]);

const char OPERATORS[] = { '*', '+', '-', '=',  '/', '>',  '<', '%' };
const int opSize = sizeof(OPERATORS) / sizeof(OPERATORS[0]);

const int stateTable[10][7] = {
	{5, 2, 4, 1, 7, 9, 0},	//1 starting state
	{3, 2, 4, 3, 3, 9, 0},	//2 digit state
	{1, 1, 1, 1, 1, 1, 1},	//3 end of digit state
	{1, 1, 1, 1, 1, 1, 0},	//4 puncuation state
	{5, 5, 4, 6, 6, 6, 0},  //5 identifier state
	{1, 1, 1, 1, 1, 1, 1},	//6 end of identifier state
	{7, 7, 7, 7, 8, 7, 0},  //7 comment state
	{1, 1, 1, 1, 1, 1, 0},  //8 end of comment state
	{10, 9, 10, 10, 10 ,10, 0}, //9 real state
	{1, 1, 1, 1, 1, 1, 1},  //10 end of real state

};		


string itemSearch(string, int&);

string numberState(string, int&);
string puncState(string, int&);
string IDState(string, int&);
void commentState(string, int&, int&);

void strSplit(string, int&, int&);
void strSpacer(string&);
void spaceRemove(string&);
void printLex(string, string);

#endif