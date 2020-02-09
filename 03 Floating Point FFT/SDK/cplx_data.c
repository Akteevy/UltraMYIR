
// Includes
#include <stdio.h>
#include "cplx_data.h"

// Public functions
void cplx_data_get_string(char* c, cplx_data_t data)
{
//	sprintf(c, "%d + j*%d", data.data_re, data.data_im);
	sprintf(c, "%10f%+10fj", data.data_re, data.data_im);
}
