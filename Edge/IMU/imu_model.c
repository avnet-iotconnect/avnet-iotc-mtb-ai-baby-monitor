/*
* ImagiNet Compiler 4.0.258
* Copyright (c) 2019- [Imagimob AB], All Rights Reserved.
* 
* Generated at 08/27/2023 00:37:02 UTC. Any changes will be lost.
* 
* Model ID  32f52be3-1ada-4b21-b842-a3fd4034713a
* 
* Memory    Size                      Efficiency
* State     808 bytes (RAM)           100 %
* 
* Backend              tensorflow
* Keras Version        2.10.0
* Backend Model Type   Sequential
* Backend Model Name   conv1d-medium-balanced-2
* 
* Class Index | Symbol Label
* 0           | unlabelled
* 1           | standing
* 2           | running
* 3           | walking
* 4           | sitting
* 5           | jumping
* 
* Layer                          Shape           Type       Function
* Sliding Window (data points)   [50,3]          float      dequeue
*    window_shape = [50,3]
*    stride = 30
*    buffer_multiplier = 2
* 
* Exported functions:
* 
* int IMAI_IMU_dequeue(float *restrict data_out)
*    Description: Dequeue features. RET_SUCCESS (0) on success, RET_NODATA (-1) if no data is available, RET_NOMEM (-2) on internal memory error
*    Parameter data_out is Output of size float[50,3].
* 
* int IMAI_IMU_enqueue(const float *restrict data_in)
*    Description: Enqueue features. Returns SUCCESS (0) on success, else RET_NOMEM (-2) when low on memory.
*    Parameter data_in is Input of size float[3].
* 
* void IMAI_IMU_init(void)
*    Description: Initializes buffers to initial state. This function also works as a reset function.
* 
* 
* Disclaimer:
*   The generated code relies on the optimizations done by the C compiler.
*   For example many for-loops of length 1 must be removed by the optimizer.
*   This can only be done if the functions are inlined and simplified.
*   Check disassembly if unsure.
*   tl;dr Compile using gcc with -O3 or -Ofast
*/

/*
* Tensorflow Test Set
* 
* (ACC) Accuracy 93.051 %
* (F1S) F1 Score 92.634 %
* 
* Name of class                                            (unlabelled)          jumping          running          sitting         standing          walking
* (TP) True Positive or Correct Positive Prediction                 151              478              924             1622             1190             1246
* (FN) False Negative or Incorrect Negative Prediction              182              146                0                0               83                8
* (FP) False Positive or Incorrect Positive Prediction               59              111              162                4               37               46
* (TN) True Negative or Correct Negative Prediction                5638             5295             4944             4404             4720             4730
* (TPR) True Positive Rate or Sensitivity, Recall               45.35 %          76.60 %         100.00 %         100.00 %          93.48 %          99.36 %
* (TNR) True Negative Rate or Specificity, Selectivity          98.96 %          97.95 %          96.83 %          99.91 %          99.22 %          99.04 %
* (PPV) Positive Predictive Value or Precision                  71.90 %          81.15 %          85.08 %          99.75 %          96.98 %          96.44 %
* (NPV) Negative Predictive Value                               96.87 %          97.32 %         100.00 %         100.00 %          98.27 %          99.83 %
* (FNR) False Negative Rate or Miss Rate                        54.65 %          23.40 %           0.00 %           0.00 %           6.52 %           0.64 %
* (FPR) False Positive Rate or Fall-Out                          1.04 %           2.05 %           3.17 %           0.09 %           0.78 %           0.96 %
* (FDR) False Discovery Rate                                    28.10 %          18.85 %          14.92 %           0.25 %           3.02 %           3.56 %
* (FOR) False Omission Rate                                      3.13 %           2.68 %           0.00 %           0.00 %           1.73 %           0.17 %
* (F1S) F1 Score                                                55.62 %          78.81 %          91.94 %          99.88 %          95.20 %          97.88 %
*/

#include <stdint.h>
#include <string.h>

#include "imu_model.h"

