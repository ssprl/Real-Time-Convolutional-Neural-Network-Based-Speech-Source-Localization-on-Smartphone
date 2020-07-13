/*
 COPYRIGHT: SSPRL, 2017, ...
 Author: Abdullah Kucuk
  9/11/2017 - AK - Initial release
 */

#include <cassert>
#include <cstring>
#include <jni.h>

#include <sys/types.h>
#include <SLES/OpenSLES.h>


#include "audio_main.h"
double ake_sum=0;
double ake_timing=0;
int xx=0;

extern "C" {
JNIEXPORT jlong JNICALL
        Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_paramInitilization(JNIEnv *env, jobject thiz,int frequency, int stepSize,int frameSize, float stepFactor, int noisetype, float ThreadTime, float DurationTime, int Image_H, int Image_W );
JNIEXPORT void JNICALL
        Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_realtimeProcessing(JNIEnv *env, jobject thiz,  jlong memoryPointer, jshortArray input);
JNIEXPORT void JNICALL
        Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_paramElimination(JNIEnv* env, jobject thiz, jlong memoryPointer);
JNIEXPORT jshortArray JNICALL
        Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_soundOutput(JNIEnv* env, jobject thiz, jlong memoryPointer, jint outputSelection);

JNIEXPORT jfloatArray JNICALL
        Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_GetCNNInput(JNIEnv *env, jclass type, jlong memoryPointer);

JNIEXPORT jshortArray JNICALL
        Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_dataOutput(JNIEnv* env, jobject thiz, jlong memoryPointer, jint outputSelection);
JNIEXPORT jfloat JNICALL
        Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_getTime(JNIEnv* env, jobject thiz, jlong memoryPointer);
JNIEXPORT jfloat JNICALL
        Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_getComputeTime(JNIEnv* env, jobject thiz, jlong memoryPointer);
JNIEXPORT jfloat JNICALL
        Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_getFilteringTime(JNIEnv* env, jobject thiz, jlong memoryPointer);}











JNIEXPORT jlong JNICALL
Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_paramInitilization(
        JNIEnv *env, jobject thiz,int frequency, int stepSize,int frameSize, float stepFactor, int noisetype, float ThreadTime, float DurationTime, int Image_H, int Image_W){
    Variables *mainParam = (Variables*)malloc(sizeof(Variables));


    mainParam->stepSize = stepSize;
    mainParam->samplingRate = frequency;
    mainParam->frameSize = frameSize;  ////Threadshold Time
    mainParam->filLen = 1;
    mainParam->topMicBuffer = (float*)calloc(frameSize+mainParam->filLen-1,sizeof(float));
    mainParam->botMicBuffer = (float*)calloc(frameSize+mainParam->filLen-1,sizeof(float));

    mainParam->x_frame = (float*)malloc(stepSize * sizeof(float));
    mainParam->h_frame = (float*)malloc(stepSize * sizeof(float));

    mainParam->originalBuffer = (int*)calloc(2*stepSize,sizeof(int));
    mainParam->mixedBuffer = (short*)calloc(stepSize,sizeof(short));
        mainParam->Trainingframe = (int)ThreadTime;
    mainParam->DurationThresh = (int)DurationTime;

    mainParam->angle = 0;
    mainParam->speech_count = 0;

    mainParam->ake_counter=0;
    mainParam->ake_avg_timer=0;

    /*Memory Update*/
    mainParam->_output=(float*)calloc(3,sizeof(float)); // We have 3 output

    mainParam->trans1= newTransform(DOA_NFFTT);
    mainParam->trans2= newTransform(DOA_NFFTT);
    mainParam->DOA_CNN_input= (float *)calloc(4*(DOA_NFFTT/2+1),sizeof(float));
    mainParam->frcount= (int *)malloc( sizeof(int));
    mainParam->image_h=Image_H;
    mainParam->image_w=Image_W;
    mainParam->wind = (float*)malloc((DOA_nn) * sizeof(float));
    mainParam->wind=Hamming(DOA_nn);
    mainParam->currmag_fft_Framex =(float*)malloc((DOA_NFFTT) * sizeof(float));
    mainParam->prevmag_fft_Framex =(float*)malloc((DOA_NFFTT) * sizeof(float));
    mainParam->sfx =(float*)malloc((DOA_NFFTT) * sizeof(float));
    mainParam->isSpeech=false;
    mainParam->sum_sfx=0;
    mainParam->SFx =0;

    __android_log_print(ANDROID_LOG_ERROR, "Parameter Initialized","Successfully");
    return (jlong)mainParam;
}


