/* PA2 estimate */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double** makeMatrix(int rows, int cols){
	double **matrix = (double **)malloc(rows*sizeof(double *));
	int i;
	for (i=0; i<rows; i++){
		matrix[i] = (double *)malloc(cols*sizeof(double));
	}
	return matrix;
}

void clearMatrix(double **matrix, int rows){
	int i;
	for (i=0; i<rows; i++){
		free(matrix[i]);
	}
	free(matrix);
}

void printMatrix(double **matrix, int rows, int cols){	
	int i,j;
	for (i=0; i<rows; i++){
		for (j=0; j<cols; j++){
			printf("%lf\t", matrix[i][j]);
		}
		printf("\n");
	}
}

double** transposeMatrix(double** matrix, int rows, int cols){

	double **transposedMatrix = makeMatrix(cols, rows);

	int i,j;
	for (i=0; i<rows; i++){
		for (j=0; j<cols; j++){
			transposedMatrix[j][i] = matrix[i][j];
		}
	}

	return transposedMatrix;
}

double** multiplyMatrix(double** matrixL, double** matrixR, int rowsL, int rowsR, int colsL, int colsR){

	double **productMatrix = makeMatrix(rowsL, colsR);

	int i,j;
	for (i=0; i<rowsL; i++){
		for (j=0; j<colsR; j++){
			productMatrix[i][j] = 0;
		}
	}

	int k;
	for (i=0; i<rowsL; i++){
		for (j=0; j<colsR; j++){
			for (k=0; k<colsL; k++){
				productMatrix[i][j] += matrixL[i][k]*matrixR[k][j];
			}
		}
	}

	return productMatrix;
}

double** invertMatrix(double** matrix, int rows){

	double **invertedMatrix = makeMatrix(rows,rows);

	int i,j;
	for (i=0; i<rows; i++){
		for (j=0; j<rows; j++){
			if (i==j){
				invertedMatrix[i][j] = 1;
			}
			else {
				invertedMatrix[i][j] = 0;
			}
		}
	}

	double multiplier, underPivot;
	int k;
	for (i=0; i<rows; i++){
		for(j=i; j<rows; j++){
			if (j==i){
				multiplier = matrix[j][i];
				for (k=0; k<rows; k++){
					matrix[j][k] = matrix[j][k]/multiplier;
					invertedMatrix[j][k] = invertedMatrix[j][k]/multiplier;
				}
			}
			else if(matrix[j][i]!=0 && j!=i){
				underPivot = matrix [j][i];
				for (k=0; k<rows; k++){
					matrix[j][k] = matrix[j][k] - (underPivot*matrix[i][k]);
					invertedMatrix[j][k] = invertedMatrix[j][k] - (underPivot*invertedMatrix[i][k]);
				}
			}
		}
	}

	for (i=rows-1; i>=0; i--){
		for (j=i; j>=0; j--){
			if (matrix[j][i] != 1 && j==i){
				multiplier = matrix[j][i];
				
				matrix[j][i] = matrix[j][i]/multiplier;
				invertedMatrix[j][i] = invertedMatrix[j][i]/multiplier;
			}
			else if (matrix[j][i] !=0 && j!=i){
				multiplier = matrix[j][i];

				for (k=rows-1; k>=0; k--){
					matrix[j][k] = matrix[j][k] - (multiplier*matrix[i][k]);
					invertedMatrix[j][k] = invertedMatrix[j][k] - (multiplier*invertedMatrix[i][k]);
				}
			}
		}
	}

	return invertedMatrix;
}


int main(int argc, char **argv)
{
	
	//Opening file and reading rows and cols
	FILE *fileTrain;
	fileTrain = fopen(argv[1], "r");
	char matrixType[6];
	int cols, rows;
	fscanf(fileTrain, "%s", matrixType);
	fscanf(fileTrain, "%d", &cols);
	fscanf(fileTrain, "%d", &rows);
	cols++;
	double **matrix;
	matrix = makeMatrix(rows, cols);

	//Reading data into matrix
	int i,j;
	for (i=0; i<rows; i++){
		for (j=0; j<cols; j++){
			fscanf(fileTrain, "%lf", &matrix[i][j]);
		}
		fscanf(fileTrain, "\n");
	}
	fclose(fileTrain);

	FILE *fileData;
	fileData = fopen(argv[2], "r");
	int rows2;
	fscanf(fileData, "%s", matrixType);
	fscanf(fileData, "%d", &cols);
	fscanf(fileData, "%d", &rows2);
	cols++;
	double **matrix2;
	matrix2 = makeMatrix(rows2, cols-1);

	for (i=0; i<rows2; i++){
		for (j=0; j<cols-1; j++){
			fscanf(fileData, "%lf", &matrix2[i][j]);
		}
		fscanf(fileData, "\n");
	}
	fclose(fileData);

	double **XMatrix;
	XMatrix = makeMatrix(rows,cols);
	for (i=0; i<rows; i++){
		XMatrix[i][0] = 1;
		for (j=1; j<cols; j++){
			XMatrix[i][j] = matrix[i][j-1];
		}
	}

	double **YMatrix;
	YMatrix = makeMatrix(rows, 1);
	for (i=0; i<rows; i++){
		YMatrix[i][0] = matrix[i][cols-1];
	}

	double **XTranspose = transposeMatrix(XMatrix, rows, cols);
	double **XTXProduct = multiplyMatrix(XTranspose, XMatrix, cols, rows, rows, cols);
	double **XTXProductInverse = invertMatrix(XTXProduct, cols);
	double **XProductTotal = multiplyMatrix(XTXProductInverse, XTranspose, cols, cols, cols, rows);
	double **weights = multiplyMatrix(XProductTotal, YMatrix, cols, rows, rows, 1);

	/*
	printf("XMatr: \n");
	printMatrix(XMatrix, rows, cols);
	printf("XTranspose: \n");
	printMatrix(XTranspose, cols, rows);
	printf("XTXProduct: \n");
	printMatrix(XTXProduct, cols, cols);
	printf("XTXProductInverse: \n");
	printMatrix(XTXProductInverse, cols, cols);
	printf("XProductTotal: \n");
	printMatrix(XProductTotal, cols, rows);
	printf("YMatr: \n");
	printMatrix(YMatrix, rows, 1);

	printf("Weight: \n");
	printMatrix(weights, cols, 1);

	printf("Matrix2: \n");
	printMatrix(matrix2, rows2, cols-1);
	*/

	double prediction=0.0;
	for (i=0; i<rows2; i++){
		for (j=-1; j<cols-1; j++){
			if (j==-1){	
				prediction += weights[0][0];
				continue;
			}
			else{
				prediction += weights[j+1][0]*matrix2[i][j];
			}
		}
		printf("%.0f\n", prediction);
		prediction=0;
	}

	clearMatrix(XTranspose, cols);
	clearMatrix(XTXProduct, cols);
	clearMatrix(XTXProductInverse, cols);
	clearMatrix(XProductTotal, cols);
	clearMatrix(weights, cols);
	clearMatrix(XMatrix, rows);
	clearMatrix(YMatrix, rows);
	clearMatrix(matrix2, rows2);
	clearMatrix(matrix, rows);	
	

	return 0;
}
