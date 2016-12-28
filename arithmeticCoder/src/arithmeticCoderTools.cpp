#include "arithmeticCoderTools.h"

ArithmeticCoderTools::ArithmeticCoderTools(){}
ArithmeticCoderTools::~ArithmeticCoderTools(){}

void ArithmeticCoderTools::start_outputing_bits()
{
	
	fout = fopen(EncodeFileName, "wb");
	buffer = 0;					/* Buffer is empty to start */
	bits_to_go = 8;				/* with.                    */
}


/* OUTPUT A BIT. */
void ArithmeticCoderTools::output_bit(int bit)
{  
	buffer >>= 1; if (bit) buffer |= 0x80;	/* Put bit in top of buffer.*/
	bits_to_go -= 1;
	if (bits_to_go == 0) {			/* Output buffer if it is   */
		//putc(buffer, stdout);			/* now full.                */
		putc(buffer, fout);			/* now full.                */
		bits_to_go = 8;
	}
}


/* FLUSH OUT THE LAST BITS. */

void ArithmeticCoderTools::done_outputing_bits()
{
	//putc(buffer >> bits_to_go, stdout);
	putc(buffer >> bits_to_go, fout);

}


//input bits
void ArithmeticCoderTools::start_inputing_bits()
{
	fin = fopen(EncodeFileName, "rb");
	//fin = fopen("out1.txt", "rb");
	bits_to_goInput = 0;				/* Buffer starts out with   */
	garbage_bits = 0;				/* no bits in it.           */
}

int ArithmeticCoderTools::input_bit()
{
	int t;
	counter2++;
	if (bits_to_goInput == 0) {							/* Read the next byte if no */
		bufferInput = getc(fin);						/* bits are left in bufferInput. */
		if (bufferInput == EOF) {
			garbage_bits += 1;							/* Return arbitrary bits*/
			if (garbage_bits>Code_value_bits - 2) {		/* after eof, but check */
				fprintf(stderr, "Bad input file\n");	/* for too many such.   */
				system("pause");
				exit(-1);
			}
		}
		bits_to_goInput = 8;
	}
	t = bufferInput & 1;								/* Return the next bit from */
	bufferInput >>= 1;									/* the bottom of the byte.  */
	bits_to_goInput -= 1;
	return t;
}

void ArithmeticCoderTools::closeEncodeFile()
{
	fclose(fout);
}

void ArithmeticCoderTools::closeDecodeFile()
{
	fclose(fin);
}