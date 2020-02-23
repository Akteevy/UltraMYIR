/*
 *
 */
#include "stdint.h"
#include "hls_stream.h"
using namespace hls;

typedef stream<int16_t> str_t;
void convolve(stream<int16_t> *str_in, stream<int16_t> *str_out);

str_t s1, s2;

int main(void){
	uint16_t i, j;
	int16_t indata[30], outdata[30];

	for(i=0; i<30; i++){
		indata[i] = (i % 6) * 10 - 25;
	}
	for (i=0; i<30; i++){
		s1<<indata[i];
		convolve(&s1, &s2);
		outdata[i] = s2.read();
		printf("%d %d\n", i, outdata[i]);
	}
	return 0;
}
