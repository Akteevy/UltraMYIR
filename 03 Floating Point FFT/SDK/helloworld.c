
//////////////////////////////////////////////////////////////////////////////////
//
// Company:        Xilinx
// Engineer:       bwiec
// Create Date:    29 June 2015, 03:34:03 PM
// App Name:       DMA Accelerator Demonstration
// File Name:      helloworld.c
// Target Devices: Zynq
// Tool Versions:  2015.1
// Description:    Implementation of FFT using a hardware accelerator with DMA.
// Dependencies:
//   - xuartps_hw.h - Driver version v3.0
//   - fft.h        - Driver version v1.0
//   - cplx_data.h  - Driver version v1.0
//   - stim.h       - Driver version v1.0
// Revision History:
//   - v1.0
//     * Initial release
//     * Tested on ZC702 and Zedboard
// Additional Comments:
//   - UART baud rate is 115200
//   - GPIO is used with some additional glue logic to control the FFT core's
//     config interface for setting various parameters.
//
//////////////////////////////////////////////////////////////////////////////////

// Includes
#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xuartps_hw.h"
#include "fft.h"
#include "cplx_data.h"
//#include "stim.h"
#include "math.h"
#include "xtime_l.h"

// External data
cplx_data_t sig_two_sine_waves[FFT_MAX_NUM_PTS]; // FFT input data

// Function prototypes
void which_fft_param(fft_t* p_fft_inst);
XTime tStart, tEnd;

// Main entry point
int main()
{

	// Local variables
	int          status = 0;
	char         c;
	fft_t*       p_fft_inst;
	cplx_data_t* stim_buf;
	cplx_data_t* result_buf;
	int i;
	double w;
	float y;

	// Setup UART and enable caches
    init_platform();
    xil_printf("\fHello World!\n\r");

    // fill stimulus with float data
    for (i=0; i<FFT_MAX_NUM_PTS; i++){
    	w = M_PI * (double)i;
    	y = sin(w);
    	y += sin(0.1 + 0.15*w);
    	y += sin(-0.13 + 0.022*w);
    	y += sin(0.08 + 0.0037*w);
    	sig_two_sine_waves[i].data_re = y;
    	sig_two_sine_waves[i].data_im = 0.0;
    }
    // Create FFT object
    p_fft_inst = fft_create
    (
    	XPAR_GPIO_0_DEVICE_ID,
    	XPAR_AXIDMA_0_DEVICE_ID,
		XPAR_PSU_ACPU_GIC_DEVICE_ID, 				//XPAR_PS7_SCUGIC_0_DEVICE_ID,
		XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR, 	//XPAR_FABRIC_CTRL_AXI_DMA_0_S2MM_INTROUT_INTR,
		XPAR_FABRIC_AXI_DMA_0_MM2S_INTROUT_INTR 	//XPAR_FABRIC_CTRL_AXI_DMA_0_MM2S_INTROUT_INTR
    );
    if (p_fft_inst == NULL)
    {
    	xil_printf("ERROR! Failed to create FFT instance.\n\r");
    	return -1;
    }

    // Allocate data buffers
    stim_buf = (cplx_data_t*) malloc(sizeof(cplx_data_t)*FFT_MAX_NUM_PTS);
    if (stim_buf == NULL)
    {
    	xil_printf("ERROR! Failed to allocate memory for the stimulus buffer.\n\r");
    	return -1;
    }

    result_buf = (cplx_data_t*) malloc(sizeof(cplx_data_t)*FFT_MAX_NUM_PTS);
    if (result_buf == NULL)
    {
    	xil_printf("ERROR! Failed to allocate memory for the result buffer.\n\r");
    	return -1;
    }

    // Fill stimulus buffer with some signal
    memcpy(stim_buf, sig_two_sine_waves, sizeof(cplx_data_t)*FFT_MAX_NUM_PTS);

    // Main control loop
    while (1)
    {

    	// Get command
    	xil_printf("What would you like to do?\n\r");
    	xil_printf("0: Print current FFT parameters\n\r");
    	xil_printf("1: Change FFT parameters\n\r");
    	xil_printf("2: Perform FFT using current parameters\n\r");
    	xil_printf("3: Print current stimulus to be used for the FFT operation\n\r");
    	xil_printf("4: Print results of previous FFT operation\n\r");
    	xil_printf("5: Quit\n\r");
    	c = XUartPs_RecvByte(XPAR_PSU_UART_0_BASEADDR);

    	if (c == '0')
    	{
    		xil_printf("---------------------------------------------\n\r");
    		fft_print_params(p_fft_inst);
    		xil_printf("---------------------------------------------\n\r");
    	}
    	else if (c == '1')
    	{
    		which_fft_param(p_fft_inst);
    	}
    	else if (c == '2') // Run FFT
		{
			// Make sure the buffer is clear before we populate it (this is generally not necessary and wastes time doing memory accesses, but for proving the DMA working, we do it anyway)
			memset(result_buf, 0, sizeof(cplx_data_t)*FFT_MAX_NUM_PTS);

			status = fft(p_fft_inst, (cplx_data_t*)stim_buf, (cplx_data_t*)result_buf);
			if (status != FFT_SUCCESS)
			{
				xil_printf("ERROR! FFT failed.\n\r");
				return -1;
			}

			xil_printf("FFT complete!\n\r");
		}
    	else if (c == '3')
    	{
    		fft_print_stim_buf(p_fft_inst);
    	}
    	else if (c == '4')
    	{
    		fft_print_result_buf(p_fft_inst);
    	}
    	else if (c == '5') // Quit
    	{
    		xil_printf("Okay, exiting...\n\r");
    		break;
    	}
    	else
    	{
    		xil_printf("Invalid character. Please try again.\n\r");
    	}

    }

    free(stim_buf);
    free(result_buf);
    fft_destroy(p_fft_inst);

    return 0;

}