// Working memory
static int8_t _state[808];

// Memory mapped buffers
#define _K3              ((int8_t *)(_state + 0x00000000))   // s8[808] (808 bytes) 

// Represents a Circular Buffer
// https://en.wikipedia.org/wiki/Circular_buffer
typedef struct
{
    char *buf;
    int size;        // total bytes allocated in *buf
    int used;        // current bytes used in buffer.
    int read;
    int write;
} cbuffer_t;

#define CBUFFER_SUCCESS 0
#define CBUFFER_NOMEM -1

// Initializes a cbuffer handle with given memory and size.
static inline void cbuffer_init(cbuffer_t *dest, void *mem, int size) {
    dest->buf = mem;
    dest->size = size;
    dest->used = 0;
    dest->read = 0;
    dest->write = 0;
}

// Returns the number of free bytes in buffer.
static inline int cbuffer_get_free(cbuffer_t *buf) {
    return buf->size - buf->used;
}

// Returns the number of used bytes in buffer.
static inline int cbuffer_get_used(cbuffer_t *buf) {
    return buf->used;
}

// Writes given data to buffer.
// Returns CBUFFER_SUCCESS or CBUFFER_NOMEM if out of memory.
static inline int cbuffer_enqueue(cbuffer_t *buf, const void *data, int data_size) {
    int free = cbuffer_get_free(buf);

    // Out of memory?
    if (free < data_size)
        return CBUFFER_NOMEM;

    // Is the data split in the end?
    if (buf->write + data_size > buf->size) {
        int first_size = buf->size - buf->write;
        memcpy(buf->buf + buf->write, data, first_size);
        memcpy(buf->buf, ((char *)data) + first_size, data_size - first_size);
    }
    else {
        memcpy(buf->buf + buf->write, data, data_size);
    }
    buf->write += data_size;
    if (buf->write >= buf->size)
        buf->write -= buf->size;

    buf->used += data_size;
    return CBUFFER_SUCCESS;
}

// Advances the read pointer by given count.
// Returns CBUFFER_SUCCESS on success or CBUFFER_NOMEM if count is more than available data
static inline int cbuffer_advance(cbuffer_t *buf, int count) {
    int used = cbuffer_get_used(buf);

    if (count > used)
        return CBUFFER_NOMEM;

    buf->read += count;
    if (buf->read >= buf->size)
        buf->read -= buf->size;

    // Reset pointers to 0 if buffer is empty in order to avoid unwanted wrapps.
    if (buf->read == buf->write) {
        buf->read = 0;
        buf->write = 0;
    }

    buf->used -= count;
    return CBUFFER_SUCCESS;
}

// Reset instance (clear buffer)
static inline void cbuffer_reset(cbuffer_t *buf) {
    buf->read = 0;
    buf->write = 0;
    buf->used = 0;
}

// Copies given "count" bytes to the "dst" buffer without advancing the buffer read offset.
// Returns CBUFFER_SUCCESS on success or CBUFFER_NOMEM if count is more than available data.
static inline int cbuffer_copyto(cbuffer_t *buf, void *dst, int count, int offset) {
    
    if (count > cbuffer_get_used(buf))
        return CBUFFER_NOMEM;

    int a0 = buf->read + offset;
    if (a0 >= buf->size)
        a0 -= buf->size;

    int c0 = count;
    if (a0 + c0 > buf->size)
        c0 = buf->size - a0;
    
    memcpy(dst, buf->buf + a0, c0);
    
    int c1 = count - c0;

    if (c1 > 0)
        memcpy(((char *)dst) + c0, buf->buf, c1);

    return CBUFFER_SUCCESS;
}

