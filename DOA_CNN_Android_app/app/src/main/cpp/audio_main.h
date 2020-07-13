//
// Created by axk166230 on 9/11/2017.
//

#ifndef TWOCHANDOA_V4_AUDIO_MAIN_H
#define TWOCHANDOA_V4_AUDIO_MAIN_H

#define EPS 1.0e-7
#define FAC 3.051757812500000e-05f
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <jni.h>
//#include "audio_common.h"
#include "Transforms.h"
#include <time.h>
//#include "LogMMSE.h"
#include <android/log.h>
typedef struct Variables{
   // Timer* timer;
    float *topMicBuffer,*botMicBuffer,*x_frame,*h_frame,*DOA_CNN_input,*wind;//, *outputBuffer
    float  *prevmag_fft_Framex, *prevmag_fft_Frameh,*currmag_fft_Framex,*currmag_fft_Frameh,*sfx;
    int *originalBuffer;
    short *mixedBuffer;
    int stepSize,samplingRate,frameSize,filLen, image_h, image_w ;
    //float *y_curr, *y_prev, *e , *w, *y, u;
   // int counter,uplimit;
    float angle, Threadtime, DurationTime,sum_sfx,SFx,SFxavg,SFxmax;
    int speech_count;
    bool isEnchanced, isSpeech;
   // twomicDOA * twoDOA;
    //logMMSE_parameters *logMMSE; // for channel 1
    int ake_counter,flagSFx,Trainingframe,DurationThresh;
    int *frcount;
    double ake_avg_timer;
    //logMMSE_parameters *logMMSE2; // for channel 2
    /*Memory Update*/
    float *_output;
    short *_in;
    jboolean  *VADarray;
    Transform *trans1,*trans2,*trans3;
}Variables;

int DOA_fss = 48000;
int DOA_NFFTT = 1024;
int DOA_nn = 960;


int checkRange( float input );
float *Hamming( int np);


#endif //TWOCHANDOA_V4_AUDIO_MAIN_H
