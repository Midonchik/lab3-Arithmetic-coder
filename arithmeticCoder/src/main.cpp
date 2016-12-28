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
	int *freq = new int[alphabetSize + 2];

	for (int i = 0; i < alphabetSize + 2; i++)
	{
		freq[i] = 0;
	}
	freq[EOF_symbol] = 1;
	fread(inputData, size, 1, f);

	for (int i = 0; i < size; i++)
	{
		freq[(int)inputData[i]]++;
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
		pr << (double)freq[i]/256 << " ";
	}
	pr.close();
	arCoder->initEncoder(inputData, size, freq);
	for (int i = 0; i < size; i++)
	{
		cout << i << endl;
		arCoder->encodeSymbol(inputData[i]);
	}
	arCoder->encodeSymbol(EOF_symbol);

	arCoder->endEncode();
	fclose(f);

	arCoder->initDecoder();
	int symbol;
	//for (;;)
	for (int i = 0; i < size; i++)
	{
		symbol = arCoder->decodeSymbol();
		if (symbol == EOF_symbol) break;
	}
	arCoder->endDecode();

	system("pause");
	return 0;
}