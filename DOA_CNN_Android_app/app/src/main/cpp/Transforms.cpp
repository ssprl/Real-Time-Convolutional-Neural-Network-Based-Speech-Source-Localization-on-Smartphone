#include "Transforms.h"

//#include "stdafx.h"
void FFT(Transform* fft, float* input);
void IFFT(Transform* fft, float* inputreal, float* inputimaginary);

Transform*
newTransform(int points)
{
    Transform* newTransform = (Transform*)malloc(sizeof(Transform));

    newTransform->points = points;
    newTransform->real = (float*)malloc(points * sizeof(float));
    newTransform->imaginary = (float*)malloc(points * sizeof(float));
    newTransform->sine = NULL;
    newTransform->cosine = NULL;
    newTransform->doTransform = FFT;
    newTransform->invTransform = IFFT;
    newTransform->sine = (float*)malloc((points / 2) * sizeof(float));
    newTransform->cosine = (float*)malloc((points / 2) * sizeof(float));
    //precompute twiddle factors
    double arg;
    int i;
    for (i = 0; i<points / 2; i++)
    {
        arg = -2 * M_PI*i / points;
        newTransform->cosine[i] = cos(arg);
        newTransform->sine[i] = sin(arg);
    }
    return newTransform;
}

void
FFT(Transform* fft, float* input)
{
    int i, j, k, L, m, n, o, p, q;
    double tempReal, tempImaginary, cos, sin, xt, yt;
    k = fft->points;
    for (i = 0; i<k; i++)
    {
        fft->real[i] = input[i];
        fft->imaginary[i] = 0;
    }

    j = 0;
    m = k / 2;
    //bit reversal
    for (i = 1; i<(k - 1); i++)
    {
        L = m;
        while (j >= L)
        {
            j = j - L;
            L = L / 2;
        }
        j = j + L;
        if (i<j)
        {
            tempReal = fft->real[i];
            tempImaginary = fft->imaginary[i];
            fft->real[i] = fft->real[j];
            fft->imaginary[i] = fft->imaginary[j];
            fft->real[j] = tempReal;
            fft->imaginary[j] = tempImaginary;
        }
    }
    L = 0;
    m = 1;
    n = k / 2;
    //computation
    for (i = k; i>1; i = (i >> 1))
    {
        L = m;
        m = 2 * m;
        o = 0;
        for (j = 0; j<L; j++)
        {
            cos = fft->cosine[o];
            sin = fft->sine[o];
            o = o + n;
            for (p = j; p<k; p = p + m)
            {
                q = p + L;
                xt = cos*fft->real[q] - sin*fft->imaginary[q];
                yt = sin*fft->real[q] + cos*fft->imaginary[q];
                fft->real[q] = (fft->real[p] - xt);
                fft->imaginary[q] = (fft->imaginary[p] - yt);
                fft->real[p] = (fft->real[p] + xt);
                fft->imaginary[p] = (fft->imaginary[p] + yt);
            }
        }
        n = n >> 1;
    }
}
void
IFFT(Transform* fft, float* inputreal, float* inputimaginary)
{
    int i, j, k, L, m, n, o, p, q;
    float tempReal, tempImaginary, cos, sin, xt, yt;
    k = fft->points;
    for (i = 0; i<k; i++)
    {
        fft->real[i] = inputreal[i];
        fft->imaginary[i] = (-1)*inputimaginary[i];
    }

    j = 0;
    m = k / 2;
    //bit reversal
    for (i = 1; i<(k - 1); i++)
    {
        L = m;
        while (j >= L)
        {
            j = j - L;
            L = L / 2;
        }
        j = j + L;
        if (i<j)
        {
            tempReal = fft->real[i];
            tempImaginary = fft->imaginary[i];
            fft->real[i] = fft->real[j];
            fft->imaginary[i] = fft->imaginary[j];
            fft->real[j] = tempReal;
            fft->imaginary[j] = tempImaginary;
        }
    }
    L = 0;
    m = 1;
    n = k / 2;
    //computation
    for (i = k; i>1; i = (i >> 1))
    {
        L = m;
        m = 2 * m;
        o = 0;
        for (j = 0; j<L; j++)
        {
            cos = fft->cosine[o];
            sin = fft->sine[o];
            o = o + n;
            for (p = j; p<k; p = p + m)
            {
                q = p + L;
                xt = cos*fft->real[q] - sin*fft->imaginary[q];
                yt = sin*fft->real[q] + cos*fft->imaginary[q];
                fft->real[q] = (fft->real[p] - xt);
                fft->imaginary[q] = (fft->imaginary[p] - yt);
                fft->real[p] = (fft->real[p] + xt);
                fft->imaginary[p] = (fft->imaginary[p] + yt);
            }
        }
        n = n >> 1;
    }
    for (i = 0; i<k; i++)
    {
        fft->real[i] = fft->real[i] / k;
        fft->imaginary[i] = fft->imaginary[i] / k;
    }
}
void
transformMagnitude(Transform* transform, double* output)
{
    int n;
    for (n = 0; n<transform->points; n++)
    {
        output[n] = sqrt(transform->real[n] * transform->real[n] + transform->imaginary[n] * transform->imaginary[n]);
    }
}

void
invtranMagnitude(Transform* transform, double* output)
{
    int n;
    float a;
    a = 1.0 / transform->points;
    for (n = 0; n < transform->points; n++)
    {
        output[n] = a * sqrt(transform->real[n] * transform->real[n] + transform->imaginary[n] * transform->imaginary[n]);
    }
}

void
destroyTransform(Transform** transform)
{
    if (*transform != NULL) {
        if ((*transform)->cosine != NULL) {
            free((*transform)->cosine);
            (*transform)->cosine = NULL;
        }
        if ((*transform)->sine != NULL) {
            free((*transform)->sine);
            (*transform)->sine = NULL;
        }
        if ((*transform)->real != NULL) {
            free((*transform)->real);
            (*transform)->real = NULL;
        }
        if ((*transform)->imaginary != NULL) {
            free((*transform)->imaginary);
            (*transform)->imaginary = NULL;
        }
        free(*transform);
        *transform = NULL;
    }
}
