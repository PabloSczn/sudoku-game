/**
 * Author: Pablo Sanchez Narro
 * Matriculation number: 40486559
 *
 * Sudoku Game Implementation
 * Last Date of Modification: 10/04/2022
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#define MAX 81

struct stack
{
  int array[MAX];
  int top;
};

/**
* Global Variables
*/
int cellsFilled;
int rowToUndo[MAX];
int colToUndo[MAX];
int movesRedo[MAX];
int counterMoves = 1;
int gameImported = 0;
int isTimerEnabled = 0;
int exitProgram = 0;
FILE *file; // to save game or open existing game
double delay = 5;
double diff = 0;

// A 2d array to hold the sudoku board
int sudoku[9][9] = { 0 };

/**
* Functions
*/
void initMenu();
void populateBoard(int sudoku[][9], int numToPopulate);
int duplicateInRow(int sudoku[][9], int row);
int duplicateInCol(int sudoku[][9], int col);
int duplicateInBox(int sudoku[][9], int r, int c);
int isBoardValid(int sudoku[][9], int row, int col);
int populateBoardRecursively(int sudoku[][9], int row, int col);
int boardFilled(int sudoku[][9]);
void printBoard(int sudoku[][9]);
void initStack(struct stack *s);
void push(struct stack *s, int item);
int *pop(struct stack *s);
void saveGame(FILE *file, char *nameGame, int sudoku[][9]);
void readGame(FILE *file, char *nameGame, int sudoku[][9]);


/**
* Main Function
*/
int main() 
{
	srand(time(NULL));
	
	//Initialize stack
	struct stack stackToUndoRedo;
	initStack(&stackToUndoRedo);
	int *move = NULL; // to pop from stacks
	
	//Time to calcute differentiate seconds
	time_t start_time;
	time_t current_time;

	time(&start_time);
	time(&current_time);
	
	
	//Menu
	initMenu();
	
	//If the timer is enabled
	if(isTimerEnabled == 1)
	{
		diff = 0;
	}
	
	system("cls");// make the screen pretty by getting rid of whatever there was before	

	//If no game has been imported
	if(gameImported == 0)
	{
		// Populating cells in the board
		populateBoard(sudoku, cellsFilled);		
	}

	//If the board is filled AND the time is not finished yet
	while (!boardFilled(sudoku) && diff < delay) 
	{
		if(isTimerEnabled == 1)
		{
			//Differentiate current time from start time to calculate seconds
			diff = difftime(current_time, start_time);
			printf("\tTime left: %lf seconds\n", (delay - diff));
			printf("\t+++ To update time on screen, select next move +++\n");
			if((delay - diff) <= 0)
			{
				break;
			}
		}
		int row;
		int col;
		int value;
		int choice;

		// Displaying the board
		printf("+++ To play, the 0's are the cells to be filled +++\n");
		printf("Sudoku Board:\n");
		printBoard(sudoku);

		// Decide your choice
		printf("\nSelect:\n-1.Input a value     -2.Undo     -3.Redo     -4.Save current game     -5.Exit\n");
		scanf("%d", &choice);
		if(choice < 1 || choice > 5)
		{
			system("cls");// make the screen pretty by getting rid of whatever there was before
			continue;
		}
		
		if(choice == 1)
		{
			// Getting input from user
			printf("\nSelect your move:\n");
			printf("Input Row (1-9 inclusive):\n");
			scanf("%d", &row);
			//Row is not suitable
			if(row < 1 || row > 9)
			{
				system("cls");// make the screen pretty by getting rid of whatever there was before
				continue;
			}
			
			printf("Input Column (1-9 inclusive):\n");
			scanf("%d", &col);
			//Column is not suitable
			if(col < 1 || col > 9)
			{
				system("cls");// make the screen pretty by getting rid of whatever there was before
				continue;
			}
			
			printf("\nInput the value:\n");
			scanf("%d", &value);
			//Value is not suitable
			if(value < 1 || value > 9)
			{
				system("cls");// make the screen pretty by getting rid of whatever there was before
				continue;
			}
			
			
			// Validating the input
			if (row >= 1 && row <= 9 && col >= 1 && col <= 9) 
			{
				row--;
				col--;
				
				//To reserve state of the cell
				int previousValue = sudoku[row][col];
				if (value >= 0 && value <= 9) 
				{

					// Trying to put the value
					sudoku[row][col] = value;
					if (isBoardValid(sudoku, row, col)) 
					{
						// User's move was placed
						system("cls");// make the screen pretty by getting rid of whatever there was before
						printf("**********Move placed successfully!**********\n\n");
						//Push to the stack
						push(&stackToUndoRedo, value);
						
						//If the user wants to undo, save row and column
						rowToUndo[counterMoves] = row;
						colToUndo[counterMoves] = col;
						counterMoves++;
					}
					else 
					{
						system("cls"); // make the screen pretty by getting rid of whatever there was before
						printf("**********Invalid move! Duplicate found. Board not updated.**********\n\n");
						// Reversing state of the cell
						sudoku[row][col] = previousValue;
					}
				
				}
				else 
				{
					printf("Invalid input! Please enter a valid value\n");
				}
			}
			else 
			{
				printf("Invalid input! Please enter a valid row and column\n");
			}
		}
		else if(choice == 2)		//UNDO Feature
		{
			system("cls");
			//Pop from stack
			move = pop(&stackToUndoRedo);
			if(move)
			{
				counterMoves--;
				sudoku[rowToUndo[counterMoves]][colToUndo[counterMoves]] = 0;
				printf("**********Number %d undid**********\n\n", *move);
				
				//Save number undone if the user wants to redo
				movesRedo[counterMoves] = *move;
			}
		}
		else if(choice == 3)	//REDO Feature
		{
			system("cls");			
			if(movesRedo[counterMoves] != 0) //If there are moves to redo
			{
				push(&stackToUndoRedo, movesRedo[counterMoves]);	//Push onto the stack
				//Set value into the cell
				sudoku[rowToUndo[counterMoves]][colToUndo[counterMoves]] = movesRedo[counterMoves];
				printf("**********Number %d redid**********\n\n", movesRedo[counterMoves]);
				//Recover the move
				counterMoves++;				
			}
			else
			{
				printf("**********No moves to redo**********\n");
			}
			
		}
		else if(choice == 4)	//save game Feature
		{
			
			char gameName[20];
			//Assign the game a name to save it
			printf("\n\nLet's do this, please enter the name of this game:\n");
			scanf("%s", gameName);
			saveGame(file, gameName, sudoku);
			
			system("cls");
			printf("**********Saved game!**********\n");
		}
		else if(choice == 5)	//Exit program
		{			
			exitProgram = 1;
			break;
		}
		//Update current time
		if(isTimerEnabled == 1)
		{
			time(&current_time);
		}
		
	}
	
	if(exitProgram == 1)
	{
		return 1;
	}
	if(diff >= delay)
	{
		system("cls");
		printf("Time's up! You lost :(\n\n");		
	}
	else
	{
		system("cls");
		//When board is filled
		printf("Congratulations! You won!\n\n");	
	}

	return 0;
}


