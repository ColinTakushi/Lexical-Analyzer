#include "StackHeader.h"
//creating the stack
int stackArr[100];
int top = -1;


void PUSHI(int val)
{
	if (top >= 99)	//stack max is 100
	{
		cout << "Stack Overflow" << endl;
	}
	else
	{
		stackArr[++top] = val;
	}
}

void PUSHM(int val)
{
	if (top >= 99)	//stack max is 100
	{
		cout << "Stack Overflow" << endl;
	}
	else
	{
		stackArr[++top] = val;
	}
}

int POPM()
{
	if (top < 0)
	{
		cout << "Stack empty" << endl;
		return -1;
	}
	else
	{
		int hold = stackArr[top];
		top--;
		return hold;
	}

}

int STDOUT()
{
	if (top < 0)
	{
		cout << "Stack empty" << endl;
		return -1;
	}
	else
	{
		int hold = stackArr[top];
		top--;
		cout << "Value is: " << hold;
		return hold;
	}
}

void STDIN(int val)
{
	if (top >= 99)	//stack max is 100
	{
		cout << "Stack Overflow" << endl;
	}
	else
	{
		stackArr[++top] = val;
	}
}

void ADD()
{
	int a, b;
	a = stackArr[top];
	top--;
	b = stackArr[top];
	top--;

	a += b;

	stackArr[++top] = a;
}

void SUB()
{
	int a, b;
	a = stackArr[top];
	top--;
	b = stackArr[top];
	top--;

	a = b - a;

	stackArr[++top] = a;
}

void MUL()
{
	int a, b;
	a = stackArr[top];
	top--;
	b = stackArr[top];
	top--;

	a *= b;

	stackArr[++top] = a;
}

void DIV()
{
	int a, b;
	a = stackArr[top];
	top--;
	b = stackArr[top];
	top--;

	a = b / a;

	stackArr[++top] = a;
}

void GRT()
{
	int a, b;
	a = stackArr[top];
	top--;
	b = stackArr[top];
	top--;

	if (b > a)
		stackArr[++top] = 1;
	else
		stackArr[++top] = 0;
}

void LES()
{
	int a, b;
	a = stackArr[top];
	top--;
	b = stackArr[top];
	top--;

	if (b < a)
		stackArr[++top] = 1;
	else
		stackArr[++top] = 0;

}

void EQU()
{
	int a, b;
	a = stackArr[top];
	top--;
	b = stackArr[top];
	top--;

	if (b == a)
		stackArr[++top] = 1;
	else
		stackArr[++top] = 0;
}

void NEQ()
{
	int a, b;
	a = stackArr[top];
	top--;
	b = stackArr[top];
	top--;

	if (b != a)
		stackArr[++top] = 1;
	else
		stackArr[++top] = 0;
}

void GEQ()
{
	int a, b;
	a = stackArr[top];
	top--;
	b = stackArr[top];
	top--;

	if (b >= a)
		stackArr[++top] = 1;
	else
		stackArr[++top] = 0;
}

void LEQ()
{
	int a, b;
	a = stackArr[top];
	top--;
	b = stackArr[top];
	top--;

	if (b <= a)
		stackArr[++top] = 1;
	else
		stackArr[++top] = 0;

}