JNIEXPORT void JNICALL
Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_realtimeProcessing(JNIEnv *env, jobject thiz,  jlong memoryPointer, jshortArray input){
    Variables *mainParam = (Variables*) memoryPointer;
    //startTimer(mainParam->timer);
    mainParam->_in = env->GetShortArrayElements(input, NULL);
    int i,j;//stepSize,frameSize,filLen;

    for (int k = 0; k < DOA_nn; k++)
    {

        mainParam-> x_frame[k] = mainParam->botMicBuffer[k] * mainParam->wind[k];
        mainParam-> h_frame[k] = mainParam->topMicBuffer[k] * mainParam->wind[k];

    }
   //*
    clock_t t;
    t = clock();


    FFT(mainParam->trans2,mainParam->h_frame);
    FFT(mainParam->trans1,mainParam->x_frame);

    mainParam->sum_sfx=0;
    mainParam->ake_counter= mainParam->ake_counter+1;


    for (int i = 0; i < DOA_NFFTT/2+1; i++)
    {

        mainParam->currmag_fft_Framex[i] = sqrt(pow(mainParam->trans1->real[i],2)+ pow(mainParam->trans1->imaginary[i], 2));


        mainParam->sfx[i] = mainParam->currmag_fft_Framex[i] - mainParam->prevmag_fft_Framex[i];

        mainParam->sum_sfx += pow( mainParam->sfx[i],2);

        mainParam->prevmag_fft_Framex[i] = mainParam->currmag_fft_Framex[i];

    }

    mainParam->SFx = mainParam->sum_sfx / (DOA_NFFTT);


    if (mainParam->ake_counter==0) {
        mainParam->SFxavg = mainParam->SFx;
        mainParam->SFxmax = mainParam->SFx;
    }
    else {

        if (mainParam->ake_counter<=mainParam->Trainingframe) {
            if (mainParam->SFx > mainParam->SFxmax) {

                mainParam->SFxmax = mainParam->SFx;

            }
            mainParam->SFxavg = (mainParam->SFxavg*(mainParam->ake_counter-1 ) + mainParam->SFx) / mainParam->ake_counter;
        }
    }

    if ((mainParam->ake_counter >= mainParam->Trainingframe) && (mainParam->SFx > mainParam->SFxavg) && (mainParam->SFx > mainParam->SFxmax)) {

        mainParam->flagSFx +=   1;										/*   % Duration*/
    }
    else {
        mainParam->flagSFx = 0;
    }


    if (mainParam->ake_counter >= mainParam->Trainingframe) {
        if (mainParam->flagSFx > mainParam->DurationThresh){
            mainParam->isSpeech=true;
            mainParam->speech_count++;

        }


        else
            mainParam->isSpeech=false;


    }
    else {

        mainParam->isSpeech=false;
    }

    for(i = 0;i<mainParam->filLen-1;i++){
        mainParam->topMicBuffer[i] = mainParam->topMicBuffer[i+mainParam->stepSize];
        mainParam->botMicBuffer[i] = mainParam->botMicBuffer[i+mainParam->stepSize];
    }
    for(i = mainParam->filLen-1,j=0;i<mainParam->filLen+mainParam->stepSize-1;i++,j+=2){
        mainParam->topMicBuffer[i]= mainParam->topMicBuffer[i+mainParam->stepSize];
        mainParam->topMicBuffer[i+mainParam->stepSize]= mainParam->_in[j+1]*FAC;

        mainParam->botMicBuffer[i]= mainParam->botMicBuffer[i+mainParam->stepSize];
        mainParam->botMicBuffer[i+mainParam->stepSize]= mainParam->_in[j]*FAC;

        mainParam->originalBuffer[j] = mainParam->_in[j];
        mainParam->originalBuffer[j+1] = mainParam->_in[j+1];
    }
    for(i=0;i<mainParam->stepSize;i++){
        mainParam->mixedBuffer[i] = (mainParam->_in[i*2]+mainParam->_in[i*2+1])/2;
    }
    env->ReleaseShortArrayElements(input, mainParam->_in, 0);

}


