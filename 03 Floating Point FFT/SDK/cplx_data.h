
//////////////////////////////////////////////////////////////////////////////////
//
// Company:        Xilinx
// Engineer:       bwiec
// Create Date:    11 May 2015, 04:42:38 PM
// Library Name:   Complex Data
// File Name:      cplx_data.h
// Target Devices: Zynq
// Tool Versions:  2015.1
// Description:    API for handling of complex data for FFT accelerator.
// Dependencies:
//   - None
// Revision History:
//   - v1.0
//     * Initial release
//     * Tested on ZC702 and Zedboard
// Additional Comments:
//   - None
//
//////////////////////////////////////////////////////////////////////////////////

#ifndef CPLX_DATA_H_
#define CPLX_DATA_H_

// For use with our FFT application, this needs to be 16-bit samples
// packed into a 32-bit word so that the DMA can populate it automatically

typedef struct cplx_data
{
	/*short*/ float data_re;
	/*short*/ float data_im;
} cplx_data_t;

// Public functions

//
// cplx_data_get_string - Get a C-style string for a given data sample.
//
//  Arguments
//    - c:    C-style string in which we place the sample value string.
//    - data: Sample whose value we want as a string.
//
void cplx_data_get_string(char* c, cplx_data_t data);

#endif /* CPLX_DATA_H_ */

