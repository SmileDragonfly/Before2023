#include "Matrix.h"

/* Function */

Matrix MoveMatrix(MatrixSize SizeInput, Matrix MatInput, uint8_t Horizontal, uint8_t Vertical)
{
	uint8_t CountRow;
	uint8_t CountColumn;
	Matrix MatOutput;
	MatOutput.row = SizeInput.row;
	MatOutput.column = SizeInput.column;
	/* Assign all elements in matrix are zero */
	InitMatrix(&MatOutput);

	if ((SizeInput.row<(MatInput.row+Vertical))||(SizeInput.column<(MatInput.column+Horizontal)))
	{
		/* Output Size is smaller than MatInput Size*/
		printf("Wrong Syntax \n");
		return MatOutput;
	}
	else
	{	
		/* Move MatInput */
		for(CountRow = 0; CountRow < MatInput.row; CountRow++)
		{
			for (CountColumn = 0; CountColumn < MatInput.column; CountColumn++)
			{
				/* Move */
				MatOutput.Mat[Vertical+CountRow][Horizontal+CountColumn] = MatInput.Mat[CountRow][CountColumn];
			}
		}
		return MatOutput;
	}
}

Matrix AddMatrix(Matrix MatInput1,Matrix MatInput2)
{
	Matrix MatOutput;
	uint8_t CountRow;
	uint8_t CountColumn;

	if((MatInput1.row = MatInput2.row)&&(MatInput1.column = MatInput2.column))
	{
		MatOutput.row = MatInput1.row;
		MatOutput.column = MatInput1.column;
		for(CountRow = 0; CountRow < MatInput1.row; CountRow++)
		{
			for (CountColumn = 0; CountColumn < MatInput1.column; CountColumn++)
			{
				/* Move */
				MatOutput.Mat[CountRow][CountColumn] = MatInput1.Mat[CountRow][CountColumn] + MatInput2.Mat[CountRow][CountColumn];
			}
		}
		return MatOutput;
	}
	else
	{
		printf("Wrong Syntax \n");
	}
}


void InitMatrix(Matrix* MatInput)
{
	uint8_t CountRow;
	uint8_t CountColumn;
	for(CountRow = 0; CountRow < MatInput->row; CountRow++)
	{
		for (CountColumn = 0; CountColumn < MatInput->column; CountColumn++)
		{
			/* Move */
			MatInput->Mat[CountRow][CountColumn] = 0;
		}
	}
}


void GetMatrixElements(Matrix* MatInput)
{
	uint8_t CountRow;
	uint8_t CountColumn;
	printf("Typing row value: ");
	scanf("%d",&MatInput->row);
	printf("Typing row value: ");
	scanf("%d",&MatInput->column);
	printf("Typing values in Matrix:\n");
	for(CountRow = 0; CountRow < MatInput->row; CountRow++)
	{
		for (CountColumn = 0; CountColumn < MatInput->column; CountColumn++)
		{
			/* Move */
			printf("Mat[%d][%d] = ", CountRow, CountColumn);
			scanf("%d",&(MatInput->Mat[CountRow][CountColumn])); 
		}
	}
}

void PrintMatrix(Matrix MatInput)
{
	uint8_t CountRow;
	uint8_t CountColumn;
	for(CountRow = 0; CountRow < MatInput.row; CountRow++)
	{
		for (CountColumn = 0; CountColumn < MatInput.column; CountColumn++)
		{
			/* Move */
			printf("%d",MatInput.Mat[CountRow][CountColumn]);
			printf("\t");
		}
		printf("\n");
	}
	printf("\n");
}

/* Add 1 more fuction 10/06/2018 */
/* Check if all elements in Matrix is 1 return 1, if not return False (0)*/
uint8_t CheckMatrix(Matrix MatInput)
{
	uint8_t CountRow;
	uint8_t CountColumn;
	for (CountRow = 0; CountRow < MatInput.row; CountRow++)
	{
		for (CountColumn = 0; CountColumn < MatInput.column; CountColumn++)
		{
			if (MatInput.Mat[CountRow][CountColumn] != 1)
			{
				return 0;
			}
		}
	}
	return 1;
}

void AssignMatrixElements(Matrix* MatOutput, Matrix MatInput)
{
	uint8_t CountRow;
	uint8_t CountColumn;
	for (CountRow = 0; CountRow < MatInput.row; CountRow++)
	{
		for (CountColumn = 0; CountColumn < MatInput.column; CountColumn++)
		{
			MatOutput->Mat[CountRow][CountColumn] = MatInput.Mat[CountRow][CountColumn];
		}
	}
}
