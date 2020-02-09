
//////////////////////////////////////////////////////////////////////////////////
//
// Company:        Xilinx
// Engineer:       bwiec
// Create Date:    29 June 2015, 03:22:41 PM
// Library Name:   Fast Fourier Transform
// File Name:      fft.h
// Target Devices: Zynq
// Tool Versions:  2015.1
// Description:    Implementation of FFT using a hardware accelerator with DMA.
// Dependencies:
//   - cplx_data.h - Driver version v1.0
//   - dma_accel.h - Driver version v1.0
//   - gpio.h      - Driver version v4.0
// Revision History:
//   - v1.0
//     * Initial release
//     * Tested on ZC702 and Zedboard
// Additional Comments:
//   - GPIO is used with some additional glue logic to control the FFT core's
//     config interface for setting various parameters.
//
//////////////////////////////////////////////////////////////////////////////////

#ifndef FFT_H_
#define FFT_H_

// Includes
#include "cplx_data.h"
#include "dma_accel.h"

// Parameter macros
#define FFT_ARCH_PIPELINED   0
#define FFT_ARCH_RADIX4      1
#define FFT_ARCH_RADIX2      2
#define FFT_ARCH_RADIX2_LITE 3

// Hardware-specific parameters
#define FFT_MAX_NUM_PTS      8192
#define FFT_NUM_PTS_MASK     0x0000001F // Bits [4:0]
#define FFT_NUM_PTS_SHIFT    0
#define FFT_FWD_INV_MASK     0x00000100 // Bit 8
#define FFT_FWD_INV_SHIFT    8
#define FFT_SCALE_SCH_MASK   0x007FFE00 // Bits [22:9]
#define FFT_SCALE_SCH_SHIFT  9

// Return types
#define FFT_SUCCESS          0
#define FFT_GPIO_INIT_FAIL  -1
#define FFT_ILLEGAL_NUM_PTS -2
#define FFT_DMA_FAIL        -3

// Enumerated data types
typedef enum
{
	FFT_INVERSE = 0,
	FFT_FORWARD = 1
} fft_fwd_inv_t;

// Object forward declaration
typedef struct fft fft_t;

// Public functions

//
// dma_accel_create - Create a FFT object.
//
//  Arguments
//    - gpio_device_id: Device ID of the GPIO instance to use.
//    - dma_device_id:  Device ID of the DMA instance to use.
//    - intc_device_id: Device ID of the Interrupt Controller instance to use.
//    - s2mm_intr_id:   Interrupt ID for the AXI DMA S2MM channel.
//    - mm2s_intr_id:   Interrupt ID for the AXI DMA MM2S channel.
//
//  Return
//    - fft*:           Non-NULL pointer to fft_t object on success.
//    - NULL:           NULL if something failed.
//
fft_t* fft_create(int gpio_device_id, int dma_device_id, int intc_device_id, int s2mm_intr_id, int mm2s_intr_id);

//
// fft_destroy - Destroy FFT object.
//
//  Arguments
//    - p_fft_inst: Pointer to fft_t object to be deallocated.
//
void fft_destroy(fft_t* p_fft_inst);

//
// fft_set_fwd_inv - Set the whether to do forward or inverse FFT.
//
//  Arguments
//    - p_fft_inst: Pointer to the fft_t object.
//    - fwd_inv:    Whether to do forward or inverse FFT.
//
void fft_set_fwd_inv(fft_t* p_fft_inst, fft_fwd_inv_t fwd_inv);

//
// fft_get_fwd_inv - Get the current setting for whether to do forward or
//                   inverse FFT.
//
//  Arguments
//    - p_fft_inst:    Pointer to the fft_t object.
//
//  Return
//    - fft_fwd_inv_t: Whether to do forward or inverse FFT.
//
fft_fwd_inv_t fft_get_fwd_inv(fft_t* p_fft_inst);

//
// fft_set_num_pts - Set the number of points to use for the FFT.
//
//  Arguments
//    - p_fft_inst:          Pointer to the fft_t object.
//    - num_pts:             Number of points to be used for the FFT.
//
//  Return
//    - FFT_SUCCESS:         num_pts is legal and the object is set.
//    - FFT_ILLEGAL_NUM_PTS: num_pts is either not a power of two or it
//                           exceeds FFT_MAX_NUM_PTS per hardware configuration.
//
int fft_set_num_pts(fft_t* p_fft_inst, int num_pts);

//
// fft_get_num_pts - Get the current setting for the number of points to use
//                   for the FFT.
//
//  Arguments
//    - p_fft_inst: Pointer to the fft_t object.
//
//  Return
//    - int:        Number of points to be used for the FFT.
//
int fft_get_num_pts(fft_t* p_fft_inst);

//
// fft_set_scale_sch - Set the scaling schedule to use for the FFT.
//
//  Arguments
//    - p_fft_inst: Pointer to the fft_t object.
//    - scale_sch:  Scaling schedule to be used for the FFT.
//
void fft_set_scale_sch(fft_t* p_fft_inst, int scale_sch);

//
// fft_get_scale_sch - Get the current setting for the scaling schedule to use
//                     for the FFT.
//
//  Arguments
//    - p_fft_inst: Pointer to the fft_t object.
//
//  Return
//    - int:        Scaling schedule to be used for the FFT.
//
int fft_get_scale_sch(fft_t* p_fft_inst);

//
// fft - Fast Fourier Transform.
//
//  Arguments
//    - p_fft_inst:   Pointer to the fft_t object.
//    - din:          Pointer to the input data buffer.
//    - dout:         Pointer to the output data buffer.
//
//  Return
//    - FFT_SUCCESS:  FFT operation completed successfully.
//    - FFT_DMA_FAIL: The DMA accelerator threw an error during FFT computation.
//
int fft(fft_t* p_fft_inst, cplx_data_t* din, cplx_data_t* dout);

// ******************************************************************************
// Public functions for debugging

//
// fft_get_stim_buf - Get the stimulus buffer to be used for the FFT.
//
//  Arguments
//    - p_fft_inst:   Pointer to the fft_t object.
//
//  Return
//    - cplx_data_t*: Stimulus buffer to be used for the FFT.
//
cplx_data_t* fft_get_stim_buf(fft_t* p_fft_inst);

//
// fft_get_result_buf - Get the result buffer used by the FFT.
//
//  Arguments
//    - p_fft_inst: Pointer to the fft_t object.
//
//  Return
//    - cplx_data_t*: Result buffer used by the FFT.
//
cplx_data_t* fft_get_result_buf(fft_t* p_fft_inst);

//
// fft_print_params - Print current state of FFT object to the console.
//
//  Arguments
//    - p_fft_inst: Pointer to the fft_t object.
//
void fft_print_params(fft_t* p_fft_inst);

//
// fft_print_stim_buf - Print the contents of the FFT's stimulus buffer.
//
//  Arguments
//    - p_fft_inst: Pointer to the fft_t object.
//
void fft_print_stim_buf(fft_t* p_fft_inst);

//
// fft_print_result_buf - Print the contents of the FFT's result buffer.
//
//  Arguments
//    - p_fft_inst: Pointer to the fft_t object.
//
void fft_print_result_buf(fft_t* p_fft_inst);

#endif /* FFT_H_ */

