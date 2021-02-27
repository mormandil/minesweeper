#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define BOARDSIZE 22

//this function prints the menue and get the user chois about the size of the board, if the the user picked option 4 - costomize board, gets his sixr for rows and cols.
//the function returns by refrence the sizes of the board.
void menu(int * rows, int * cols);

//this function gets the board game and prints it in format.
void printBoard(char board[BOARDSIZE][BOARDSIZE], int numRows, int numCols);

void printLine(int numCols);

/*this function sets the game board- the data game board which holds the mines locations, the numbers indicate on mine neighbors and open cells.
the function randomly sets the mines to board.
she makes an array of the mines location and due to that uses setNeighborsValue function to number their neighbors (according to amount of mines serrounding them).
she is also replacing every empty cell in space instead of zero, for future cells use and failure board print.
*/
void setBoard(char board[BOARDSIZE][BOARDSIZE], int numRows, int numCols);

//this function gets the randomly grilled mine absolute location(number of cell on the board) and returns by refrence to the setBoard function his relative location on the board.
void placeMine(int absoluteLocation, int numRows, int numCols, int * mineRow, int * mineCol);

//calaculate how many mines should be, according to board size.
int minesAmount(int rows, int cols);

//calculate how many cell there is in the board.
int boardCellsAmount(int rows, int cols);

// this function gets a mine location and according to that numbers his neighbors.
void setNeighborsValue(int row, int col, char board[BOARDSIZE][BOARDSIZE], int numRows, int numCols);

//this function gets a board (a two dimensional array of chars) and a char, and puts the char in every call of the board.
void setAllBoardToSymbol(char board[BOARDSIZE][BOARDSIZE], int numRows, int numCols, char ch);

//this function gets a board location (a row and a col) and returns whether it is inside of board limits.
bool isInBoard(int row, int col, int numRows, int numCols);

/*this functions gets the user play in his turn and due to thet respons-
the user can put a flag on a cell or to open it.
if the user input (-1 x) it opens the first x cells (which are not mines or already open).
if the user landed on a mine the game is over.
if the user input is valid (in borders of board and isn't open yet) she open the chosen cell.
if its an empty cells all empty cells around her open aotumatically.
the function return true\false if the user had won\lost.
*/
bool theGame(char userBoard[BOARDSIZE][BOARDSIZE], char dataBoard[BOARDSIZE][BOARDSIZE], int numRows, int numCols);

//this function gets the user input - a board cell (a row and a col) , and returns if its valid or not. (in borders of board and isn't open yet)
bool isValidInput(int row, int col, char userBoard[BOARDSIZE][BOARDSIZE], int numRows, int numCols, char symbol);

//a recursive function thar gets an empty cell and 'aotumatically' opens all empty cell in it serrounding.
void openAllClearCells(char data[BOARDSIZE][BOARDSIZE], char user[BOARDSIZE][BOARDSIZE], int numRows, int numCols, int currentRow, int currentCol, int *totalOpened);

//returns if a cgiven char is a number.
bool isItANumber(char c);

void main() {

	bool winTheGame = false;
	int numRows, numCols;

	menu(&numRows, &numCols);
	
	char userBoard[BOARDSIZE][BOARDSIZE];
	setAllBoardToSymbol(userBoard, numRows, numCols, 'X');	

	char dataBoard[BOARDSIZE][BOARDSIZE];
	setAllBoardToSymbol(dataBoard, numRows, numCols,'0');	
	setBoard(dataBoard, numRows, numCols);		
	
	winTheGame = theGame(userBoard, dataBoard, numRows,numCols);
	if (winTheGame)
	{
		printf("You won!! congragulations! CHAMPION!");
	}
	else
	{
		printf("You've hit a bomb! Game over!\n");
		printBoard(dataBoard,numRows,numCols);
	}
}

