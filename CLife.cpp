// Assignment : Project 2
// File: CLife.cpp
// Author : Chris Tran
// Date : Spring 2017
// Description : This program will run the ASCII version of the game of life with a seed generation that is either
// randomized, inputted by the user, or read in from a file.

#include <ctime>
#include <cstdlib>
#include "CLife.h"

CLife::CLifeSquare::CLifeSquare ()
{
  // This function will: set all of the cells in the grid to be border cells.

  what = BORDER;
}

void CLife::CLifeSquare::Display (ostream & outs)
{
  // This function will: display a single cell.
  char esc = 0x1B;
  string dispString;
  // Decide what characters will represent empty, adding, person, deleting, and border cells
  switch (what)
    {
    case EMPTY:
      dispString = " ";
      break;
    case ADDING:
      dispString = "*";
      break;
    case PERSON:
      dispString = "X";
      break;
    case DELETING:
      dispString = "#";
      break;
    default:
      dispString = "B";
    }
  // First vertical bar before the cell and a space after the cell
  outs << esc << "[2;36;40m" << "|" << esc << "[0m" << ' ' << esc << "[2;32;40m" << dispString << esc << "[0m" << ' ';
}

int CLife::CLifeSquare::Reset ()
{
  // This function will: set adding to person and set deleting to empty.
  int PopChange = 0;
  if (what == ADDING)
    {
      what = PERSON;
      PopChange++;
    }
  if (what == DELETING)
    {
      what = EMPTY;
      PopChange--;
    }
  return PopChange;
}

CLife::CLife ()
{
  // This function will: construct the default playing grid.

  numRows = 1;
  numCols = 1;
  grid = new CLifeSquare * [numRows+2];
  for (int r = 0; r < numRows+2; r++)
    grid[r] = new CLifeSquare [numCols+2];
  //grid[1][1].what = EMPTY;
  evaluating = true;
  finished = 0;
  populationSize = 0;
  generations = 0;
}

CLife::~CLife ()
{
  // This function will: deconstruct the playing grid.

  for (int r = 0; r < numRows+2; r++)
    delete [] grid[r];
  delete [] grid;
}

void CLife::Init (int R, int C)
{
  // This function will: initialize the full playing grid.

  for (int r = 0; r < numRows+2; r++)
    delete [] grid[r];
  delete [] grid;
  generationsMap.clear();
  numRows = R;
  numCols = C;
  grid = new CLifeSquare * [numRows+2];
  for (int r = 0; r < numRows+2; r++)
    grid[r] = new CLifeSquare [numCols+2];
  for (int r = 1; r <= numRows; r++)
    for (int c = 1; c <= numCols; c++)
      grid[r][c].what = EMPTY;
  finished = 0;
  evaluating = true;
  populationSize = 0;
  generations = 1;
}

void CLife::Instructions (ostream & outs, istream & ins)
{
  // This function will: display the title of the game and the instructions.
  char esc = 0x1B;
  outs << endl;
  outs << esc << "[2;30;47m" << "Game of Life\n\n" << esc << "[0m";
  outs << esc << "[2;30;47m" "This version of Game of Life will allow a user to view a seed pattern as it progresses generation from generation.\nThe user can randomize the initial population, read it in from a file, or input their own seed pattern.\n" << esc << "[0m";
  outs << endl;
}

void CLife::Display (ostream & outs)
{
  // This function will: display the playing grid.
  char esc = 0x1B;
  string Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  outs << endl << "    ";
  for (int c = 1; c <= numCols; c++)
    {
      outs << esc << "[1;31;40m" << Alphabet[c-1] << esc << "[0m" << "   ";
    }
  outs << endl;
  for (int r = 1; r <= numRows; r++)
    {
      outs << "  " << esc << "[2;36;40m" << "+" << esc << "[0m";
      for (int c = 1; c <= numCols; c++)
	{
	  outs << esc << "[2;36;40m" << "---+" << esc << "[0m";
	}
      outs << endl;
      outs << esc << "[1;31;40m" << Alphabet[r-1] << esc << "[0m" << ' ';
      for (int c = 1; c <= numCols; c++)
	{
	  grid[r][c].Display (outs);
	}
      outs << esc << "[2;36;40m" << '|' << esc << "[0m" << ' ';
      outs << esc << "[1;31;40m" << Alphabet[r-1] << esc << "[0m" << endl;
    }
  outs << "  " << esc << "[2;36;40m" << "+" << esc << "[0m";
  for (int c = 1; c <=numCols; c++)
    {
      outs << esc << "[2;36;40m" << "---+" << esc << "[0m";
    }
  outs << endl << "    ";
  for (int c = 1; c <= numCols; c++)
    {
      outs << esc << "[1;31;40m" << Alphabet[c-1] << esc << "[0m" << "   ";
    }
  outs << endl << endl << esc << "[2;30;47m" << "Population: " << populationSize << esc << "[0m" << "   " << esc << "[2;30;47m" << "Generation: " << generations << esc << "[0m" << endl;
}

void CLife::InitRandom ()
{
  // This function will: initialize a new game with a randomized seed.
  srand(time(NULL));
  for (int n = 0; n < 30; n++)
    {
      int r = rand() % numRows + 1;
      int c = rand() % numCols + 1;
      if (Add(r,c))
	populationSize+= 0;
    }
}