JNIEXPORT void JNICALL
Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_paramElimination(JNIEnv* env, jobject thiz, jlong memoryPointer){
    Variables *mainParam = (Variables*) memoryPointer;
    if(mainParam != NULL){
        /*
        if(mainParam->topMicBuffer!= NULL)
        free(mainParam->topMicBuffer);mainParam->topMicBuffer = NULL;
        if(mainParam->botMicBuffer!= NULL)
        free(mainParam->botMicBuffer);mainParam->botMicBuffer = NULL;
        //free(mainParam->outputBuffer);mainParam->outputBuffer = NULL;
        if(mainParam->x_frame!= NULL)
        free(mainParam->x_frame);mainParam->x_frame = NULL;
        if(mainParam->h_frame!= NULL)
        free(mainParam->h_frame);mainParam->h_frame = NULL;
        if(mainParam->originalBuffer!= NULL)
        free(mainParam->originalBuffer);mainParam->originalBuffer = NULL;
        if(mainParam->mixedBuffer!= NULL)
        free(mainParam->mixedBuffer);mainParam->mixedBuffer = NULL;
        if(mainParam->logMMSE!= NULL)
        free(mainParam->logMMSE);mainParam->logMMSE = NULL;
        if(mainParam->_output!= NULL)
        free(mainParam->_output);mainParam->_output = NULL;
        if(mainParam->_in!= NULL)
        free(mainParam->_in);mainParam->_in = NULL;
        //if(mainParam->twoDOA!= NULL)
        //free(mainParam->twoDOA);mainParam->twoDOA = NULL;
        //*/

        /*
        free(mainParam->y_prev);mainParam->y_prev = NULL;
        free(mainParam->y_curr);mainParam->y_curr = NULL;
        free(mainParam->y);mainParam->y = NULL;
        free(mainParam->e);mainParam->e = NULL;
        free(mainParam->w);mainParam->w = NULL;//*/

        //destroylogMMSE(&(mainParam->logmmsePtr));
        //destroyTimer(&(mainParam->timer));
        //free(mainParam);mainParam = NULL;
    }
}

JNIEXPORT jshortArray JNICALL
Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_soundOutput(JNIEnv* env, jobject thiz, jlong memoryPointer, jint outputSelection){
    Variables* mainParam = (Variables*) memoryPointer;
    jshortArray output = env->NewShortArray(mainParam->stepSize);
    short *_output = env->GetShortArrayElements( output, NULL);
    int i;
    switch (outputSelection){
        case 0:		// Original

            for (i=0;i<mainParam->stepSize;i++){
                _output[i] = (short)checkRange(mainParam->mixedBuffer[i]);
            }
            //mainParam->angle = mainParam->angle_count;
            //mainParam->angle_count++;

            for (i=0;i<mainParam->stepSize;i++){
                _output[i] = mainParam->mixedBuffer[i];
            }
            break;

    }
    env->ReleaseShortArrayElements(output, _output, 0);
    return output;
}



