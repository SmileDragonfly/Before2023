#include "Matrix.h"
#define ROW_ORIGINAL 6
#define COLUMN_ORIGINAL 8
#define NUMBER_OF_SPICE 11
    MatrixSize Size = {ROW_ORIGINAL,COLUMN_ORIGINAL};
    Matrix A[NUMBER_OF_SPICE];
    Matrix A0[NUMBER_OF_SPICE];
    Matrix MatrixSum;
int main(int argc, char const *argv[])
{
    /* code */
    uint8_t CountMatrix = 0;
    uint8_t CountMatrixMax = NUMBER_OF_SPICE;
    uint8_t c;
/*  MatrixSize Size = {6,8};
    Matrix A[11];
    Matrix A0[11];
    Matrix MatrixSum; */
    
    Matrix MatrixSumTemp[NUMBER_OF_SPICE];
    
    Matrix MatrixMove;
    MatrixSum.row = ROW_ORIGINAL;
    MatrixSum.column = COLUMN_ORIGINAL;
    MatrixMove.row = ROW_ORIGINAL;
    MatrixMove.column = COLUMN_ORIGINAL;
    uint8_t CountVertical[NUMBER_OF_SPICE];
    uint8_t CountHorizontal[NUMBER_OF_SPICE];
    uint8_t CountVerticalMax[NUMBER_OF_SPICE];
    uint8_t CountHorizontalMax[NUMBER_OF_SPICE];
    InitMatrix(&MatrixSum);
/*     GetMatrixElements(&A[0]);
    GetMatrixElements(&A[1]);
    GetMatrixElements(&A[2]);
    GetMatrixElements(&A[3]);
    GetMatrixElements(&A[4]);
    GetMatrixElements(&A[5]);
    GetMatrixElements(&A[6]);
    GetMatrixElements(&A[7]);
    GetMatrixElements(&A[8]); */
/*     GetMatrixElements(&A[9]);
    GetMatrixElements(&A[10]); */
    uint8_t i;
    uint8_t j;
    /* A0 elements value*/
    A[6].row = 3;
    A[6].column = 3;
    A[6].Mat[0][0] = 1;
    A[6].Mat[0][1] = 1;
    A[6].Mat[0][2] = 1;
    A[6].Mat[1][0] = 1;
    A[6].Mat[1][1] = 0;
    A[6].Mat[1][2] = 0;
    A[6].Mat[2][0] = 1;
    A[6].Mat[2][1] = 0;
    A[6].Mat[2][2] = 0;
    
    /* A1 elements value*/
    A[1].row = 3;
    A[1].column = 3;
    A[1].Mat[0][0] = 1;
    A[1].Mat[0][1] = 1;
    A[1].Mat[0][2] = 1;
    A[1].Mat[1][0] = 0;
    A[1].Mat[1][1] = 1;
    A[1].Mat[1][2] = 0;
    A[1].Mat[2][0] = 0;
    A[1].Mat[2][1] = 1;
    A[1].Mat[2][2] = 0;
    
    /* A2 elements value*/
    A[2].row = 3;
    A[2].column = 3;
    A[2].Mat[0][0] = 0;
    A[2].Mat[0][1] = 1;
    A[2].Mat[0][2] = 0;
    A[2].Mat[1][0] = 0;
    A[2].Mat[1][1] = 1;
    A[2].Mat[1][2] = 1;
    A[2].Mat[2][0] = 1;
    A[2].Mat[2][1] = 1;
    A[2].Mat[2][2] = 0;
    
    /* A3 elements value*/
    A[3].row = 3;
    A[3].column = 3;
    A[3].Mat[0][0] = 1;
    A[3].Mat[0][1] = 1;
    A[3].Mat[0][2] = 0;
    A[3].Mat[1][0] = 0;
    A[3].Mat[1][1] = 1;
    A[3].Mat[1][2] = 1;
    A[3].Mat[2][0] = 0;
    A[3].Mat[2][1] = 0;
    A[3].Mat[2][2] = 1;
    
    /* A4 elements value*/
    A[4].row = 2;
    A[4].column = 1;
    A[4].Mat[0][0] = 1;
    A[4].Mat[1][0] = 1;
    
    /* A5 elements value*/
    A[5].row = 3;
    A[5].column = 1;
    A[5].Mat[0][0] = 1;
    A[5].Mat[1][0] = 1;
    A[5].Mat[2][0] = 1;
    
    /* A6 elements value*/
    A[0].row = 2;
    A[0].column = 2;
    A[0].Mat[0][0] = 1;
    A[0].Mat[0][1] = 1;
    A[0].Mat[1][0] = 1;
    A[0].Mat[1][1] = 0;
    
    /* A7 elements value*/
    A[7].row = 2;
    A[7].column = 3;
    A[7].Mat[0][0] = 1;
    A[7].Mat[0][1] = 1;
    A[7].Mat[0][2] = 0;
    A[7].Mat[1][0] = 1;
    A[7].Mat[1][1] = 1;
    A[7].Mat[1][2] = 1;

    /* A8 elements value*/
    A[8].row = 2;
    A[8].column = 4;
    A[8].Mat[0][0] = 0;
    A[8].Mat[0][1] = 1;
    A[8].Mat[0][2] = 0;
    A[8].Mat[0][3] = 0;
    A[8].Mat[1][0] = 1;
    A[8].Mat[1][1] = 1;
    A[8].Mat[1][2] = 1;
    A[8].Mat[1][3] = 1;
    
    /* A9 elements value*/
    A[9].row = 2;
    A[9].column = 4;
    A[9].Mat[0][0] = 1;
    A[9].Mat[0][1] = 1;
    A[9].Mat[0][2] = 1;
    A[9].Mat[0][3] = 0;
    A[9].Mat[1][0] = 0;
    A[9].Mat[1][1] = 0;
    A[9].Mat[1][2] = 1;
    A[9].Mat[1][3] = 1;
    
    /* A10 elements value*/
    A[10].row = 5;
    A[10].column = 1;
    A[10].Mat[0][0] = 1;
    A[10].Mat[1][0] = 1;
    A[10].Mat[2][0] = 1;
    A[10].Mat[3][0] = 1;
    A[10].Mat[4][0] = 1;
    
    for(j = 0; j < NUMBER_OF_SPICE; j++)
    {
        MatrixSumTemp[j].row = ROW_ORIGINAL;
        MatrixSumTemp[j].column = COLUMN_ORIGINAL;
        InitMatrix(&(MatrixSumTemp[j]));
        PrintMatrix(MatrixSumTemp[j]);
    }
    
    /* Print all Matrix and init CountHorizontalMax and CountVerticalMax */
    for (CountMatrix = 0; CountMatrix < NUMBER_OF_SPICE; CountMatrix++)
    {
        /* code */
        printf("A%d = \n", CountMatrix);
        PrintMatrix(A[CountMatrix]);
        printf("\n");
        CountHorizontal[CountMatrix] = 0;
        CountVertical[CountMatrix] = 0;
        CountVerticalMax[CountMatrix] = Size.row - A[CountMatrix].row + 1;
        CountHorizontalMax[CountMatrix] = Size.column - A[CountMatrix].column + 1;
    }
    CountMatrix = 0;
    while (CountMatrix < CountMatrixMax)
    {
        /* If is 1st matrix */
        if(CountMatrix == 0)
        {
            MatrixSum = MoveMatrix(Size, A[CountMatrix], CountHorizontal[CountMatrix], CountVertical[CountMatrix]);
            MatrixSumTemp[CountMatrix] = MatrixSum;
            CountMatrix++;
        }
        else
        {
            while(CountVertical[CountMatrix] < CountVerticalMax[CountMatrix])
            {
                while(CountHorizontal[CountMatrix] < CountHorizontalMax[CountMatrix])
                {
                    MatrixSum = AddMatrix(MatrixSumTemp[CountMatrix - 1],MoveMatrix(Size, A[CountMatrix], CountHorizontal[CountMatrix], CountVertical[CountMatrix]));
                    MatrixSumTemp[CountMatrix] = MatrixSum;
                    PrintMatrix(MatrixSum);
                    if(CheckMatrix(MatrixSum) == 0)
                    {
                        CountHorizontal[CountMatrix]++;
                    }
                    else
                    {
                        CountHorizontal[CountMatrix]++;
                        break;
                    }
                }
                if(CheckMatrix(MatrixSum) == 0)
                {
                    CountVertical[CountMatrix]++;
                    CountHorizontal[CountMatrix] = 0;
                    if(CountVertical[CountMatrix] == CountVerticalMax[CountMatrix])
                    {
                        CountHorizontal[CountMatrix] = 0;
                        CountVertical[CountMatrix] = 0;
                        CountMatrix--;
                    }
                }
                else
                {
                    CountMatrix++;
                    break;
                }
                if(CountMatrix == 0)
                {
                    CountHorizontal[CountMatrix]++;
                    if(CountHorizontal[CountMatrix] == CountHorizontalMax[CountMatrix])
                    {
                        CountVertical[CountMatrix]++;
                        CountHorizontal[CountMatrix] = 0;
                    }
                    break;
                }
            }
        }
    }
    for (i = 0; i < CountMatrixMax; i++)
    {
        if(i == 0)
        {
            printf("CountVertical[%d] = %d\n", i, CountVertical[i]);
            printf("CountHorizontal[%d] = %d\n", i, CountHorizontal[i]);
        }
        else
        {
            printf("CountVertical[%d] = %d\n", i, CountVertical[i]);
            printf("CountHorizontal[%d] = %d\n", i, CountHorizontal[i] - 1);
        }
    }
    return 0;
}
