
//////////////////////////////////////////////////////////////////////////////////
//
// Company:        Xilinx
// Engineer:       bwiec
// Create Date:    29 June 2015, 02:59:27 PM
// Library Name:   AXI DMA Accelerator
// File Name:      dma_accel.h
// Target Devices: Zynq
// Tool Versions:  2015.1
// Description:    Middleware API for a DMA Accelerator using the AXI DMA.
// Dependencies:
//   - xaxidma.h - Driver version v8.1
//   - xscugic.h - Driver version v3.0
// Revision History:
//   - v1.0
//     * Initial release
//     * Tested on ZC702 and Zedboard with FFT accelerator
// Additional Comments:
//   - This library is intended to be used as a layer between the base xaxidma
//     driver and application software. The programming model is an
//     algorithm-agnostic DMA accelerator which takes stimulus and result buffers
//     as arguments and transmits data from the stimulus buffer (presumably in
//     main memory), through some sort of hardware accelerator, and back into
//     memory for consumption by the processor or some other module.
//   - While this library is designed targeting Zynq, since it is a soft IP, it
//     could be easily extended to support Microblaze as well. The only difference
//     is that you would need an AXI Interrupt controller in fabric instead of
//     using the GIC in the PS (which is what is currently used).
//   - Source and destination buffers must be contiguous. Scatter gather is not
//     supported.
//   - S2MM and MM2S tdata widths on the AXI DMA must be the same.
//
//////////////////////////////////////////////////////////////////////////////////

#ifndef DMA_ACCEL_H_
#define DMA_ACCEL_H_

// Hardware-specific parameters
#define DMA_ACCEL_IS_CACHE_COHERENT 0 // Set to 1 to avoid overhead of software cache flushes if going through the ACP

// Return types
#define DMA_ACCEL_SUCCESS           0
#define DMA_ACCEL_DMA_INIT_FAIL    -1
#define DMA_ACCEL_INTC_INIT_FAIL   -2
#define DMA_ACCEL_XFER_FAIL        -3

// Object forward declaration
typedef struct dma_accel dma_accel_t;

// Public functions

//
// dma_accel_create - Create a DMA Accelerator object.
//
//  Arguments
//    - dma_device_id:     Device ID of the DMA instance to use.
//    - intc_device_id:    Device ID of the Interrupt Controller instance to use.
//    - s2mm_intr_id:      Interrupt ID for the AXI DMA S2MM channel.
//    - mm2s_intr_id:      Interrupt ID for the AXI DMA MM2S channel.
//    - sample_size_bytes: Number of bytes per sample (i.e. number of bytes in tdata bus).
//
//  Return
//    - dma_accel_t*:      Non-NULL pointer to dma_accel_t object on success.
//    - NULL:              NULL if something failed.
//
dma_accel_t* dma_accel_create(int dma_device_id, int intc_device_id, int s2mm_intr_id,
		                      int mm2s_intr_id, int sample_size_bytes);

//
// dma_accel_destroy - Destroy DMA Accelerator object.
//
//  Arguments
//    - p_dma_accel_inst: Pointer to dma_accel_t object to be deallocated.
//
void dma_accel_destroy(dma_accel_t* p_dma_accel_inst);

//
// dma_accel_set_stim_buf - Set pointer to stimulus buffer to be used.
//
//  Arguments
//    - p_dma_accel_inst: Pointer to dma_accel_t object.
//    - p_stim_buf:       Pointer to stimulus buffer to be sent out by the DMA.
//
void dma_accel_set_stim_buf(dma_accel_t* p_dma_accel_inst, void* p_stim_buf);

//
// dma_accel_get_stim_buf - Get a pointer to the stimulus buffer.
//.
//  Arguments
//    - p_dma_accel_inst: Pointer to dma_accel_t object.
//
//  Return
//    - void*:            Pointer to the stimulus buffer to be sent out by the DMA.
//
void* dma_accel_get_stim_buf(dma_accel_t* p_dma_accel_inst);

//
// dma_accel_set_result_buf - Set pointer to result buffer to be used.
//
//  Arguments
//    - p_dma_accel_inst: Pointer to dma_accel_t object.
//    - p_stim_buf:       Pointer to result buffer to be filled by the DMA.
//
void dma_accel_set_result_buf(dma_accel_t* p_dma_accel_inst, void* p_result_buf);

//
// dma_accel_get_result_buf - Get a pointer to the result buffer.
//.
//  Arguments
//    - p_dma_accel_inst: Pointer to dma_accel_t object.
//
//  Return
//    - void*:            Pointer to the result buffer to be filled by the DMA.
//
void* dma_accel_get_result_buf(dma_accel_t* p_dma_accel_inst);

//
// dma_accel_set_buf_length - Set the buffer length (in samples) to use for DMA transfers.
//
//    - p_dma_accel_inst: Pointer to dma_accel_t object.
//    - buf_length:       Buffer length (in samples) to use for DMA transfers.
//
void dma_accel_set_buf_length(dma_accel_t* p_dma_accel_inst, int buf_length);

//
// dma_accel_get_buf_length - Get the buffer length (in samples) to be used for DMA transfers.
//
//  Arguments
//    - p_dma_accel_inst: Pointer to dma_accel_t object.
//
//  Return
//    - int:              Buffer length (in samples) to be transferred by the DMA.
//
int dma_accel_get_buf_length(dma_accel_t* p_dma_accel_inst);

//
// dma_accel_set_sample_size_bytes - Set the size (in bytes) of each sample (i.e. number
//                                   of bytes in tdata bus).
//
//  Arguments
//    - p_dma_accel_inst:  Pointer to dma_accel_t object.
//    - sample_size_bytes: Number of bytes per sample.
//
void dma_accel_set_sample_size_bytes(dma_accel_t* p_dma_accel_inst, int sample_size_bytes);

//
// dma_accel_get_sample_size_bytes - Get the size (in bytes) of each sample (i.e. number
//                                   of bytes in tdata bus).
//
//  Arguments
//    - p_dma_accel_inst: Pointer to dma_accel_t object.
//
//  Return
//    - int:              Number of bytes per sample.
int dma_accel_get_sample_size_bytes(dma_accel_t* p_dma_accel_inst);

//
// dma_accel_xfer - Perform a data transfer via DMA.
//
//  Arguments
//    - p_dma_accel_inst:    Pointer to dma_accel_t object.
//
//  Return
//    - DMA_SUCCESS:         No errors occurred during any DMA transfers.
//    - DMA_ACCEL_XFER_FAIL: Some error occurred during one of the DMA transfers.
//
int dma_accel_xfer(dma_accel_t* p_dma_accel_inst);

#endif /* DMA_ACCEL_H_ */

