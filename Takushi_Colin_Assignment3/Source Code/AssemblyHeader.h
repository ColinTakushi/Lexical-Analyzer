#ifndef ASSEMBLYHEADER_H
#define ASSEMBLYHEADER_H	

#include <iostream> 
#include <iomanip>
#include <vector>
#include <tuple>
#include <string>

using namespace std;

	void assemblyReader();
	void declareState(int);
	void assignState(int);
	void expressState(int);
	void funcState(int);
	int findEnd(int);
	int findElse(int);
	bool declareCheck(int, string);
	string returnValue(int, int);

	void printAll();

#endif