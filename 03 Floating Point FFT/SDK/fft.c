
// Includes
#include <stdlib.h>
#include "fft.h"
#include "xgpio.h"

// Private data
typedef struct fft_periphs
{
	dma_accel_t* p_dma_accel_inst;
	XGpio        gpio_inst;
} fft_periphs_t;

// Object definition
typedef struct fft
{
	fft_periphs_t periphs;
	fft_fwd_inv_t fwd_inv;
	int           num_pts;
	int           scale_sch;
} fft_t;

// Private functions
static int is_power_of_2(int x)
// Determines if the input integer is a power of two.
{
	while ((x % 2 == 0) && (x > 1))
	{
		x /= 2;
	}

	return (x == 1);
}

static int int_log2(int x)
// Computes the log2 of the input integer. Remainder is floored.
{

	int log2_N = 0;

	x--;
	for (log2_N = 0;  x > 0; log2_N++)
	{
		x >>= 1;
	}

	return log2_N;

}

static int init_gpio(XGpio* p_gpio_inst, int gpio_device_id)
{

	// Local variables
	int status = 0;

	// Initialize driver
	status = XGpio_Initialize(p_gpio_inst, gpio_device_id);
	if (status != XST_SUCCESS)
	{
		xil_printf("ERROR! Initialization of AXI GPIO instance failed.\n\r");
		return FFT_GPIO_INIT_FAIL;
	}

	return FFT_SUCCESS;

}

static void fft_commit_params(fft_t* p_fft_inst)
{

	// Local variables
	int reg = 0;

	reg  = (p_fft_inst->scale_sch         << FFT_SCALE_SCH_SHIFT) & FFT_SCALE_SCH_MASK;
	reg |= (p_fft_inst->fwd_inv           << FFT_FWD_INV_SHIFT)   & FFT_FWD_INV_MASK;
	reg |= (int_log2(p_fft_inst->num_pts) << FFT_NUM_PTS_SHIFT)   & FFT_NUM_PTS_MASK;
	xil_printf("writing configuration: %8X\r\n", reg);
	XGpio_DiscreteWrite(&p_fft_inst->periphs.gpio_inst, 1, reg);

}

// Public functions
fft_t* fft_create(int gpio_device_id, int dma_device_id, int intc_device_id, int s2mm_intr_id, int mm2s_intr_id)
{

	// Local variables
	fft_t* p_obj;
	int   status;

	// Allocate memory for FFT object
	p_obj = (fft_t*) malloc(sizeof(fft_t));
	if (p_obj == NULL)
	{
		xil_printf("ERROR! Failed to allocate memory for FFT object.\n\r");
		return NULL;
	}

	// Create DMA Accelerator object to be used by the FFT
	p_obj->periphs.p_dma_accel_inst = dma_accel_create
	(
		dma_device_id,
		intc_device_id,
		s2mm_intr_id,
		mm2s_intr_id,
		sizeof(cplx_data_t)
	);
	if (p_obj->periphs.p_dma_accel_inst == NULL)
	{
		xil_printf("ERROR! Failed to create DMA Accelerator object for use by the FFT engine.\n\r");
		free(p_obj);
		return NULL;
	}

	// Register and initialize peripherals
	status = init_gpio(&p_obj->periphs.gpio_inst, gpio_device_id);
	if (status != FFT_SUCCESS)
	{
		xil_printf("ERROR! Failed to initialize GPIO.\n\r");
		fft_destroy(p_obj);
		return NULL;
	}

	// Initialize FFT parameters
	fft_set_fwd_inv(p_obj, FFT_FORWARD);
	status = fft_set_num_pts(p_obj, 1024);
	if (status != FFT_SUCCESS)
	{
		xil_printf("ERROR! Failed to initialize the number of points in the FFT.\n\r");
		fft_destroy(p_obj);
		return NULL;
	}
	fft_set_scale_sch(p_obj, 0x2AB);

	return p_obj;

}

void fft_destroy(fft_t* p_fft_inst)
{
	dma_accel_destroy(p_fft_inst->periphs.p_dma_accel_inst);
	free(p_fft_inst);
}

void fft_set_fwd_inv(fft_t* p_fft_inst, fft_fwd_inv_t fwd_inv)
{
	p_fft_inst->fwd_inv = fwd_inv;
}