JNIEXPORT jfloatArray JNICALL
Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_GetCNNInput(JNIEnv *env, jclass type,
                                                                      jlong memoryPointer) {
    Variables* mainParam = (Variables*) memoryPointer;
    jfloatArray output = env->NewFloatArray((DOA_NFFTT)*2+4);
    mainParam->DOA_CNN_input = env->GetFloatArrayElements( output, NULL);


    memcpy(mainParam->DOA_CNN_input, mainParam->trans1->imaginary , sizeof(float)*((DOA_NFFTT/2)+1));
    memcpy(mainParam->DOA_CNN_input+((DOA_NFFTT/2)+1), mainParam->trans2->imaginary , sizeof(float)*((DOA_NFFTT/2)+1));
    memcpy(mainParam->DOA_CNN_input+2*((DOA_NFFTT/2)+1), mainParam->trans1->real , sizeof(float)*((DOA_NFFTT/2)+1));
    memcpy(mainParam->DOA_CNN_input+3*((DOA_NFFTT/2)+1), mainParam->trans2->real , sizeof(float)*((DOA_NFFTT/2)+1));




 



    env->ReleaseFloatArrayElements(output, mainParam->DOA_CNN_input, 0);

    return output;

}
extern "C"
JNIEXPORT jintArray JNICALL
Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_Getframecounter(JNIEnv *env, jclass type,
                                                                     jlong memoryPointer) {
    Variables* mainParam = (Variables*) memoryPointer;
    jintArray output = env->NewIntArray(1);
    mainParam->frcount = env->GetIntArrayElements( output, NULL);

    mainParam->frcount[0]=mainParam->speech_count;



    env->ReleaseIntArrayElements(output, mainParam->frcount, 0);

    return output;

}

extern "C"
JNIEXPORT jbooleanArray JNICALL
Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_getVADdecision__J(JNIEnv *env, jclass type,
                                                                           jlong memoryPointer) {

    Variables* mainParam = (Variables*) memoryPointer;
    jbooleanArray output = env->NewBooleanArray(1);
    mainParam->VADarray =  env->GetBooleanArrayElements(output, NULL);
    mainParam->VADarray[0] = mainParam->isSpeech ? JNI_TRUE : JNI_FALSE;
    env->ReleaseBooleanArrayElements(output,mainParam->VADarray,0);
    return output;

}
JNIEXPORT jshortArray JNICALL
Java_com_example_abdullah_doa_1cnn_1app_SignalProcessing_dataOutput(JNIEnv* env, jobject thiz, jlong memoryPointer, jint outputSelection){
    Variables* mainParam = (Variables*) memoryPointer;
    jshortArray output = env->NewShortArray(2*mainParam->stepSize);
    short *_output = env->GetShortArrayElements(output, NULL);
    int i;
    switch (outputSelection){
        case 0:
            for(i= 0;i<2*mainParam->stepSize;i++){
                _output[i] = mainParam->originalBuffer[i];
            }
            break;
      /*  case 1:
            for(i= 0;i<2*mainParam->stepSize;i++){
                _output[i] = (short)checkRange(mainParam->outputBuffer[i]);
            }
            break;
        case 2:
            break;//*/
    }
    env->ReleaseShortArrayElements(output, _output, 0);
    return output;
}




float *Hamming( int np) {// Hamming definition is changed according to Hamming.m file
    float *y = (float*)calloc(np, sizeof(float));
    int a;
    for (a = 0; a < np; a++) {
        y[a] = (0.54 - 0.46*cos((2 * M_PI*a) / (np - 1)));
        //printf("y[%d]=%.17f\n", a, y[a]);

    }

    return y;
    free(y);
    y=NULL;
}


int checkRange(float input)
{
    int output;
    if(input>1){
        output = 32767;
    }else if(input<-1){
        output = -32768;
    }else
        output = 32768*input;
    return output;
}
//////////////
///////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