void which_fft_param(fft_t* p_fft_inst)
{
	// Local variables
	char c;

	xil_printf("Okay, which parameter would you like to change?\n\r");
	xil_printf("0: Point length\n\r");
	xil_printf("1: Direction\n\r");
	xil_printf("2: Exit\n\r");
	while (1)
	{
		c = XUartPs_RecvByte(XPAR_PSU_UART_0_BASEADDR);

		if (c == '0')
		{
			xil_printf("What would you like to set the FFT point length to? Type:\n\r");
			xil_printf("0: FFT point length = 16\n\r");
			xil_printf("1: FFT point length = 32\n\r");
			xil_printf("2: FFT point length = 64\n\r");
			xil_printf("3: FFT point length = 128\n\r");
			xil_printf("4: FFT point length = 256\n\r");
			xil_printf("5: FFT point length = 512\n\r");
			xil_printf("6: FFT point length = 1024\n\r");
			xil_printf("7: FFT point length = 2048\n\r");
			xil_printf("8: FFT point length = 4096\n\r");
			xil_printf("9: FFT point length = 8192\n\r");

			c = XUartPs_RecvByte(XPAR_PSU_UART_0_BASEADDR);
			while (1)
			{
				if (c == '0')
				{
					xil_printf("Okay, setting the core to perform a 16-point FFT.\n\r");
					fft_set_num_pts(p_fft_inst, 16);
					break;
				}
				else if (c == '1')
				{
					xil_printf("Okay, setting the core to perform a 32-point FFT.\n\r");
					fft_set_num_pts(p_fft_inst, 32);
					break;
				}
				else if (c == '2')
				{
					xil_printf("Okay, setting the core to perform a 64-point FFT.\n\r");
					fft_set_num_pts(p_fft_inst, 64);
					break;
				}
				else if (c == '3')
				{
					xil_printf("Okay, setting the core to perform a 128-point FFT.\n\r");
					fft_set_num_pts(p_fft_inst, 128);
					break;
				}
				else if (c == '4')
				{
					xil_printf("Okay, setting the core to perform a 256-point FFT.\n\r");
					fft_set_num_pts(p_fft_inst, 256);
					break;
				}
				else if (c == '5')
				{
					xil_printf("Okay, setting the core to perform a 512-point FFT.\n\r");
					fft_set_num_pts(p_fft_inst, 512);
					break;
				}
				else if (c == '6')
				{
					xil_printf("Okay, setting the core to perform a 1024-point FFT.\n\r");
					fft_set_num_pts(p_fft_inst, 1024);
					break;
				}
				else if (c == '7')
				{
					xil_printf("Okay, setting the core to perform a 2048-point FFT.\n\r");
					fft_set_num_pts(p_fft_inst, 2048);
					break;
				}
				else if (c == '8')
				{
					xil_printf("Okay, setting the core to perform a 4096-point FFT.\n\r");
					fft_set_num_pts(p_fft_inst, 4096);
					break;
				}
				else if (c == '9')
				{
					xil_printf("Okay, setting the core to perform a 8192-point FFT.\n\r");
					fft_set_num_pts(p_fft_inst, 8192);
					break;
				}
				else
		    		xil_printf("Invalid character. Please try again.\n\r");
			}
			break;
		}
		else if (c == '1')
		{

			xil_printf("What would you like to set the FFT direction to? Type:\n\r");
			xil_printf("0: Inverse\n\r");
			xil_printf("1: Forward\n\r");

			c = XUartPs_RecvByte(XPAR_PSU_UART_0_BASEADDR);
			while (1)
			{
				if (c == '0')
				{
					xil_printf("Okay, setting the core to perform an inverse FFT.\n\r");
					fft_set_fwd_inv(p_fft_inst, FFT_INVERSE);
					break;
				}
				else if (c == '1')
				{
					xil_printf("Okay, setting the core to perform a forward FFT.\n\r");
					fft_set_fwd_inv(p_fft_inst, FFT_FORWARD);
					break;
				}
				else
		    		xil_printf("Invalid character. Please try again.\n\r");
			}
			break;
		}
		else if (c == '2')
		{
			return;
		}
		else
		{
			xil_printf("Invalid character. Please try again.\n\r");
		}
	}
}