void CLife::InitFile (istream & ins)
{
  // This function will: initialize a playing grid based on a read-in file.  
  int rows;
  int cols;
  ins >> rows >> cols;
  
  Init(rows,cols);
  
  char filetype;
  ins >> filetype;
  if (filetype == 'G')
    {
      string line;
      for (int r = 1; r <= rows; r++)
	{
	  ins >> line;
	  for (int c = 1; c < line.length(); c++)
	    {
	      if (line[c - 1] == 'X')
		{
		  grid[r][c].what = PERSON;
		  populationSize++;
		}
	    }
	}
    }
  else if (filetype == 'P')
    {
    while (ins >> rows >> cols)
      {
	grid[rows][cols].what = PERSON;
	populationSize++;
      }
    }
  else
    InitRandom();
    
}

void CLife::Next ()
{
  // This function will: progress the game from one generation to the next generation.

  if (evaluating)
    for (int r = 1; r <= numRows; r++)
      for (int c = 1; c <= numCols; c++)
	Evaluate (r, c);
  else
    {
      for (int r = 1; r <= numRows; r++)
	for (int c = 1; c <= numCols; c++)
	  populationSize += grid[r][c].Reset ();
      string mapStr = MakeString ();
      map<string, int>::iterator itr = generationsMap.find (mapStr);
      if (itr == generationsMap.end())
	{
	  generations++;
	  generationsMap [mapStr] = generations;
	}
      else
	{
	  finished = itr->second;
	}
    }
  if (populationSize == 0)
    finished = -2;
  evaluating = !evaluating;
}

bool CLife::Add (int row, int col)
{
  // This function will: change an empty cell to a person cell and return true. If a person cell is already present
  // it will return false.
  if (row < 1)
    return false;
  if (row > numRows)
    return false;
  if (col < 1)
    return false;
  if (col > numCols)
    return false;
  if (grid[row][col].what == PERSON)
    return false;
  if (grid[row][col].what == EMPTY)
    {
      grid[row][col].what = PERSON;
      populationSize++;
      return true;
    }
  return false;
}

bool CLife::Delete (int row, int col)
{
  // This function will: change a person cell to an empty cell and return true. If an empty cell is already present
  // it will return false.

  if (grid[row][col].what == PERSON)
    {
      grid[row][col].what = EMPTY;
      return true;
    }
  return false;
}

void CLife::Message (ostream & outs)
{
  // This function will: print a string upon quitting the game, when the population dies out, or when the
  // population reaches a stable state.
  char esc = 0x1B;
  if (populationSize == 0)
    {
      outs << endl << esc << "[2;30;47m" << "Thank you for playing!" << esc << "[0m"  << endl;
      outs << esc << "[2;30;47m" << "The population has died out." << esc << "[0m" << endl << endl;
    }
  else if (populationSize != 0)
    {
      outs << endl << esc << "[2;30;47m" << "Thank you for playing!" << esc << "[0m" << endl;
      outs << esc << "[2;30;47m" << "The population has reached a stable state." << esc << "[0m" << endl << endl;
    }  
}

bool CLife::Done () 
{
  // This function will: end the game.

  return finished != 0;
}

void CLife::Evaluate (int row, int col)
{
  // This function will: look at each individual CLifeSquare and decide if the population lives or dies.
  int neighbors = 0;
  if (grid[row-1][col-1].what == PERSON || grid[row-1][col-1].what == DELETING)
    neighbors++;
  if (grid[row-1][col].what == PERSON || grid[row-1][col].what == DELETING)
    neighbors++;
  if (grid[row-1][col+1].what == PERSON || grid[row-1][col+1].what == DELETING)
    neighbors++;
  if (grid[row][col-1].what == PERSON || grid[row][col-1].what == DELETING)
    neighbors++;
  if (grid[row][col+1].what == PERSON || grid[row][col+1].what == DELETING)
    neighbors++;
  if (grid[row+1][col-1].what == PERSON || grid[row+1][col-1].what == DELETING)
    neighbors++;
  if (grid[row+1][col].what == PERSON || grid[row+1][col].what == DELETING)
    neighbors++;
  if (grid[row+1][col+1].what == PERSON || grid[row+1][col+1].what == DELETING)
    neighbors++;

  if (grid[row][col].what == PERSON && neighbors < 2)
    grid[row][col].what = DELETING;
  if (grid[row][col].what == PERSON && neighbors == 2)
    grid[row][col].what = PERSON;
  if (grid[row][col].what == PERSON && neighbors == 3)
    grid[row][col].what = PERSON;
  if (grid[row][col].what == PERSON && neighbors > 3)
    grid[row][col].what = DELETING;
  if (grid[row][col].what == EMPTY && neighbors == 3)
    grid[row][col].what = ADDING;
}

string CLife::MakeString () const
{
  // This function will: return a string that will be placed into a map in order to compare generations to
  // one another and determine if they are repeating.
  string MapStr = "";
  for (int r = 1; r <= numRows; r++)
    {
      for (int c = 1; c <= numCols; c++)
	{
	  if (grid[r][c].what == EMPTY)
	    MapStr+= '.';
	  else if (grid[r][c].what == PERSON)
	    MapStr+= 'X';
	}
    }
  return MapStr;
}