fft_fwd_inv_t fft_get_fwd_inv(fft_t* p_fft_inst)
{
	return (p_fft_inst->fwd_inv);
}

int fft_set_num_pts(fft_t* p_fft_inst, int num_pts)
{
	if (num_pts > FFT_MAX_NUM_PTS)
	{
		xil_printf("ERROR! Attempted to set too large number of points in the FFT.\n\r");
		return FFT_ILLEGAL_NUM_PTS;
	}
	else if (!is_power_of_2(num_pts))
	{
		xil_printf("ERROR! Attempted to set a non-power-of-2 value for the number of points in the FFT.\n\r");
		return FFT_ILLEGAL_NUM_PTS;
	}
	else
	{
		p_fft_inst->num_pts = num_pts;
		dma_accel_set_buf_length(p_fft_inst->periphs.p_dma_accel_inst, p_fft_inst->num_pts);
		return FFT_SUCCESS;
	}
}

int fft_get_num_pts(fft_t* p_fft_inst)
{
	return (p_fft_inst->num_pts);
}

void fft_set_scale_sch(fft_t* p_fft_inst, int scale_sch)
{
	p_fft_inst->scale_sch = scale_sch;
}

int fft_get_scale_sch(fft_t* p_fft_inst)
{
	return (p_fft_inst->scale_sch);
}

int fft(fft_t* p_fft_inst, cplx_data_t* din, cplx_data_t* dout)
{
	// Local variables
	int status = 0;

	xil_printf("in fft function\r\n");
	// Commit struct parameters to hardware
	fft_commit_params(p_fft_inst);

	// Set buffers for DMA
	xil_printf("setting dma buffers\r\n");
	dma_accel_set_stim_buf(p_fft_inst->periphs.p_dma_accel_inst, (void*)din);
	dma_accel_set_result_buf(p_fft_inst->periphs.p_dma_accel_inst, (void*)dout);

	// Perform DMA transfer
	status = dma_accel_xfer(p_fft_inst->periphs.p_dma_accel_inst);
	if (status != DMA_ACCEL_SUCCESS)
	{
		xil_printf("ERROR! DMA transfer failed.\n\r");
		return FFT_DMA_FAIL;
	}

	return FFT_SUCCESS;
}

cplx_data_t* fft_get_stim_buf(fft_t* p_fft_inst)
{
	return (cplx_data_t*)dma_accel_get_stim_buf(p_fft_inst->periphs.p_dma_accel_inst);
}

cplx_data_t* fft_get_result_buf(fft_t* p_fft_inst)
{
	return (cplx_data_t*)dma_accel_get_result_buf(p_fft_inst->periphs.p_dma_accel_inst);
}

void fft_print_params(fft_t* p_fft_inst)
{
	xil_printf("fwd_inv   = %s\n\r", (p_fft_inst->fwd_inv == FFT_FORWARD ? "forward" : "inverse"));
	xil_printf("num_pts   = %d\n\r", p_fft_inst->num_pts);
	xil_printf("scale_sch = 0x%X\n\r", p_fft_inst->scale_sch);
}

void fft_print_stim_buf(fft_t* p_fft_inst)
{

	// Local variables
	int          ii = 0;
	char         str[25]; // Large enough to hold 2 ints plus extra characters
	cplx_data_t* tmp;

	tmp = (cplx_data_t*)dma_accel_get_stim_buf(p_fft_inst->periphs.p_dma_accel_inst);

	for (ii = 0; ii < p_fft_inst->num_pts; ii++)
	{
		cplx_data_get_string(str, tmp[ii]);
		xil_printf("Xn(%4d) = %s\n\r", ii, str);
	}

}

void fft_print_result_buf(fft_t* p_fft_inst)
{

	// Local variables
	int          ii = 0;
	char         str[25]; // Large enough to hold 2 ints plus extra characters
	cplx_data_t* tmp;

	tmp = (cplx_data_t*)dma_accel_get_result_buf(p_fft_inst->periphs.p_dma_accel_inst);

	for (ii = 0; ii < p_fft_inst->num_pts; ii++)
	{
		cplx_data_get_string(str, tmp[ii]);
		xil_printf("Xk(%4d) = %s\n\r", ii, str);
	}

}