/**
* This function partially populates the board with values
*/
void populateBoard(int sudoku[][9], int numToPopulate) 
{

	// Call to recursive function
	populateBoardRecursively(sudoku, 0, 0);

	// The board is now filled, so we remove enough cells from board
	int i = 0;

	if (numToPopulate > 81) 
	{
		// Making sure that invalid value for this variable is not possible
		numToPopulate = 81;
	}

	while (i < 9 * 9 - numToPopulate) 
	{
		//Place 0 in random places
		int randRow = rand() % 9;
		int randCol = rand() % 9;

		if (sudoku[randRow][randCol] != 0) 
		{
			sudoku[randRow][randCol] = 0;
			i++;
		}

	}

}


/**
* This function uses recursion and backtracking to populate the board
*/
int populateBoardRecursively(int sudoku[][9], int row, int col) 
{

	if (row == 8 && col >= 9) 
	{
		// Board is completely filled
		return 1;
	}

	if (col >= 9) 
	{
		//Column starts from 0 again
		col = 0;
		// The row is increased if we exceed columns
		row++;
	}

	if (sudoku[row][col] == 0) 
	{
		int i = 1;
		while (i <= 9) 
		{

			sudoku[row][col] = i;
			if (isBoardValid(sudoku, row, col)) 
			{

				if (populateBoardRecursively(sudoku, row, col + 1)) 
				{
					return 1;
				}
			}

			sudoku[row][col] = 0;
			i++;
		}
		return 0;
	}
	else 
	{
		// The cell is already populated so we dont need to do anything
		return populateBoardRecursively(sudoku, row, col + 1);
	}
}


