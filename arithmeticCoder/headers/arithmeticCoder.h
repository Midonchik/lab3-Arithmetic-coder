#ifndef ARITHMETIC_CODER
#define ARITHMETIC_CODER

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h> 
#include "arithmeticCoderTools.h"
#include "defines.h"

class ArithmeticCoder{

private:
/* TRANSLATION TABLES BETWEEN CHARACTERS AND SYMBOL INDEXES. */

int char_to_index[No_of_chars];		/* To index from character          */
unsigned char index_to_char[No_of_symbols + 1]; /* To character from index    */
int cum_freq[No_of_symbols + 1];		/* Cumulative symbol frequencies    */

int high = Top_value;
int low = 0;
int bits_to_follow = 0;
ArithmeticCoderTools *arTools;

code_value value;	/* Currently-seen code value                */
code_value lowDec, highDec;	/* Ends of current code region              */

FILE *foutDecode;
void bit_plus_follow(int bit);
void start_encoding();
void start_decoding();

public:
	ArithmeticCoder();
	~ArithmeticCoder();
	void initEncoder(char* data, int size, int *freq);
	void initDecoder();
	void encodeSymbol(int symbol);
	void done_encoding();
	int decodeSymbol();
	void update_model(int symbol);
	//void decode();
	void endEncode();
	void endDecode();

};

#endif