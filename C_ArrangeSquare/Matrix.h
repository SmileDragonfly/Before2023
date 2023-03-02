#include <stdio.h>
#include <stdint.h>

/* This application purpose is resolve a game that arrange all parts separated to a rectangle */
/* Declare struct */

typedef struct Matrix
{
	uint8_t row;
	uint8_t column;
	uint8_t Mat[10][10];	
}Matrix;

typedef struct MatrixSize
{
	uint8_t row;
	uint8_t column;
}MatrixSize;

/* Function prototype */

Matrix MoveMatrix(MatrixSize SizeInput, Matrix MatInput, uint8_t Horizontal, uint8_t Vertical);

Matrix AddMatrix(Matrix MatInput1, Matrix MatInput2);

void InitMatrix(Matrix* MatInput);

void GetMatrixElements(Matrix* MatInput);

void PrintMatrix(Matrix MatInput);

uint8_t CheckMatrix(Matrix MatInput);

void AssignMatrixElements(Matrix* MatOutput,Matrix MatInput);
