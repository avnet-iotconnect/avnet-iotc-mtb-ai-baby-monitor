/*
* ImagiNet Compiler 4.0.258
* Copyright (c) 2019- [Imagimob AB], All Rights Reserved.
* 
* Generated at 08/31/2023 00:04:37 UTC. Any changes will be lost.
* 
* Model ID  110f29cd-9dc4-437a-b0a0-c606d9a61803
* 
* Memory    Size                      Efficiency
* Buffers   6152 bytes (RAM)          100 %
* State     8392 bytes (RAM)          100 %
* Readonly  2136 bytes (Flash)        100 %
* 
* Backend              tensorflow
* Keras Version        2.10.0
* Backend Model Type   Sequential
* Backend Model Name   conv2d-medium-speed-0
* 
* Class Index | Symbol Label
* 0           | unlabelled
* 1           | baby_cry
* 
* Layer                          Shape           Type       Function
* Sliding Window (data points)   [512]           float      dequeue
*    window_shape = [512]
*    stride = 160
*    buffer_multiplier = 1
* Hann smoothing                 [512]           float      dequeue
*    sym = True
* Real Discrete Fourier Transform [257,2]         float      dequeue
*    axis = 0
* Frobenius norm                 [257]           float      dequeue
*    axis = 0
* Mel Filterbank                 [20]            float      dequeue
*    num_filters = 20
*    sample_rate = 16000
*    f_low = 300
*    f_high = 8000
*    htk = True
*    librosa = False
* Clip                           [20]            float      dequeue
*    min = 0.000316227766016
*    max = 3.40282347E+38
* Logarithm                      [20]            float      dequeue
*    base = 0
* Sliding Window (data points)   [60,20]         float      dequeue
*    window_shape = [60,20]
*    stride = 660
*    buffer_multiplier = 1
* 
* Exported functions:
* 
* int IMAI_PDM_dequeue(float *restrict data_out)
*    Description: Dequeue features. RET_SUCCESS (0) on success, RET_NODATA (-1) if no data is available, RET_NOMEM (-2) on internal memory error
*    Parameter data_out is Output of size float[60,20].
* 
* int IMAI_PDM_enqueue(const float *restrict data_in)
*    Description: Enqueue features. Returns SUCCESS (0) on success, else RET_NOMEM (-2) when low on memory.
*    Parameter data_in is Input of size float[1].
* 
* void IMAI_PDM_init(void)
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

#ifndef _IMAI_PDM_PDM_MODEL_H_
#define _IMAI_PDM_PDM_MODEL_H_
#ifdef _MSC_VER
#pragma once
#endif

#include <stdint.h>

typedef struct {    
    char *name;
    double TP; // True Positive or Correct Positive Prediction
    double FN; // False Negative or Incorrect Negative Prediction
    double FP; // False Positive or Incorrect Positive Prediction
    double TN; // True Negative or Correct Negative Prediction
    double TPR; // True Positive Rate or Sensitivity, Recall
    double TNR; // True Negative Rate or Specificity, Selectivity
    double PPV; // Positive Predictive Value or Precision
    double NPV; // Negative Predictive Value
    double FNR; // False Negative Rate or Miss Rate
    double FPR; // False Positive Rate or Fall-Out
    double FDR; // False Discovery Rate
    double FOR; // False Omission Rate
    double F1S; // F1 Score
} IMAI_PDM_stats;

/*
* Tensorflow Test Set
* 
* (ACC) Accuracy 82.630 %
* (F1S) F1 Score 83.927 %
* 
* Name of class                                            (unlabelled)         baby_cry
* (TP) True Positive or Correct Positive Prediction                 618               67
* (FN) False Negative or Incorrect Negative Prediction               98               46
* (FP) False Positive or Incorrect Positive Prediction               46               98
* (TN) True Negative or Correct Negative Prediction                  67              618
* (TPR) True Positive Rate or Sensitivity, Recall               86.31 %          59.29 %
* (TNR) True Negative Rate or Specificity, Selectivity          59.29 %          86.31 %
* (PPV) Positive Predictive Value or Precision                  93.07 %          40.61 %
* (NPV) Negative Predictive Value                               40.61 %          93.07 %
* (FNR) False Negative Rate or Miss Rate                        13.69 %          40.71 %
* (FPR) False Positive Rate or Fall-Out                         40.71 %          13.69 %
* (FDR) False Discovery Rate                                     6.93 %          59.39 %
* (FOR) False Omission Rate                                     59.39 %           6.93 %
* (F1S) F1 Score                                                89.57 %          48.20 %
*/