void setAllBoardToSymbol(char board[BOARDSIZE][BOARDSIZE],int numRows, int numCols, char ch)
{
	int row, col;

	for (row = 0; row < numRows; row++)
	{
		for (col = 0; col < numCols; col++)
		{
			board[row][col] = ch;
		}
	}
}

int minesAmount(int rows, int cols)
{
	int numOfMInes;
	numOfMInes = sqrt(rows*cols);	
	return numOfMInes;
}

int boardCellsAmount(int rows, int cols)
{
	int cellsAmount = (rows * cols) ;	
	return cellsAmount;
}

void setBoard(char board[BOARDSIZE][BOARDSIZE],int numRows,int numCols)
{
	int mineAbsolutLocation, mineRow, mineCol;
	int currentMineIndex;
	int numOfMines = minesAmount(numRows, numCols);
	int cellsAmount = boardCellsAmount(numRows, numCols);
	int minesLocationsArray[2][23]; //an array contain mines rellative locations, first row of array - mine row number, second row of array - mine col number (for each one of the mines).
	int minesAbsoluteLocationsArray[BOARDSIZE]; // an array contains all the absolute mine locations.
	int currentMineAbsoluteLocationIndex;
	bool needAssigment = false;	
	int cellRow, cellCol;

	srand(time(NULL));	

	//set the mines randomly to board.
	for (currentMineIndex = 0; currentMineIndex < numOfMines; currentMineIndex++)
	{
		needAssigment = true;
		while (needAssigment)
		{
			mineAbsolutLocation = rand() % (cellsAmount); // grill location according to board num of cells
			minesAbsoluteLocationsArray[currentMineIndex] = mineAbsolutLocation;			

			for (currentMineAbsoluteLocationIndex = 0; currentMineAbsoluteLocationIndex < currentMineIndex; currentMineAbsoluteLocationIndex++)
			{
				if (mineAbsolutLocation == minesAbsoluteLocationsArray[currentMineAbsoluteLocationIndex]) //if a mine already exists in this absolute location, grill a new one.
				{
					break;
				}
			}
			if (currentMineAbsoluteLocationIndex == currentMineIndex) 
			{
				needAssigment = false;
			}
		}

		placeMine(mineAbsolutLocation, numRows, numCols, &mineRow, &mineCol);

		board[mineRow][mineCol] = '*';

		//enter mines relative location to mines Locations Array
		minesLocationsArray[0][currentMineIndex] = mineRow;
		minesLocationsArray[1][currentMineIndex] = mineCol;		
	}
	
	//set mines Neighbors Values
	for (currentMineIndex = 0; currentMineIndex < numOfMines; currentMineIndex++)
	{
		setNeighborsValue(minesLocationsArray[0][currentMineIndex], minesLocationsArray[1][currentMineIndex], board, numRows, numCols);
	}	

	// sets all '0' on board to ' ' (this is needed in the sequence).
	for (cellRow = 0; cellRow <= numRows; cellRow++)
	{
		for (cellCol = 0; cellCol <= numCols; cellCol++)
		{
			if (board[cellRow][cellCol] == '0')
			{
				board[cellRow][cellCol] = ' ';
			}
		}
	}	
}

void placeMine(int absoluteLocation, int numRows, int numCols, int * mineRow, int * mineCol)
{
	if (numRows == numCols)
	{
		*mineRow = absoluteLocation / numRows;
		*mineCol = absoluteLocation % numCols;
	}
	else	
	{
		if (absoluteLocation % numCols == 0)
		{
			*mineRow = (absoluteLocation - 1) / numCols;
		}
		else 
		{
			*mineRow = absoluteLocation / numCols;
		}
		
		*mineCol = absoluteLocation - (numCols*(*mineRow)) - 1;
	}
}

