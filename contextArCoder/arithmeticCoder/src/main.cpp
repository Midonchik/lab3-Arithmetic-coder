#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "arithmeticCoder.h"
#include "arithmeticCoderTools.h"
#include "defines.h"

using namespace std;

int main()
{
	ArithmeticCoder *arCoder = new ArithmeticCoder();
	int alphabetSize = No_of_chars;
	FILE *f;
	f = fopen("data_static.txt", "rb");
	fseek(f, 0L, SEEK_END);
	int size = ftell(f);
	fseek(f, 0L, SEEK_END);
	rewind(f);

	char *inputData = new char[size];
	int **freq = new int*[alphabetSize + 2];
	for (int i = 0; i < alphabetSize + 2; i++)
	{
		freq[i] = new int[alphabetSize + 2];
		for (int i2 = 0; i2 < alphabetSize + 2; i2++)
		{
			freq[i][i2] = 0;
		}

	}

	for (int i = 0; i < alphabetSize + 2; i++)
	{
		freq[i][EOF_symbol] = 1;
	}


	fread(inputData, size, 1, f);

	for (int i = 0; i < size; i++)
	{
		int y = (int)inputData[i];
		int x = (int)inputData[i + 1];
		freq[y][x]++;
	}

	//int c = 8;
	//for (int i = 1; i < 4; i++)
	//{
	//	int buf = 0;
	//	buf = freq[i];
	//	freq[i] = freq[c];
	//	freq[c] = buf;
	//	c--;
	//}
	fstream pr;
	pr.open("Pr.txt");
	for (int i = 0; i < alphabetSize; i++)
	{
		for (int i2 = 0; i2 < alphabetSize; i2++)
		{
			pr << freq[i][i2] << " ";
		}
		pr << endl;
	}
	pr.close();

	FILE *f2;
	f2 = fopen(EncodeFileName, "wb");
	fwrite(&inputData[0], 1, 1, f2);
	fclose(f2);

	arCoder->initEncoder(inputData, size, freq[inputData[0]]);
	for (int i = 1; i < size; i++)
	{
		cout << i << endl;
		arCoder->encodeSymbol(inputData[i]);
		arCoder->initCum_freq(freq[inputData[i]]);
	}
	arCoder->encodeSymbol(EOF_symbol);

	arCoder->endEncode();
	fclose(f);

	char frstSymb = 0;
	arCoder->initDecoder(&frstSymb);
	arCoder->initCum_freq(freq[frstSymb]);

	int symbol;
	//for (;;)
	for (int i = 1; i < size; i++)
	{
		symbol = arCoder->decodeSymbol();
		arCoder->initCum_freq(freq[symbol]);
		if (symbol == EOF_symbol) break;
	}
	arCoder->endDecode();

	system("pause");
	return 0;
}