#define IMAI_PDM_TEST_AVG_ACC 0.8262967430639324 // Accuracy
#define IMAI_PDM_TEST_AVG_F1S 0.8392697013522554 // F1 Score

#define IMAI_PDM_TEST_STATS { \
 {name: "(unlabelled)", TP: 618, FN: 98, FP: 46, TN: 67, TPR: 0.8631284916201, TNR: 0.5929203539823, PPV: 0.9307228915662, NPV: 0.4060606060606, FNR: 0.1368715083798, FPR: 0.4070796460176, FDR: 0.0692771084337, FOR: 0.5939393939393, F1S: 0.8956521739130, }, \
 {name: "baby_cry", TP: 67, FN: 46, FP: 98, TN: 618, TPR: 0.5929203539823, TNR: 0.8631284916201, PPV: 0.4060606060606, NPV: 0.9307228915662, FNR: 0.4070796460176, FPR: 0.1368715083798, FDR: 0.5939393939393, FOR: 0.0692771084337, F1S: 0.4820143884892, }, \
}

#ifdef IMAI_PDM_STATS_ENABLED
static const IMAI_PDM_stats IMAI_PDM_test_stats[] = IMAI_PDM_TEST_STATS;
#endif

/*
* Tensorflow Train Set
* 
* (ACC) Accuracy 97.823 %
* (F1S) F1 Score 97.873 %
* 
* Name of class                                            (unlabelled)         baby_cry
* (TP) True Positive or Correct Positive Prediction               19703             3166
* (FN) False Negative or Incorrect Negative Prediction              444               65
* (FP) False Positive or Incorrect Positive Prediction               65              444
* (TN) True Negative or Correct Negative Prediction                3166            19703
* (TPR) True Positive Rate or Sensitivity, Recall               97.80 %          97.99 %
* (TNR) True Negative Rate or Specificity, Selectivity          97.99 %          97.80 %
* (PPV) Positive Predictive Value or Precision                  99.67 %          87.70 %
* (NPV) Negative Predictive Value                               87.70 %          99.67 %
* (FNR) False Negative Rate or Miss Rate                         2.20 %           2.01 %
* (FPR) False Positive Rate or Fall-Out                          2.01 %           2.20 %
* (FDR) False Discovery Rate                                     0.33 %          12.30 %
* (FOR) False Omission Rate                                     12.30 %           0.33 %
* (F1S) F1 Score                                                98.72 %          92.56 %
*/


#define IMAI_PDM_TRAIN_AVG_ACC 0.9782273932757293 // Accuracy
#define IMAI_PDM_TRAIN_AVG_F1S 0.9787271407874095 // F1 Score

#define IMAI_PDM_TRAIN_STATS { \
 {name: "(unlabelled)", TP: 19703, FN: 444, FP: 65, TN: 3166, TPR: 0.9779619794510, TNR: 0.9798823893531, PPV: 0.9967118575475, NPV: 0.8770083102493, FNR: 0.0220380205489, FPR: 0.0201176106468, FDR: 0.0032881424524, FOR: 0.1229916897506, F1S: 0.9872479017913, }, \
 {name: "baby_cry", TP: 3166, FN: 65, FP: 444, TN: 19703, TPR: 0.9798823893531, TNR: 0.9779619794510, PPV: 0.8770083102493, NPV: 0.9967118575475, FNR: 0.0201176106468, FPR: 0.0220380205489, FDR: 0.1229916897506, FOR: 0.0032881424524, F1S: 0.9255956731472, }, \
}

#ifdef IMAI_PDM_STATS_ENABLED
static const IMAI_PDM_stats IMAI_PDM_train_stats[] = IMAI_PDM_TRAIN_STATS;
#endif

/*
* Tensorflow Validation Set
* 
* (ACC) Accuracy 94.687 %
* (F1S) F1 Score 94.753 %
* 
* Name of class                                            (unlabelled)         baby_cry
* (TP) True Positive or Correct Positive Prediction                5756             1070
* (FN) False Negative or Incorrect Negative Prediction              231              152
* (FP) False Positive or Incorrect Positive Prediction              152              231
* (TN) True Negative or Correct Negative Prediction                1070             5756
* (TPR) True Positive Rate or Sensitivity, Recall               96.14 %          87.56 %
* (TNR) True Negative Rate or Specificity, Selectivity          87.56 %          96.14 %
* (PPV) Positive Predictive Value or Precision                  97.43 %          82.24 %
* (NPV) Negative Predictive Value                               82.24 %          97.43 %
* (FNR) False Negative Rate or Miss Rate                         3.86 %          12.44 %
* (FPR) False Positive Rate or Fall-Out                         12.44 %           3.86 %
* (FDR) False Discovery Rate                                     2.57 %          17.76 %
* (FOR) False Omission Rate                                     17.76 %           2.57 %
* (F1S) F1 Score                                                96.78 %          84.82 %
*/


