//
// Created by Abdullah on 10/9/18.
//

#ifndef DOA_CNN_APP_TRANSFORMS_H
#define DOA_CNN_APP_TRANSFORMS_H

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct Transform {
    int points;
    float* sine;
    float* cosine;
    float* real;
    float* imaginary;
    void(*doTransform)(struct Transform* transform, float* input);
    void(*invTransform)(struct Transform* transform, float* inputreal, float* inputimaginary);
} Transform;

Transform* newTransform(int points);
void transformMagnitude(Transform* transform, float* output);
void invtranMagnitude(Transform* transform, float* output);
void destroyTransform(Transform** transform);
void FFT(Transform* fft, float* input);
void IFFT(Transform* fft, float* inputreal, float* inputimaginary);




#endif //DOA_CNN_APP_TRANSFORMS_H