/**
* This function checks if the board is valid
*/ 
int isBoardValid(int sudoku[][9], int row, int col) 
{
    // Check the row and column of the current cell
    if (duplicateInRow(sudoku, row) || duplicateInCol(sudoku, col)) {
        return 0;
    }

    // Check the 3x3 box of the current cell
    if (duplicateInBox(sudoku, row - row % 3, col - col % 3)) {
        return 0;
    }

    // All tests cleared so the board is valid
    return 1;
}


/**
* This function checks if there is a duplicate in the
* specified row
*/
int duplicateInRow(int sudoku[][9], int row) 
{

	int rowDigitCounter[9];
	int i = 0;

	for (i = 0; i < 9; i++) 
	{
		rowDigitCounter[i] = 0;
	}

	// Counting occurence of each digit in row
	for (i = 0; i < 9; i++) 
	{
		if (sudoku[row][i] != 0) 
		{
			rowDigitCounter[sudoku[row][i] - 1]++;
		}
	}

	// Now checking if any character appeared more than once
	for (i = 0; i < 9; i++) 
	{
		if (rowDigitCounter[i] > 1) 
		{
			// Found duplicate in row
			return 1;
		}
	}

	// No duplicate was found
	return 0;
}


/**
* This function checks if there is a duplicate in the
* specified column
*/
int duplicateInCol(int sudoku[][9], int col) 
{

	int colDigitCounter[9] = { 0 };
	int i = 0;

	// Counting occurence of each digit in column
	for (i = 0; i < 9; i++) 
	{
		if (sudoku[i][col] != 0) 
		{
			colDigitCounter[sudoku[i][col] - 1]++;
		}
	}

	// Now checking if any character appeared more than once
	for (i = 0; i < 9; i++) 
	{
		if (colDigitCounter[i] > 1) 
		{
			// Found duplicate in column
			return 1;
		}
	}

	// No duplicate was found
	return 0;
}


/**
* This function checks if there is a duplicate in a box in
* sudoku board
*/
int duplicateInBox(int sudoku[][9], int r, int c) 
{

	int boxDigitCounter[9] = { 0 };

	int i = 0;
	int j = 0;

	// Counting occurence of each digit in box
	for (i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			if (sudoku[i + r][j + c] != 0) 
			{
				boxDigitCounter[sudoku[i + r][j + c] - 1]++;
			}
		}

	}

	// Now checking if any character appeared more than once
	for (i = 0; i < 9; i++) 
	{
		if (boxDigitCounter[i] > 1) 
		{
			// Found duplicate in column
			return 1;
		}
	}

	return 0;
}


/**
* This function checks if the whole board is filled
*/
int boardFilled(int sudoku[][9]) 
{
	int i;
	int j;
	for (i = 0; i < 9; i++) 
	{
		for (j = 0; j < 9; j++) 
		{
			if (sudoku[i][j] == 0) 
			{
				// Found a non filled cell
				return 0;
			}
		}
	}
	return 1;
}


/**
* This function prints the board on console
*/
void printBoard(int sudoku[][9]) 
{
	int i;
	int j;
	
	printf(" \t");
	for (i = 0; i < 9; i++) 
	{
		if(i == 0)
		{
			printf("|C%d  ", (i+1));
		}
		else if((i+1) == 3 || (i+1) == 6 || (i+1) == 9)
		{
			printf(" C%d |", (i+1));			
		}
		else
		{
			printf(" C%d  ", (i+1));
		}
	}
	
	printf("\n----------------------------------------------------\n");
	for (i = 0; i < 9; i++) 
	{
		printf("R%d\t", (i+1));
		
		for (j = 0; j < 9; j++) 
		{
			if(j == 0)
			{
				printf("| %d  ", sudoku[i][j]);
			}
			else if((j+1) == 3 || (j+1) == 6 || (j+1) == 9)
			{
				printf("  %d |", sudoku[i][j]);	
			}
			else
			{
				printf("  %d  ", sudoku[i][j]);
			}
		}
		
		if((i+1) == 3 || (i+1) == 6 || (i+1) == 9)
		{
			printf("\n----------------------------------------------------\n");
		}
		else
		{
			printf("\n\n");
		}
	}
}