void setNeighborsValue(int row, int col, char board[BOARDSIZE][BOARDSIZE], int numRows, int numCols)
{
	int currentRow, currentCol;

	for (currentRow = row - 1; currentRow <= (row + 1); currentRow++) {

		for (currentCol = col - 1; currentCol <= (col + 1); currentCol++)
		{
			if (isInBoard(currentRow, currentCol, numRows, numCols))
			{
				if (board[currentRow][currentCol] != '*')
				{
					board[currentRow][currentCol] = board[currentRow][currentCol] + 1;
				}
			}
		}
	}
}

bool isInBoard(int row, int col, int numRows, int numCols)
{
	return((row >= 0 && row <= numRows-1) && (col >= 0 && col <= numCols-1));
}

void menu(int * rows, int * cols)
{
	int userChoise;
	bool validChoiseWasMade = false;

	while (!validChoiseWasMade)
	{
		printf("Welcome to Minesweeper!\nPlease choose one of the following options and enter it's number:\n\n1 - for size 8X8\n\n2 - for size 12X12\n\n3 - for size 15X15\n\n4 - for custom size\n\n0 - Exit\n\n");
		scanf("%d", &userChoise);

		if (userChoise >= 0 && userChoise <= 4)
		{
			if (userChoise == 1)
			{
				*rows = 8;
				*cols = 8;
			}
			else if (userChoise == 2)
			{
				*rows = 12;
				*cols = 12;
			}
			else if (userChoise == 3)
			{
				*rows = 15;
				*cols = 15;
			}
			else if (userChoise == 4)
			{
				printf("Please enter the size of the board(Lines <= 22 and Cols <= 22) :");
				scanf("%d %d", &*rows, &*cols);
			}
			else if (userChoise == 0)
			{
				exit(0);
			}

			validChoiseWasMade = true;
		}
	}
}

bool theGame(char userBoard[BOARDSIZE][BOARDSIZE], char dataBoard[BOARDSIZE][BOARDSIZE],int numRows, int numCols)
{
	int inputRow, inputCol, movesCounter = 0, cellsToOpen, currentCell, totalOpened = 0;
	int cellsAmount = boardCellsAmount(numRows, numCols);
	int numOfMines = minesAmount(numRows, numCols);
	bool win = false;
	char inputChar;

	while (!win)
	{
		printBoard(userBoard, numRows,numCols);
		printf("Please enter your move, row and column :\n");
		scanf("%d %d",&inputRow, &inputCol);	

		
		
		if (inputRow == -1) //if row input is -1 open the colinput amount of first unopend cells (unless they are mines).
		{
			int opened = 0;
			cellsToOpen = inputCol;

			for (int row = 0; row < numRows && opened <= cellsToOpen ; row++)
			{
				for (int col = 0; col < numCols && opened <= cellsToOpen; col++)
				{
					if (userBoard[row][col] == 'X') // hasnt been opened or flaged yet
					{
						if (dataBoard[row][col] != '*') // isn't a mine
						{
							userBoard[row][col] = dataBoard[row][col];
							opened++;
							totalOpened++;
						}
					}

				}
			}

		}
		else
		{
			scanf(" %c", &inputChar);

			if (!isValidInput(inputRow, inputCol, userBoard, numRows, numCols, inputChar)) // if the input is not valid (out of board or alreadey was chosen)
			{
				printf("Invalid move, please enter valid choice!\n");
			}
			else //input is valid.
			{
				if (inputChar == 'F' || inputChar == 'f') // if char is F, flag the cell
				{
					userBoard[inputRow][inputCol] = 'F';
				}
				else if (inputChar == 'O' || inputChar == 'o') //if char is O open the cell
				{
					if (dataBoard[inputRow][inputCol] == '*') // if its a mine - game over. 
					{
						break;
					}
					else
					{
						if (dataBoard[inputRow][inputCol] == ' ')
						{							
							openAllClearCells(dataBoard, userBoard, numRows, numCols, inputRow, inputCol,&totalOpened);							
						}
						else
						{
							userBoard[inputRow][inputCol] = dataBoard[inputRow][inputCol]; //user board gets cell from data board.				
							totalOpened++;
						}						
					}
				}			

		    }			
		}

		if (totalOpened >= (cellsAmount - numOfMines)) // win 
		{
			win = true;
		}
	}

	return win;
}

