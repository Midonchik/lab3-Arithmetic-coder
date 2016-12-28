#ifndef ARITHMETIC_CODER_TOOLS
#define ARITHMETIC_CODER_TOOLS

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "defines.h"
#include <stdlib.h>
/* INITIALIZE FOR BIT OUTPUT. */
class ArithmeticCoderTools{
private:
	int buffer;		/* Bits buffered for output                 */
	int bits_to_go;		/* Number of bits free in buffer            */

	int bufferInput;                 /* —ам буфеp             */
	int bits_to_goInput;             /* —колько битов в буфеpе*/
	int garbage_bits;
	FILE *fout;
	FILE *fin; /*file for decode*/
public:
	int counter = 0;
	int counter2 = 0;

	ArithmeticCoderTools();
	~ArithmeticCoderTools();

	//output bits
	void start_outputing_bits();
	void output_bit(int bit);
	void done_outputing_bits();

	//input bits
	void start_inputing_bits();
	int input_bit();
	
	void closeEncodeFile();
	void closeDecodeFile();

};

#endif