#define IMAI_PDM_VALIDATION_AVG_ACC 0.9468719655985574 // Accuracy
#define IMAI_PDM_VALIDATION_AVG_F1S 0.9475273127666857 // F1 Score

#define IMAI_PDM_VALIDATION_STATS { \
 {name: "(unlabelled)", TP: 5756, FN: 231, FP: 152, TN: 1070, TPR: 0.9614164022047, TNR: 0.8756137479541, PPV: 0.9742721733243, NPV: 0.8224442736356, FNR: 0.0385835977952, FPR: 0.1243862520458, FDR: 0.0257278266756, FOR: 0.1775557263643, F1S: 0.9678015973097, }, \
 {name: "baby_cry", TP: 1070, FN: 152, FP: 231, TN: 5756, TPR: 0.8756137479541, TNR: 0.9614164022047, PPV: 0.8224442736356, NPV: 0.9742721733243, FNR: 0.1243862520458, FPR: 0.0385835977952, FDR: 0.1775557263643, FOR: 0.0257278266756, F1S: 0.8481965913594, }, \
}

#ifdef IMAI_PDM_STATS_ENABLED
static const IMAI_PDM_stats IMAI_PDM_validation_stats[] = IMAI_PDM_VALIDATION_STATS;
#endif

#define IMAI_PDM_API_QUEUE

// All symbols in order
#define IMAI_PDM_SYMBOL_MAP {"unlabelled", "baby_cry"}

// Model GUID (16 bytes)
#define IMAI_PDM_MODEL_ID {0xcd, 0x29, 0x0f, 0x11, 0xc4, 0x9d, 0x7a, 0x43, 0xb0, 0xa0, 0xc6, 0x06, 0xd9, 0xa6, 0x18, 0x03}

// First nibble is bit encoding, second nibble is number of bytes
#define IMAGINET_TYPES_NONE    (0x0)
#define IMAGINET_TYPES_FLOAT32    (0x14)
#define IMAGINET_TYPES_FLOAT64    (0x18)
#define IMAGINET_TYPES_INT8    (0x21)
#define IMAGINET_TYPES_INT16    (0x22)
#define IMAGINET_TYPES_INT32    (0x24)
#define IMAGINET_TYPES_INT64    (0x28)
#define IMAGINET_TYPES_QDYN8    (0x31)
#define IMAGINET_TYPES_QDYN16    (0x32)
#define IMAGINET_TYPES_QDYN32    (0x34)

// data_in [1] (4 bytes)
#define IMAI_PDM_DATA_IN_COUNT (1)
#define IMAI_PDM_DATA_IN_TYPE float
#define IMAI_PDM_DATA_IN_TYPE_ID IMAGINET_TYPES_FLOAT32
#define IMAI_PDM_DATA_IN_SCALE (1)
#define IMAI_PDM_DATA_IN_OFFSET (0)
#define IMAI_PDM_DATA_IN_IS_QUANTIZED (0)

// data_out [60,20] (4800 bytes)
#define IMAI_PDM_DATA_OUT_COUNT (1200)
#define IMAI_PDM_DATA_OUT_TYPE float
#define IMAI_PDM_DATA_OUT_TYPE_ID IMAGINET_TYPES_FLOAT32
#define IMAI_PDM_DATA_OUT_SCALE (1)
#define IMAI_PDM_DATA_OUT_OFFSET (0)
#define IMAI_PDM_DATA_OUT_IS_QUANTIZED (0)

#define IMAI_PDM_KEY_MAX (16)



// Return codes
#define IMAI_PDM_RET_SUCCESS 0
#define IMAI_PDM_RET_NODATA -1
#define IMAI_PDM_RET_NOMEM -2

// Exported methods
int IMAI_PDM_dequeue(float *restrict data_out);
int IMAI_PDM_enqueue(const float *restrict data_in);
void IMAI_PDM_init(void);

#endif /* _IMAI_PDM_PDM_MODEL_H_ */
