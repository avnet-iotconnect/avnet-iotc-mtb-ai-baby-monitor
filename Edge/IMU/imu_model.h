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

#ifndef _IMAI_IMU_IMU_MODEL_H_
#define _IMAI_IMU_IMU_MODEL_H_
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
} IMAI_IMU_stats;

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


#define IMAI_IMU_TEST_AVG_ACC 0.930514096185738 // Accuracy
#define IMAI_IMU_TEST_AVG_F1S 0.926339382561455 // F1 Score

#define IMAI_IMU_TEST_STATS { \
 {name: "(unlabelled)", TP: 151, FN: 182, FP: 59, TN: 5638, TPR: 0.4534534534534, TNR: 0.9896436721081, PPV: 0.7190476190476, NPV: 0.9687285223367, FNR: 0.5465465465465, FPR: 0.0103563278918, FDR: 0.2809523809523, FOR: 0.0312714776632, F1S: 0.5561694290976, }, \
 {name: "standing", TP: 1190, FN: 83, FP: 37, TN: 4720, TPR: 0.9347996857816, TNR: 0.9922219886483, PPV: 0.9698451507742, NPV: 0.9827191338746, FNR: 0.0652003142183, FPR: 0.0077780113516, FDR: 0.0301548492257, FOR: 0.0172808661253, F1S: 0.952, }, \
 {name: "running", TP: 924, FN: 0, FP: 162, TN: 4944, TPR: 1, TNR: 0.9682726204465, PPV: 0.8508287292817, NPV: 1, FNR: 0, FPR: 0.0317273795534, FDR: 0.1491712707182, FOR: 0, F1S: 0.9194029850746, }, \
 {name: "walking", TP: 1246, FN: 8, FP: 46, TN: 4730, TPR: 0.9936204146730, TNR: 0.9903685092127, PPV: 0.9643962848297, NPV: 0.9983115238497, FNR: 0.0063795853269, FPR: 0.0096314907872, FDR: 0.0356037151702, FOR: 0.0016884761502, F1S: 0.9787902592301, }, \
 {name: "sitting", TP: 1622, FN: 0, FP: 4, TN: 4404, TPR: 1, TNR: 0.9990925589836, PPV: 0.9975399753997, NPV: 1, FNR: 0, FPR: 0.0009074410163, FDR: 0.0024600246002, FOR: 0, F1S: 0.9987684729064, }, \
 {name: "jumping", TP: 478, FN: 146, FP: 111, TN: 5295, TPR: 0.7660256410256, TNR: 0.9794672586015, PPV: 0.8115449915110, NPV: 0.9731666972982, FNR: 0.2339743589743, FPR: 0.0205327413984, FDR: 0.1884550084889, FOR: 0.0268333027017, F1S: 0.7881286067600, }, \
}

#ifdef IMAI_IMU_STATS_ENABLED
static const IMAI_IMU_stats IMAI_IMU_test_stats[] = IMAI_IMU_TEST_STATS;
#endif

/*
* Tensorflow Train Set
* 
* (ACC) Accuracy 96.441 %
* (F1S) F1 Score 96.367 %
* 
* Name of class                                            (unlabelled)          jumping          running          sitting         standing          walking
* (TP) True Positive or Correct Positive Prediction                 481             1960             2293             4424             4079             4134
* (FN) False Negative or Incorrect Negative Prediction              223               41               40                0              115              222
* (FP) False Positive or Incorrect Positive Prediction              106               49              178              100              131               77
* (TN) True Negative or Correct Negative Prediction               17202            15962            15501            13488            13687            13579
* (TPR) True Positive Rate or Sensitivity, Recall               68.32 %          97.95 %          98.29 %         100.00 %          97.26 %          94.90 %
* (TNR) True Negative Rate or Specificity, Selectivity          99.39 %          99.69 %          98.86 %          99.26 %          99.05 %          99.44 %
* (PPV) Positive Predictive Value or Precision                  81.94 %          97.56 %          92.80 %          97.79 %          96.89 %          98.17 %
* (NPV) Negative Predictive Value                               98.72 %          99.74 %          99.74 %         100.00 %          99.17 %          98.39 %
* (FNR) False Negative Rate or Miss Rate                        31.68 %           2.05 %           1.71 %           0.00 %           2.74 %           5.10 %
* (FPR) False Positive Rate or Fall-Out                          0.61 %           0.31 %           1.14 %           0.74 %           0.95 %           0.56 %
* (FDR) False Discovery Rate                                    18.06 %           2.44 %           7.20 %           2.21 %           3.11 %           1.83 %
* (FOR) False Omission Rate                                      1.28 %           0.26 %           0.26 %           0.00 %           0.83 %           1.61 %
* (F1S) F1 Score                                                74.52 %          97.76 %          95.46 %          98.88 %          97.07 %          96.51 %
*/


