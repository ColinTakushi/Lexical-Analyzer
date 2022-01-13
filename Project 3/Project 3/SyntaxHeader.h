#ifndef SYNTAXHEADER_H
#define SYNTAXHEADER_H

#include <iostream>
#include <ostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>


using namespace std;


void addToVec(string, string, int);
void vecInitialize(string, string);

void lineReader(int);
void assignmentState(int);
void declarativeState(int);
void functionState(int);
void expressionState(int, int);

void printSyntax();


const vector<vector<string>> syntaxRules = {
	{"<Expression> ->", " <Expression> ", " <Term>", " <Term>"},	//0
	{"<Term> ->", " <Term>* <Factor>", " <Term> / <Factor>", " <Factor>"},	//1
	{"<Factor> ->", " Function(<Expression>)", " <Identifier>", " Num"},	//2
	{"<Identifier>-> Identifier"},		//3

	{"<Statement> -> <Assign>"}, //4
	{"<Assign> -> <Identifier> = <Expression>;"},		//5

	{"<Statement> -> <Declarative>"},		//6
	{"<Declarative> -> ", "<Type> <Identifier>;", "<Type> <Expression>;"},		//7
	{"<Type> -> ",  " bool", " float", " int"},		//8
	

};

// >

// Arithmetic Expressions First: 
// <Expression> -> <Expression> + <Term> | <Expression> - <Term> | <Term>
// <Term> -> <Term> * <Factor> | <Term> / <Factor> | <Factor>
// <Factor> -> (<Expression>) | <ID> | <Num>
// <ID>->id

// Assignemnt Statements
// <Statement> -> <Assign>
// <Assign> -> <Identifier> = <Expression>;

// Declarative Statements: 
// <Statement> -> <Declarative>
// <Declarative> -> <Type> <Identifier>;
// <Type> -> bool | float | int



#endif