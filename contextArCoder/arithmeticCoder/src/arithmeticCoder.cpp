#include "arithmeticCoder.h"

ArithmeticCoder::ArithmeticCoder(){}
ArithmeticCoder::~ArithmeticCoder(){}

void ArithmeticCoder::initCum_freq(int *freq)
{
	int noOfSymbols = No_of_symbols;
	int noOfChars = No_of_chars;

	for (int i = 0; i<noOfChars; i++) { /* Установка таблиц    */
		char_to_index[i] = i + 1;     /* пеpекодиpовки типов   */
		index_to_char[i + 1] = i;
	}
	cum_freq[noOfSymbols] = 0;
	for (int i = noOfSymbols; i > 0; i--)
	{												/* Установка  */
		cum_freq[i - 1] = cum_freq[i] + freq[i];    /* счетчиков  */
	}
	if (cum_freq[0] > Max_frequency) /* Пpовеpка*/
	{
		printf("cumFreq>Max_freq");
		abort();
	}

}
void ArithmeticCoder::initEncoder(char* data, int size, int *freq)
{
	int noOfSymbols = No_of_symbols;
	int noOfChars = No_of_chars;

	arTools = new ArithmeticCoderTools();
	for (int i = 0; i<noOfChars; i++) { /* Установка таблиц    */
	char_to_index[i] = i + 1;     /* пеpекодиpовки типов   */
	index_to_char[i + 1] = i;
	}
	cum_freq[noOfSymbols] = 0;
	for (int i = noOfSymbols; i > 0; i--)
	{												/* Установка  */
		cum_freq[i - 1] = cum_freq[i] + freq[i];    /* счетчиков  */
	}
												/* накопленных частот  */
	arTools->start_outputing_bits();
	start_encoding();
	
}

void ArithmeticCoder::start_encoding()
{   
	low = 0;                 /* Полный кодовый интеpвал     */
    high = Top_value;
    bits_to_follow = 0;      /* Добавлять биты пока не надо */
}

void ArithmeticCoder::encodeSymbol(int symbol)
{   
	long range;			/* Size of the current code region          */
	range = (long)(high - low) + 1;
	high = low +				/* Narrow the code region   */
	(range*cum_freq[symbol - 1]) / cum_freq[0] - 1;	/* to that allotted to this */
	low = low + 				/* symbol.                  */
	(range*cum_freq[symbol]) / cum_freq[0];
	for (;;) {					/* Loop to output bits.     */
		if (high<Half) {
			bit_plus_follow(0);			/* Output 0 if in low half. */
		}
		else if (low >= Half) {			/* Output 1 if in high half.*/
			bit_plus_follow(1);
			low -= Half;
			high -= Half;			/* Subtract offset to top.  */
		}
		else if (low >= First_qtr			/* Output an opposite bit   */
			&& high<Third_qtr) {		/* later if in middle half. */
			bits_to_follow += 1;
			low -= First_qtr;			/* Subtract offset to middle*/
			high -= First_qtr;
		}
		else break;				/* Otherwise exit loop.     */
		low = 2 * low;
		high = 2 * high + 1;			/* Scale up code range.     */
	}
}


void ArithmeticCoder::bit_plus_follow(int bit)
{   
	arTools->output_bit(bit);			/* Output the bit.          */
	while (bits_to_follow>0) {
		arTools->output_bit(!bit);		/* Output bits_to_follow    */
		bits_to_follow -= 1;			/* opposite bits. Set       */
	}									/* bits_to_follow to zero.  */
}
void ArithmeticCoder::done_encoding()
{
	bits_to_follow += 1;			/* Output two bits that     */
	if (low<First_qtr) bit_plus_follow(0);	/* select the quarter that  */
	else bit_plus_follow(1);			/* the current code range   */
}						/* contains.                */

void ArithmeticCoder::update_model(int symbol)
{
	/* Hичего не делается    */
}

//for decoder
void ArithmeticCoder::initDecoder(char *frstSymb)
{
	*frstSymb = arTools->start_inputing_bits();
	firstSymbol = *frstSymb;
	start_decoding();
}

void ArithmeticCoder::start_decoding()
{
	int i;
	foutDecode = fopen(DecodeFileName, "wb");
	fwrite(&firstSymbol, 1, 1, foutDecode);
	//start_inputing_bits()
	value = 0;					/* Input bits to fill the   */
	for (i = 1; i <= Code_value_bits; i++) {	/* code value.              */
		value = 2 * value + arTools->input_bit();
		arTools->counter++;
	}
	lowDec = 0;					/* Full code range.         */
	highDec = Top_value;
}

int ArithmeticCoder::decodeSymbol() /* Cumulative symbol frequencies            */
{   
	long range;									/* Size of current code region              */
	int cum;									/* Cumulative frequency calculated          */
	int symbol;									/* Symbol decoded                           */
	range = (long)(highDec - lowDec) + 1;
	cum = 														/* Find cum freq for value. */
	(((long)(value - lowDec) + 1)*cum_freq[0] - 1) / range;
	for (symbol = 1; cum_freq[symbol]>cum; symbol++);			/* Then find symbol. */
	highDec = lowDec +											/* Narrow the code region   */
	(range*cum_freq[symbol - 1]) / cum_freq[0] - 1;				/* to that allotted to this */
	lowDec = lowDec + 											/* symbol.                  */
	(range*cum_freq[symbol]) / cum_freq[0];
	for (;;) {									/* Loop to get rid of bits. */
		if (highDec<Half) {
			/* nothing */						/* Expand low half.         */
		}
		else if (lowDec >= Half) {				/* Expand high half.        */
			value -= Half;
			lowDec -= Half;						/* Subtract offset to top.  */
			highDec -= Half;
		}
		else if (lowDec >= First_qtr			/* Expand middle half.      */
			&& highDec<Third_qtr) {
			value -= First_qtr;
			lowDec -= First_qtr;				/* Subtract offset to middle*/
			highDec -= First_qtr;
		}
		else break;								/* Otherwise exit loop.     */
		lowDec = 2 * lowDec;
		highDec = 2 * highDec + 1;				/* Scale up code range.     */
		value = 2 * value + arTools->input_bit();		/* Move in next input bit.  */

	}
	char charBuffer = (char)symbol;

	fwrite(&charBuffer, 1, 1, foutDecode);
	return symbol;
}

void ArithmeticCoder::endEncode()
{
	arTools->closeEncodeFile();
}

void ArithmeticCoder::endDecode()
{
	fclose(foutDecode);
	arTools->closeDecodeFile();
}