#define IMAI_IMU_TRAIN_AVG_ACC 0.9644126138130136 // Accuracy
#define IMAI_IMU_TRAIN_AVG_F1S 0.9636672948513608 // F1 Score

#define IMAI_IMU_TRAIN_STATS { \
 {name: "(unlabelled)", TP: 481, FN: 223, FP: 106, TN: 17202, TPR: 0.6832386363636, TNR: 0.9938756644326, PPV: 0.8194207836456, NPV: 0.9872022955523, FNR: 0.3167613636363, FPR: 0.0061243355673, FDR: 0.1805792163543, FOR: 0.0127977044476, F1S: 0.7451587916343, }, \
 {name: "standing", TP: 4079, FN: 115, FP: 131, TN: 13687, TPR: 0.9725798760133, TNR: 0.9905196121001, PPV: 0.9688836104513, NPV: 0.9916678742211, FNR: 0.0274201239866, FPR: 0.0094803878998, FDR: 0.0311163895486, FOR: 0.0083321257788, F1S: 0.9707282246549, }, \
 {name: "running", TP: 2293, FN: 40, FP: 178, TN: 15501, TPR: 0.9828546935276, TNR: 0.9886472351553, PPV: 0.9279643868878, NPV: 0.9974261630525, FNR: 0.0171453064723, FPR: 0.0113527648446, FDR: 0.0720356131121, FOR: 0.0025738369474, F1S: 0.9546211490424, }, \
 {name: "walking", TP: 4134, FN: 222, FP: 77, TN: 13579, TPR: 0.9490358126721, TNR: 0.9943614528412, PPV: 0.9817145571123, NPV: 0.9839142091152, FNR: 0.0509641873278, FPR: 0.0056385471587, FDR: 0.0182854428876, FOR: 0.0160857908847, F1S: 0.9650986342943, }, \
 {name: "sitting", TP: 4424, FN: 0, FP: 100, TN: 13488, TPR: 1, TNR: 0.9926405652045, PPV: 0.9778956675508, NPV: 1, FNR: 0, FPR: 0.0073594347954, FDR: 0.0221043324491, FOR: 0, F1S: 0.9888243182834, }, \
 {name: "jumping", TP: 1960, FN: 41, FP: 49, TN: 15962, TPR: 0.9795102448775, TNR: 0.9969396040222, PPV: 0.9756097560975, NPV: 0.9974379803786, FNR: 0.0204897551224, FPR: 0.0030603959777, FDR: 0.0243902439024, FOR: 0.0025620196213, F1S: 0.9775561097256, }, \
}

#ifdef IMAI_IMU_STATS_ENABLED
static const IMAI_IMU_stats IMAI_IMU_train_stats[] = IMAI_IMU_TRAIN_STATS;
#endif

/*
* Tensorflow Validation Set
* 
* (ACC) Accuracy 87.506 %
* (F1S) F1 Score 87.414 %
* 
* Name of class                                            (unlabelled)          jumping          running          sitting         standing          walking
* (TP) True Positive or Correct Positive Prediction                 159              439              511             1308             1481             1215
* (FN) False Negative or Incorrect Negative Prediction              169               57               96              243               38              127
* (FP) False Positive or Incorrect Positive Prediction              119              129              100                2              271              109
* (TN) True Negative or Correct Negative Prediction                5396             5218             5136             4290             4053             4392
* (TPR) True Positive Rate or Sensitivity, Recall               48.48 %          88.51 %          84.18 %          84.33 %          97.50 %          90.54 %
* (TNR) True Negative Rate or Specificity, Selectivity          97.84 %          97.59 %          98.09 %          99.95 %          93.73 %          97.58 %
* (PPV) Positive Predictive Value or Precision                  57.19 %          77.29 %          83.63 %          99.85 %          84.53 %          91.77 %
* (NPV) Negative Predictive Value                               96.96 %          98.92 %          98.17 %          94.64 %          99.07 %          97.19 %
* (FNR) False Negative Rate or Miss Rate                        51.52 %          11.49 %          15.82 %          15.67 %           2.50 %           9.46 %
* (FPR) False Positive Rate or Fall-Out                          2.16 %           2.41 %           1.91 %           0.05 %           6.27 %           2.42 %
* (FDR) False Discovery Rate                                    42.81 %          22.71 %          16.37 %           0.15 %          15.47 %           8.23 %
* (FOR) False Omission Rate                                      3.04 %           1.08 %           1.83 %           5.36 %           0.93 %           2.81 %
* (F1S) F1 Score                                                52.48 %          82.52 %          83.91 %          91.44 %          90.55 %          91.15 %
*/