bool isItANumber(char c) {
	if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c== '7' || c=='8')
	{
		return true;
	}
	return false;
}

void openAllClearCells(char data[BOARDSIZE][BOARDSIZE], char user[BOARDSIZE][BOARDSIZE], int numRows, int numCols, int currentRow, int currentCol,int * totalOpened)
{
	
	if (user[currentRow][currentCol] == ' ' || isItANumber(user[currentRow][currentCol])) {		
		return;
	}
	
	if ((user[currentRow][currentCol] == 'X' || user[currentRow][currentCol] == 'F') && data[currentRow][currentCol] != '*') {
		user[currentRow][currentCol] = data[currentRow][currentCol]; //open current cell
		*totalOpened = *totalOpened + 1;
	}

	if (data[currentRow][currentCol] != ' ') //if its a number open end return;
	{		
		return;
	}	
	
	if (isInBoard(currentRow - 1, currentCol, numRows, numCols))
	{
		openAllClearCells(data, user, numRows, numCols, currentRow - 1, currentCol, totalOpened);
	}
	if (isInBoard(currentRow - 1, currentCol-1, numRows, numCols))
	{
		openAllClearCells(data, user, numRows, numCols, currentRow - 1, currentCol-1, totalOpened);
	}
	if (isInBoard(currentRow, currentCol - 1, numRows, numCols))
	{
		openAllClearCells(data, user, numRows, numCols, currentRow, currentCol - 1, totalOpened);
	}
	if (isInBoard(currentRow + 1, currentCol - 1, numRows, numCols))
	{
		openAllClearCells(data, user, numRows, numCols, currentRow + 1, currentCol - 1, totalOpened);
	}
	if (isInBoard(currentRow + 1, currentCol, numRows, numCols))
	{
		openAllClearCells(data, user, numRows, numCols, currentRow + 1, currentCol, totalOpened);
	}
	if (isInBoard(currentRow + 1, currentCol+1, numRows, numCols))
	{
		openAllClearCells(data, user, numRows, numCols, currentRow + 1, currentCol+1, totalOpened);
	}
	if (isInBoard(currentRow , currentCol+1, numRows, numCols))
	{
		openAllClearCells(data, user, numRows, numCols, currentRow, currentCol+1, totalOpened);
	}
	if (isInBoard(currentRow - 1, currentCol+1, numRows, numCols))
	{
		openAllClearCells(data, user, numRows, numCols, currentRow - 1, currentCol+1, totalOpened);
	}
	
}

bool isValidInput(int row, int col, char userBoard[BOARDSIZE][BOARDSIZE], int numRows, int numCols,char symbol)
{
	if (symbol == 'O'|| symbol == 'o' || symbol == 'F' || symbol == 'f')
	{
		if (!isInBoard(row, col, numRows, numCols)) //if users input is out of board its not valid choise.
		{	
			return false;
		}
		if (userBoard[row][col] != 'X' && userBoard[row][col] != 'F') //if this cell has already been revelid its not valid choise.
		{			
			return false;
		}
	}
	else
	{		
		return false;
	}
	
	return true;
}

void printBoard(char board[BOARDSIZE][BOARDSIZE], int numRows, int numCols)
{
	
	printf("   ");
	for (int index = 0; index < numCols; index++)
	{
		printf(" %2d ", index);
	}printf("\n");
	printLine(numCols);

	int i, j;

	for (i = 0; i < numRows; i++)
	{
		printf("%2d |", i);
		for (j = 0; j < numCols; j++)
		{
			printf("%2c |", board[i][j]);
		}
		printf("\n");
	}

	printLine(numCols);

}

void printLine(int numCols)
{
	for (int i = 0; i <= (4*numCols +3); i++)
	{
		printf("_");
	}
	printf("\n");
}