// Returns a read pointer at given offset and  
// updates *can_read_bytes (if not NULL) with the number of bytes that can be read.
// 
// Note! Byte count written to can_read_bytes can be less than what cbuffer_get_used() returns.
// This happens when the read has to be split in two since it's a circular buffer.
static inline void *cbuffer_readptr(cbuffer_t* buf, int offset, int *can_read_bytes)
{
    int a0 = buf->read + offset;
    if (a0 >= buf->size)
        a0 -= buf->size;
    if (can_read_bytes != NULL)
    {
        int c0 = buf->used;
        if (a0 + c0 > buf->size)
            c0 = buf->size - a0;

        *can_read_bytes = c0;
    }
    return buf->buf + a0;
}

typedef struct {
    cbuffer_t data_buffer;            // Circular Buffer for features
    int input_size;                    // Number of bytes in each input chunk
    int window_count;                // Number of input chunks in output window.
} fixwin_t;

#ifdef _MSC_VER
static_assert(sizeof(fixwin_t) <= 64, "Data structure 'fixwin_t' is too big");
#endif

#define IPWIN_RET_SUCCESS 0
#define IPWIN_RET_NODATA -1
#define IPWIN_RET_NOMEM -2

/*
* Try to dequeue a window.
*
* @param handle Pointer to an initialized handle.
* @param dst Pointer where to write window.
* @param stride_count Number of items (of size handle->input_size) to stride window.
* @return IPWIN_RET_SUCCESS (0) or IPWIN_RET_NODATA (-1) is no data is available.
*/
static inline int fixwin_dequeuef32(void* restrict handle, void* restrict dst, int stride_count)
{
    fixwin_t* fep = (fixwin_t*)handle;

    const int stride_bytes = stride_count * fep->input_size;
    const int size = fep->window_count * fep->input_size;
    if (cbuffer_get_used(&fep->data_buffer) >= size) {
        if (cbuffer_copyto(&fep->data_buffer, dst, size, 0) != 0)
            return IPWIN_RET_NOMEM;

        if (cbuffer_advance(&fep->data_buffer, stride_bytes) != 0)
            return IPWIN_RET_NOMEM;

        return IPWIN_RET_SUCCESS;
    }
    return IPWIN_RET_NODATA;
}

/**
 * Enqueue handle->input_size values from given *data pointer to internal window buffer.
 *
 * @param handle Pointer to an initialized handle.
 * @param data Data to enqueue.
 * @return IPWIN_RET_SUCCESS (0) or IPWIN_RET_NOMEM (-2) if internal buffer is out of memory.
 */
static inline int fixwin_enqueuef32(void* restrict handle, const void* restrict data)
{
    fixwin_t* fep = (fixwin_t*)handle;

    if (cbuffer_enqueue(&fep->data_buffer, data, fep->input_size) != 0)
        return IPWIN_RET_NOMEM;

    return IPWIN_RET_SUCCESS;
}

/**
* Initializes a fixwin sampler handle.
*
* @param handle Pointer to a preallocated memory area of fixwin_handle_size() bytes to initialize.
*
* @param input_size Number of bytes to enqueue.
* @param window_count Number of items (of size input_size) in each window
*/
static inline void fixwin_initf32(void* restrict handle, int input_size, int window_count)
{
    fixwin_t* fep = (fixwin_t*)handle;
    fep->input_size = input_size;
    fep->window_count = window_count;

    char* mem = ((char*)handle) + sizeof(fixwin_t);

    int data_buffer = input_size * window_count;
    
    cbuffer_init(&fep->data_buffer, mem, data_buffer);
}

#define __RETURN_ERROR(_exp) do { int __ret = (_exp); if(__ret < 0) return __ret; } while(0)
#define __RETURN_ERROR_BREAK_EMPTY(_exp) {  int __ret = (_exp); if(__ret == -1) break; if(__ret < 0) return __ret;  } 

int IMAI_IMU_dequeue(float *restrict data_out) {    
    __RETURN_ERROR(fixwin_dequeuef32(_K3, data_out, 10));
    return 0;
}

int IMAI_IMU_enqueue(const float *restrict data_in) {    
    __RETURN_ERROR(fixwin_enqueuef32(_K3, data_in));
    return 0;
}

void IMAI_IMU_init(void) {    
    fixwin_initf32(_K3, 12, 50);
}