#define IMAI_IMU_VALIDATION_AVG_ACC 0.8750641793599179 // Accuracy
#define IMAI_IMU_VALIDATION_AVG_F1S 0.8741440339706302 // F1 Score

#define IMAI_IMU_VALIDATION_STATS { \
 {name: "(unlabelled)", TP: 159, FN: 169, FP: 119, TN: 5396, TPR: 0.4847560975609, TNR: 0.9784224841341, PPV: 0.5719424460431, NPV: 0.9696316262353, FNR: 0.5152439024390, FPR: 0.0215775158658, FDR: 0.4280575539568, FOR: 0.0303683737646, F1S: 0.5247524752475, }, \
 {name: "standing", TP: 1481, FN: 38, FP: 271, TN: 4053, TPR: 0.9749835418038, TNR: 0.9373265494912, PPV: 0.8453196347031, NPV: 0.9907113175262, FNR: 0.0250164581961, FPR: 0.0626734505087, FDR: 0.1546803652968, FOR: 0.0092886824737, F1S: 0.9055334760012, }, \
 {name: "running", TP: 511, FN: 96, FP: 100, TN: 5136, TPR: 0.8418451400329, TNR: 0.9809014514896, PPV: 0.8363338788870, NPV: 0.9816513761467, FNR: 0.1581548599670, FPR: 0.0190985485103, FDR: 0.1636661211129, FOR: 0.0183486238532, F1S: 0.8390804597701, }, \
 {name: "walking", TP: 1215, FN: 127, FP: 109, TN: 4392, TPR: 0.9053651266766, TNR: 0.9757831592979, PPV: 0.9176737160120, NPV: 0.9718964372648, FNR: 0.0946348733233, FPR: 0.0242168407020, FDR: 0.0823262839879, FOR: 0.0281035627351, F1S: 0.9114778694673, }, \
 {name: "sitting", TP: 1308, FN: 243, FP: 2, TN: 4290, TPR: 0.8433268858800, TNR: 0.9995340167753, PPV: 0.9984732824427, NPV: 0.9463931171409, FNR: 0.1566731141199, FPR: 0.0004659832246, FDR: 0.0015267175572, FOR: 0.0536068828590, F1S: 0.9143656064313, }, \
 {name: "jumping", TP: 439, FN: 57, FP: 129, TN: 5218, TPR: 0.8850806451612, TNR: 0.9758743220497, PPV: 0.7728873239436, NPV: 0.9891943127962, FNR: 0.1149193548387, FPR: 0.0241256779502, FDR: 0.2271126760563, FOR: 0.0108056872037, F1S: 0.8251879699248, }, \
}

#ifdef IMAI_IMU_STATS_ENABLED
static const IMAI_IMU_stats IMAI_IMU_validation_stats[] = IMAI_IMU_VALIDATION_STATS;
#endif

#define IMAI_IMU_API_QUEUE

// All symbols in order
#define IMAI_IMU_SYMBOL_MAP {"unlabelled", "standing", "running", "walking", "sitting", "jumping"}

// Model GUID (16 bytes)
#define IMAI_IMU_MODEL_ID {0xe3, 0x2b, 0xf5, 0x32, 0xda, 0x1a, 0x21, 0x4b, 0xb8, 0x42, 0xa3, 0xfd, 0x40, 0x34, 0x71, 0x3a}

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

// data_in [3] (12 bytes)
#define IMAI_IMU_DATA_IN_COUNT (3)
#define IMAI_IMU_DATA_IN_TYPE float
#define IMAI_IMU_DATA_IN_TYPE_ID IMAGINET_TYPES_FLOAT32
#define IMAI_IMU_DATA_IN_SCALE (1)
#define IMAI_IMU_DATA_IN_OFFSET (0)
#define IMAI_IMU_DATA_IN_IS_QUANTIZED (0)

// data_out [50,3] (600 bytes)
#define IMAI_IMU_DATA_OUT_COUNT (150)
#define IMAI_IMU_DATA_OUT_TYPE float
#define IMAI_IMU_DATA_OUT_TYPE_ID IMAGINET_TYPES_FLOAT32
#define IMAI_IMU_DATA_OUT_SCALE (1)
#define IMAI_IMU_DATA_OUT_OFFSET (0)
#define IMAI_IMU_DATA_OUT_IS_QUANTIZED (0)

#define IMAI_IMU_KEY_MAX (3)



// Return codes
#define IMAI_IMU_RET_SUCCESS 0
#define IMAI_IMU_RET_NODATA -1
#define IMAI_IMU_RET_NOMEM -2

// Exported methods
int IMAI_IMU_dequeue(float *restrict data_out);
int IMAI_IMU_enqueue(const float *restrict data_in);
void IMAI_IMU_init(void);

#endif /* _IMAI_IMU_IMU_MODEL_H_ */
