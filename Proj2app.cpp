// Assignment : Project 2
// File: Proj2app.cpp
// Author : Chris Tran
// Date : Spring 2017
// Description : The application file that contains the main function for Project 2.
#include <iostream>
#include <fstream>
#include "SetLimits.h"
#include "CLife.h"
using namespace std;

int main ()
{
  char esc = 0x1B;
    SetLimits();
    char play = 'y';
    CLife life;
    life.Instructions (cout, cin);
    while (tolower (play) == 'y')
    {
	char key;
	char row, col;
	string filename;
	ifstream input;
	life.Init(15, 21);
	cout << esc << "[2;30;47m" << "An empty community has been created!" << esc << "[0m" << endl;
	life.Display (cout);
	cout << endl << esc << "[2;30;47m" << "How would you like to populate your community?" << esc << "[0m" << endl << endl;
	cout << esc << "[2;30;47m" << "Enter R/r for a randomly generated initial population." << esc << "[0m" << endl;
	cout << esc << "[2;30;47m" << "Enter U/u to establish your own initial population." << esc << "[0m" << endl;
	cout << esc << "[2;30;47m" << "Enter F/f to read the initial population from a file." << esc << "[0m" << endl << endl;
	cin >> key;
	switch (tolower (key))
	{
		case 'r' :
			life.InitRandom ();
			break;
		case 'u' :
			do
			{
			  cout << endl << esc << "[2;30;47m" << "Enter row and column letters (ZZ to end):" << esc << "[0m" << ' ';
				cin >> row >> col;
				if (life.Add (toupper(row) - 'A' + 1, toupper(col) - 'A' + 1))
				  {
				    life.Display (cout);
				  }
			} while (toupper(row) != 'Z' && toupper(col) != 'Z');
			break;
		case 'f' :
		  cout << esc << "[2;30;47m" << "Enter the name of the file:" << esc << "[0m" << ' ';
			cin >> filename;
			input.open (filename.c_str());
			if (input.fail())
			{
				life.InitRandom ();
			}
			else
			{
				life.InitFile (input);
				input.close();
			}
			break;
		default :
		  cout << esc << "[2;30;47m" << "Not a valid selection" << esc << "[0m" << endl;
			life.InitRandom ();
	}
	life.Display (cout);
	bool finished = false, quitting = false;
	while (!finished && !quitting)
	{
		int row, col;
		cout << endl << esc << "[2;30;47m" << "Enter option (N/n = next, Q/q = quit, I/i = instructions):" << esc << "[0m" << ' ';
		cin >> key;
		switch (tolower (key))
		{
			case 'n' :
				life.Next ();
				break;
			case 'q' :
				quitting = true;
				break;
			case 'i':
				life.Instructions (cout, cin);
				break;
			default :
			  cout << esc << "[2;30;47m" << "Not a valid selection" << esc << "[0m" << endl;
		}
		life.Display (cout);
		finished = life.Done();
	}
	if (quitting == true)
	  {
	    cout << endl << esc << "[2;30;47m" << "Thank you for playing!" << esc << "[0m" << endl;
	    cout << esc << "[2;30;47m" << "The player has quit." << esc << "[0m" << endl << endl;
	  }
	else
	  life.Message (cout);
	cout << esc << "[2;30;47m" << "Would you like to play again (y/n)? " << esc << "[0m";
	cin >> play;
	cin.clear ();;
    }
    cout << endl;
    return 0;
}
