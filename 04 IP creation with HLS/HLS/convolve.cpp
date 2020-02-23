/*
 *
 */
#include "stdint.h"
#include "hls_stream.h"
using namespace hls;

void convolve(stream<int16_t> *str_in, stream<int16_t> *str_out){
	static int16_t buff[8] = {0,0,0,0,0,0,0,0};
	const int16_t coeff[8] = {-2, -5, -10, -4, 4, 10, 5, 2};
	static int16_t acc = 0;
	uint16_t i;

	buff_shift:for(i=7; i>0; i--) buff[i] = buff[i-1];
	str_in->read(buff[0]);
	mult:for(i=0; i<8; i++){
		acc += buff[i]*coeff[i];
	}
	str_out->write(acc);
	acc = 0;
}
