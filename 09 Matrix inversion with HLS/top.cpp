/*
 * Matrix inversion by Gauss-Jordan method
 */
#include "stdint.h"

#define d 6				// matrices dimension
#define d2 d*d			// d^2, matrices size
#define u8 uint8_t		// to type less
#define DTYPE float		// data type

void matinv(DTYPE *m, DTYPE *in){

	DTYPE mat[d2];		// internal copy of input matrix m
#pragma HLS ARRAY_PARTITION dim=0 type=complete variable=mat
	DTYPE inv[d2];		// internal working copy of result
#pragma HLS ARRAY_PARTITION dim=0 type=complete variable=inv

	// copy input matrix into local 2D and create I matrix in inv
	loop_copy_in_1: for(u8 i=0, k=0; i < d; i++)
		loop_copy_in_2: for(u8 j=0; j < d; j++, k++){
			mat[i*d+j] = m[k];
			inv[i*d+j] = (i==j)? 1.0 : 0.0;
		}

	loop_lowtri_1: for(u8 row=0; row < d; row++){
#pragma HLS PIPELINE style=stp
		// for each row 0 to d-1
		if(mat[row*d+row] == 0.0) {		// if diagonal element is zero
			loop_lowtri_2: for(u8 i = row+1; i<d; i++){
				// look in rows below for one with a non-zero element and add to current row
				if(mat[i*d+row] != 0){
					loop_lowtri_3: for(u8 j=0; j<d; j++){
						// add row to current row and same for inv matrix
						mat[row*d+j] += mat[i*d+j];
						inv[row*d+j] += inv[i*d+j];
					}
					break;
				}
			}
		}

		DTYPE k = mat[row*d+row]; // make diagonal element in current row equal to 1
		loop_diag_unit: for(u8 i=0; i<d; i++){
			mat[row*d+i] /= k;
			inv[row*d+i] /= k;
		}

		loop_lowzeros_1: for(u8 i=row+1; i<d; i++){
			// for all the rows below subtract current row as to make elements below diagonal zero
			DTYPE k = mat[i*d+row];
			loop_lowzeros_2: for(u8 j=0; j<d; j++){
				mat[i*d+j] -= k*mat[row*d+j];
				inv[i*d+j] -= k*inv[row*d+j];
			}
		}
	}

	// make zeros in upper triangle starting from bottom
	loop_upzeros_1: for(int8_t row = d-1; row >= 0; row--){
#pragma HLS PIPELINE style=stp
		loop_upzeros_2: for(int8_t i = row-1; i >=0; i--){
			DTYPE k = mat[i*d+row];
			loop_upzeros_3: for(u8 j=0; j<d; j++){
				mat[i*d+j] -= k*mat[row*d+j];
				inv[i*d+j] -= k*inv[row*d+j];
			}
		}
	}

	// copy output to pointed array
	loop_copy_o1: for(u8 i=0, k=0; i < d; i++)
			loop_copy_o2: for(u8 j=0; j < d; j++, k++)
				in[k] = inv[i*d+j];

}
