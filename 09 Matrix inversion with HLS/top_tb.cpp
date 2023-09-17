/*
 * Matrix inversion by Gauss-Jordan - testbench
 */

#include "stdint.h"
#include "stdio.h"

#define d 6
#define d2 d*d
#define u8 uint8_t

#define DTYPE float		// data type

void matinv(DTYPE *m, DTYPE *in);

int main(){

	float f[d2] = { 1.0, 2.0, 0.0,-1.0, 3.0, 2.0,	// test matrix
			        0.0,-1.0, 1.5, 3.2, 1.1, 0.8,
					1.4, 2.2,-0.4, 2.1, 1.7,-0.6,
					2.4,-2.0, 1.9, 1.3,-2.5,-1.2,
					3.0, 2.6, 2.4, 1.1, 5.1, 2.8,
					2.0,-1.5,-3.0,-2.1,-1.6, 0.1
			};

	float g[d2] = { 1.0, 2.0, 0.0,-1.0, 3.0, 2.0,	// copy of f to test A * A^-1 = I
			        0.0,-1.0, 1.5, 3.2, 1.1, 0.8,
					1.4, 2.2,-0.4, 2.1, 1.7,-0.6,
					2.4,-2.0, 1.9, 1.3,-2.5,-1.2,
					3.0, 2.6, 2.4, 1.1, 5.1, 2.8,
					2.0,-1.5,-3.0,-2.1,-1.6, 0.1
			};

	float inv[d2];
	float pro[d*d];

	printf("input matrix f\n");
	printf("=======================\n");
	for(uint8_t i=0, z=0; i<d; i++){
		for(uint8_t j=0; j<d; j++, z++){
			printf("%+6.4f ", f[z]);
		}
		printf("\n");
	}
	printf("\n");

	matinv(f, inv);

	printf("inverse of input matrix\n");
	printf("=======================\n");
	for(uint8_t i=0, z=0; i<d; i++){
		for(uint8_t j=0; j<d; j++, z++){
			printf("%+6.4f ", inv[z]);
		}
		printf("\n");
	}
	printf("\n");

	// multiply pro = g * inv(f)
	for(uint8_t i=0, ia=0; i<d; i++, ia+=d){
		for(uint8_t j=0; j<d; j++){
			pro[ia+j] = 0.0;
			for(uint8_t k=0, ib = j; k<d; k++, ib+=d){
				pro[ia+j] += g[ia+k]*inv[ib];
			}
		}
	}

	printf("product of f*inv(f)\n");
	printf("=======================\n");
	for(uint8_t i=0, z=0; i<d; i++){
		for(uint8_t j=0; j<d; j++, z++){
			printf("%+6.4f ", pro[z]);
		}
		printf("\n");
	}

	return 0;
}