/**
* This function initialize the menu
*/
void initMenu()
{
	//Decision
	int option = -1;
	
	system("cls");// make the screen pretty by getting rid of whatever there was before
	printf("\n\n ***********************\n *       SUDOKU        *\n *                     *\n *      by Pablo       *\n ***********************\n");
	printf("\nChoose your mode of playing:\n\n- 1 Easy mode\n- 2 Medium mode\n- 3 Hard mode\n\n- 4 Play against time\n- 5 Import game\n\nType 1, 2, 3, 4 or 5:\n\n");
	
	while(option != 1 && option != 2 && option != 3 && option != 4 && option != 5)
	{
		scanf("%d", &option);
		
		//Easy mode fills 72 cells
		if(option == 1)
		{
			cellsFilled = 72;
		}	
		//Medium mode fills 64 cells
		else if(option == 2)
		{
			cellsFilled = 60;
		}
		//Hard mode fills 56 cells
		else if(option == 3)
		{
			cellsFilled = 50;
		}
		//Play against time feature
		else if(option == 4)
		{
			cellsFilled = 65;
			//Timer enabled
			isTimerEnabled = 1;
			double minutes = 0;
			//Assign the game a name to save it
			printf("\n\nLet's do this, How many minutes you want?\n");
			scanf("%lf", &minutes);
			//Convert minutes to second
			delay = minutes * 60;
		}
		//Import game
		else if(option == 5)
		{
			char gameToImport[20];
			//Assign the game a name to save it
			printf("\n\nLet's do this, please enter the name of the game you want to import:\n");
			//Scan the game to play
			scanf("%s", gameToImport);
			readGame(file, gameToImport, sudoku);
			gameImported = 1;
		}
	}
	
}


/**
* Accessing the variable that points to the top of the Stack
* setting it to -1 to indicate that the stack is empty
*/
void initStack(struct stack *s)
{
  s->top = -1;
}


/**
* Checks the position of the variable that indicates the top of the stack 
*/ 
void push(struct stack *s, int item)
{
  // If top = max size, taking into account zero indexing
  if(s->top == MAX-1)
  {
    // Then stack is full
    printf("\nStack overflow!!\n");
    return;
  }
  // Otherwise we alter where the variable pointing to top points to
  s->top++;
  // then place the item into the stack
  s->array[s->top] = item;
}


/**
* Removes and return top item of stack
*/ 
int *pop(struct stack *s)
{
  // temprary storage for item to pop
  int *data;

  // check position of top indicator variable
  // if it points to the bottom of the array...
  if (s->top == -1)
  {
    // stack is empty and there is nothing to pop
    printf("There are no moves to undo.\n");
    // if pop() == NULL then stack is empty
    return NULL;
  }
  // access the value stored in the current location
  data = &s->array[s->top];
  // alter top pointer to point to the item below the current item
  // so that next time pop is called it will retrieve the next item
  s->top--;

  return data;
}


/**
* Save state of game to file
*/ 
void saveGame(FILE *file, char *nameGame, int sudoku[][9])
{
	file = fopen("SudokuGames.txt", "a");
	//Name of the game
	fprintf(file, "\n\n%s\n", nameGame);           

	for(int i = 0; i < 9; i++)
	{
		for ( int j = 0; j < 9; j++)
		{
			fprintf(file, "%d", sudoku[i][j]);
		}
		
	}

	fclose(file);
}


/**
* Read game from file
*/ 
void readGame(FILE *file, char *nameGame, int sudoku[][9])
{
	file = fopen("SudokuGames.txt", "r");
	
	//If fails to read the file
	if (file == NULL) 
	{
		system("cls");
        printf("***No existing games!!***\n");
		return;
    }	
	
	
	char line[100];
	int nameFound = 0;
	//Fgets go line by line
	while (fgets (line , 100, file) != NULL)
	{
		//If the game exists
		if(strstr(line, nameGame) != NULL)
		{
			//The name has been found
			nameFound = 1;
		}
		else if(nameFound == 1)	//Changes the sudoku board to the saved one
		{
			int column = 0;
			int rows = 0;
			//For every character in the line
			for(int i = 0; i < strlen(line); i++)
			{
				if(column < 8)
				{
					sudoku[rows][column] = (line[i] - '0');
					column++;
				}
				else	//Last column
				{
					sudoku[rows][column] = (line[i] - '0');
					column = 0;
					rows++;
				}
				
			}
			nameFound = 0;
		}
	}	
	fclose(file);
}
