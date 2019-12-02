// OS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <Psapi.h>
#include <string>
#include <cstring>

struct matrix
{
	int m;
	int n;
};

struct Args
{
	int * row;
	int * col;
	int count;
};

void mul(Args &args)
{
	int r = 0;
	for (int i = 0; i < args.count; i++)
		r += args.row[i] * args.col[i];
	args.count = r;
}


void main()
{
	matrix mat1;
	matrix mat2;

	int ** m1;
	int ** m2;
	int ** mr;

	std::cout << "Matrix 1 : ";
	std::cin >> mat1.m >> mat1.n;
	std::cout << "Matrix 2 : ";
	std::cin >> mat2.m >> mat2.n;

	if (mat1.n == mat2.m)
	{
		#pragma region Declear Matrices
		/* Matrix 1 */
		m1 = new int *[mat1.m];
		for (int i = 0; i < mat1.m; i++)
			m1[i] = new int[mat1.n];

		/* Matrix 2 */
		m2 = new int *[mat2.m];
		for (int i = 0; i < mat2.m; i++)
			m2[i] = new int[mat2.n];

		/* Result Matrix */
		mr = new int *[mat1.m];
		for (int i = 0; i < mat1.m; i++)
			mr[i] = new int[mat2.n];
#pragma endregion
		
		#pragma region Get Matrices Values
		/* Matrix 1 */
		std::cout << "Matrix 1 (" << mat1.m << " x " << mat1.n << " )  values : \n";
		for (int i = 0; i < mat1.m; i++)
			for (int j = 0; j < mat1.n; j++)
				std::cin >> m1[i][j];

		/* Matrix 2*/
		std::cout << "Matrix 2 (" << mat2.m << " x " << mat2.n << " )  values : \n";
		for (int i = 0; i < mat2.m; i++)
			for (int j = 0; j < mat2.n; j++)
				std::cin >> m2[i][j];
#pragma endregion
		
		#pragma region Multiplying Matrices
		int * arg1 = new int[mat1.m];
		int * arg2 = new int[mat1.m];
		int ThCount = (mat1.m * mat2.n);
		Args * data = new Args[ThCount];
		DWORD  * ThreadsId = new DWORD[ThCount];
		HANDLE * ThHandle = new HANDLE[ThCount];

		int count = 0;

		int arrcount = mat1.m; // arg elements number
		for (int i = 0; i < mat1.m; i++)
		{
			for (int j = 0; j < mat1.m; j++)
				arg1[j] = m1[i][j];

			for (int k = 0; k < mat2.n; k++)
			{
				for (int x = 0; x < mat1.m; x++)
					arg2[x] = m2[x][k];

				data[count] = { arg1 , arg2 , arrcount };
				ThHandle[count] = CreateThread
				(NULL, 0, (LPTHREAD_START_ROUTINE)&mul, &data[count], 0, &ThreadsId[count]);
				count++;
			}
		} // End for

		WaitForMultipleObjects(ThCount, ThHandle, TRUE, INFINITE);

		count = 0;
		for (int i = 0; i < mat1.m; i++)
			for (int j = 0; j < mat2.n; j++)
			{
				mr[i][j] = data[count].count;
				count++;
			}

		for (int i = 0; i<ThCount; i++)
			CloseHandle(ThHandle[i]);
#pragma endregion
		
		#pragma region Print Matrices
		std::cout << "\n=============================\nMatrix 1:\n";
		/* Matrix 1 */
		for (int i = 0; i < mat1.m; i++)
		{
			for (int j = 0; j < mat1.n; j++)
				std::cout << m1[i][j] << " ";
			std::cout << "\n";
		}

		/* Matrix 2 */
		std::cout << "Matrix 2:\n";
		for (int i = 0; i < mat2.m; i++)
		{
			for (int j = 0; j < mat2.n; j++)
				std::cout << m2[i][j] << " ";
			std::cout << "\n";
		}

		/* Result Matrix */
		std::cout << "=============================\nResult :\n";
		for (int i = 0; i < mat1.m; i++)
		{
			for (int j = 0; j < mat2.n; j++)
				std::cout << mr[i][j] << " ";
			std::cout << "\n";
		}
#pragma endregion
		
		#pragma region Delete Matrices
		/* Matrix 1*/
		for (int i = 0; i < mat1.m; i++)
			delete[] m1[i];

		/* Matrix 2*/
		for (int i = 0; i < mat2.m; i++)
			delete[] m2[i];

		/* Result Matrix */
		for (int i = 0; i < mat1.m; i++)
			delete[] mr[i];

		/* Other Elements */
		delete[] ThHandle;
		delete[] ThreadsId;
		delete[] arg1;
		delete[] arg2;
		delete[] data;
#pragma endregion
	}
	else
	{
		std::cout << "\ncannot multiplying these matrices "
			<< "(" << mat1.m << " x " << mat1.n << ") x "
			<< "(" << mat2.m << " x " << mat2.n << ").\n";
	}
	system("pause");
}

