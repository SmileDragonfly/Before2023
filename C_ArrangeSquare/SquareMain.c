#include "Matrix.h"
	MatrixSize Size = {6,8};
	Matrix A[11];
	Matrix A0[11];
	Matrix MatrixSum;
int main(int argc, char const *argv[])
{
	/* code */
	uint8_t CountMatrix;
	uint8_t c;
/* 	MatrixSize Size = {6,8};
	Matrix A[11];
	Matrix A0[11];
	Matrix MatrixSum; */
	
	Matrix MatrixSumTemp[10];
	
	Matrix MatrixMove;
	MatrixSum.row = 6;
	MatrixSum.column = 8;
	MatrixMove.row = 6;
	MatrixMove.column = 8;
	uint8_t CountVertical[11];
	uint8_t CountHorizontal[11];
	InitMatrix(&MatrixSum);
	/*GetMatrixElements(&A[0]);
	GetMatrixElements(&A[1]);
	GetMatrixElements(&A[2]);
	GetMatrixElements(&A[3]);
	GetMatrixElements(&A[4]);
	GetMatrixElements(&A[5]);
	GetMatrixElements(&A[6]);
	GetMatrixElements(&A[7]);
	GetMatrixElements(&A[8]);
	GetMatrixElements(&A[9]);
	GetMatrixElements(&A[10]);*/
	uint8_t i;
	uint8_t j;
	/* Init MatrixSumTemp */
	for(j = 0; j < 11; j++)
	{
		MatrixSumTemp[j].row = 6;
		MatrixSumTemp[j].column = 8;
		InitMatrix(&(MatrixSumTemp[j]));
		PrintMatrix(MatrixSumTemp[j]);
	}
	
	/* A0 elements value*/
	A[0].row = 3;
	A[0].column = 3;
	A[0].Mat[0][0] = 1;
	A[0].Mat[0][1] = 1;
	A[0].Mat[0][2] = 1;
	A[0].Mat[1][0] = 1;
	A[0].Mat[1][1] = 0;
	A[0].Mat[1][2] = 0;
	A[0].Mat[2][0] = 1;
	A[0].Mat[2][1] = 0;
	A[0].Mat[2][2] = 0;
	
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
	A[6].row = 2;
	A[6].column = 2;
	A[6].Mat[0][0] = 1;
	A[6].Mat[0][1] = 1;
	A[6].Mat[1][0] = 1;
	A[6].Mat[1][1] = 1;
	
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
	/* Print all Matrix */
	for (CountMatrix = 0; CountMatrix < 11; CountMatrix++)
	{
		/* code */
		printf("A%d = \n", CountMatrix);
		PrintMatrix(A[CountMatrix]);
		printf("\n");
	}
	
	/* Move and Sum Matrixes */
	for(CountVertical[0] = 0; CountVertical[0]<(Size.row - A[0].row + 1); CountVertical[0]++)
	{
		for (CountHorizontal[0] = 0; CountHorizontal[0] <(Size.column - A[0].column + 1); CountHorizontal[0]++)
		{
			MatrixSum = MoveMatrix(Size, A[0], CountHorizontal[0], CountVertical[0]);
			for (CountVertical[1] = 0; CountVertical[1]<(Size.row - A[1].row + 1); CountVertical[1]++)
			{
				for (CountHorizontal[1] = 0; CountHorizontal[1] <(Size.column - A[1].column + 1); CountHorizontal[1]++)
				{
/* 					MatrixSumTemp = MatrixSum; */
/* 					PrintMatrix(MatrixSumTemp);	 */				
/* 					MatrixMove = MoveMatrix(Size, A[1], CountHorizontal[1], CountVertical[1]); */
					MatrixSumTemp[1] = MatrixSum;
					MatrixSum = AddMatrix(MatrixSum, MoveMatrix(Size, A[1], CountHorizontal[1], CountVertical[1]));
					PrintMatrix(MatrixSum);
					while(0 == CheckMatrix(MatrixSum))
					{
						MatrixSum = MatrixSumTemp[1];
						break;
					}
					for (CountVertical[2] = 0; CountVertical[2]<(Size.row - A[2].row + 1); CountVertical[2]++)
					{
						for (CountHorizontal[2] = 0; CountHorizontal[2] <(Size.column - A[2].column + 1); CountHorizontal[2]++)
						{
							MatrixSumTemp[2] = MatrixSum;
							MatrixMove = MoveMatrix(Size, A[2], CountHorizontal[2], CountVertical[2]);
							MatrixSum = AddMatrix(MatrixSum, MatrixMove);
							PrintMatrix(MatrixSum);
							for (CountVertical[3] = 0; CountVertical[3]<(Size.row - A[3].row + 1); CountVertical[3]++)
							{
								for (CountHorizontal[3] = 0; CountHorizontal[3] <(Size.column - A[3].column + 1); CountHorizontal[3]++)
								{
									MatrixSumTemp[3] = MatrixSum;
									MatrixSum = AddMatrix(MatrixSum, MoveMatrix(Size, A[3], CountHorizontal[3], CountVertical[3]));
									PrintMatrix(MatrixSum);
									for (CountVertical[4] = 0; CountVertical[4]<(Size.row - A[4].row + 1); CountVertical[4]++)
									{
										for (CountHorizontal[4] = 0; CountHorizontal[4] <(Size.column - A[4].column + 1); CountHorizontal[4]++)
										{
											MatrixSumTemp[4] = MatrixSum;
											MatrixSum = AddMatrix(MatrixSum, MoveMatrix(Size, A[4], CountHorizontal[4], CountVertical[4]));
											PrintMatrix(MatrixSum);
											for (CountVertical[5] = 0; CountVertical[5]<(Size.row - A[5].row + 1); CountVertical[5]++)
											{
												for (CountHorizontal[5] = 0; CountHorizontal[5] <(Size.column - A[5].column + 1); CountHorizontal[5]++)
												{
													MatrixSumTemp[5] = MatrixSum;
													MatrixSum = AddMatrix(MatrixSum, MoveMatrix(Size, A[5], CountHorizontal[5], CountVertical[5]));
													PrintMatrix(MatrixSum);
													for (CountVertical[6] = 0; CountVertical[6]<(Size.row - A[6].row + 1); CountVertical[6]++)
													{
														for (CountHorizontal[6] = 0; CountHorizontal[6] <(Size.column - A[6].column + 1); CountHorizontal[6]++)
														{
															MatrixSumTemp[6] = MatrixSum;
															MatrixSum = AddMatrix(MatrixSum, MoveMatrix(Size, A[6], CountHorizontal[6], CountVertical[6]));
															PrintMatrix(MatrixSum);
															for (CountVertical[7] = 0; CountVertical[7]<(Size.row - A[7].row + 1); CountVertical[7]++)
															{
																for (CountHorizontal[7] = 0; CountHorizontal[7] <(Size.column - A[7].column + 1); CountHorizontal[7]++)
																{
																	MatrixSumTemp[7] = MatrixSum;
																	MatrixSum = AddMatrix(MatrixSum, MoveMatrix(Size, A[7], CountHorizontal[7], CountVertical[7]));
																	PrintMatrix(MatrixSum);
																	for (CountVertical[8] = 0; CountVertical[8]<(Size.row - A[8].row + 1); CountVertical[8]++)
																	{
																		for (CountHorizontal[8] = 0; CountHorizontal[8] <(Size.column - A[8].column + 1); CountHorizontal[8]++)
																		{
																			MatrixSumTemp[8] = MatrixSum;
																			MatrixSum = AddMatrix(MatrixSum, MoveMatrix(Size, A[8], CountHorizontal[8], CountVertical[8]));
																			PrintMatrix(MatrixSum);
																			for (CountVertical[9] = 0; CountVertical[9]<(Size.row - A[9].row + 1); CountVertical[9]++)
																			{
																				for (CountHorizontal[9] = 0; CountHorizontal[9] <(Size.column - A[9].column + 1); CountHorizontal[9]++)
																				{
																					MatrixSumTemp[9] = MatrixSum;
																					MatrixSum = AddMatrix(MatrixSum, MoveMatrix(Size, A[9], CountHorizontal[9], CountVertical[9]));
																					PrintMatrix(MatrixSum);
																					for (CountVertical[10] = 0; CountVertical[10]<(Size.row - A[10].row + 1); CountVertical[10]++)
																					{
																						for (CountHorizontal[10] = 0; CountHorizontal[10] <(Size.column - A[10].column + 1); CountHorizontal[10]++)
																						{
																							/* AssignMatrixElements(&(MatrixSumTemp[10]),MatrixSum); */
																							MatrixSumTemp[10] = MatrixSum;
																							printf("Matrix 10\n");
																							MatrixSum = AddMatrix(MatrixSum, MoveMatrix(Size, A[10], CountHorizontal[10], CountVertical[10]));
																							PrintMatrix(MatrixSum);
																							if ( 1 == CheckMatrix(MatrixSum))
																							{
																								for (i = 0; i < 11; i++)
																								{
																									printf("CountVertical[%d] = %d\n", i, CountVertical[i]);
																									printf("CountHorizontal[%d] = %d\n", i, CountHorizontal[i]);
																								}
																								return 0;
																							}
																							else
																							{
																								MatrixSum = MatrixSumTemp[10];
																							}
																						}
																					}
																					MatrixSum = MatrixSumTemp[9];
																				}
																			}
																			MatrixSum = MatrixSumTemp[8];
																		}
																	}
																	MatrixSum = MatrixSumTemp[7];
																}
															}
															MatrixSum = MatrixSumTemp[6];
														}
													}
													MatrixSum = MatrixSumTemp[5];
												}
											}
											MatrixSum = MatrixSumTemp[4];
										}
									}
									MatrixSum = MatrixSumTemp[3];
								}
							}
							MatrixSum = MatrixSumTemp[2];
						}
					}
					MatrixSum = MatrixSumTemp[1];
				}
			}
		}
	}
	
	c = getchar();
	return 0;
}
