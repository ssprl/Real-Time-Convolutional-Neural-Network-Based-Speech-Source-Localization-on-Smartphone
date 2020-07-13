# Real-Time-Convolutional-Neural-Network-Based-Speech-Source-Localization-on-Smartphone
 
## Overview

This GitHub repository provides for Convolutional Neural Network based Two Microphone DOA estimation on Android smartphone platform. The example app provided here is for hearing improvement studies. 
> **Abstract:** In this paper, we present a real-time convolutional neural network (CNN) based approach for speech source localization (SSL) using Android-based smartphone and its two built-in microphones under noisy conditions. We propose a new input feature set - using real and imaginary parts of the short-time Fourier transform (STFT) for CNN-based SSL. We use simulated noisy data from popular datasets that was augmented with few hours of real recordings collected on smartphones to train our CNN model. We compare the proposed method to recent CNN-based SSL methods that are trained on our dataset and show that our CNN-based SSL method offers higher accuracy on identical test datasets. Another unique aspect of this work is that we perform real-time inferencing of our CNN model on an Android smartphone with low latency (14 milliseconds(ms) for single frame-based estimation, 180 ms for multi frame-based estimation and frame length is 20 ms for both cases) and high accuracy (i.e. 88.83% at 0dB SNR). We show that our CNN model is rather robust to smartphone hardware mismatch, hence we may not need to retrain the entire model again for use with different smartphones. The proposed application provides a 'visual' indication of the direction of a talker on the screen of Android smartphones for improving the hearing of people with hearing disorders.

You can find the manuscript for this GitHub repository :https://ieeexplore.ieee.org/document/8910614

## Audio-Video Demo

https://youtu.be/7xadBuEBnGU

## Users Guides

A [User's Guide](User’s Guide- Android_TwoMicDOA_version_4.pdf) is provided describing the real-time implementation on Android smartphone platforms.

## Requirements 

- add requirements

## License and Citation

The codes are licensed under MIT license.

For any utilization of the code content of this repository, one of the following books needs to get cited by the user:

A. Küçük, A. Ganguly, Y. Hao and I. M. S. Panahi, "Real-Time Convolutional Neural Network-Based Speech Source Localization on Smartphone," in IEEE Access, vol. 7, pp. 169969-169978, 2019, doi: 10.1109/ACCESS.2019.2955049.

## Disclaimer

This work was supported in part by the National Institute of the Deafness and Other Communication Disorders (NIDCD) of the National

Institutes of Health (NIH) under Award 1R01DC015430-04. The content is solely the responsibility of the authors and does not necessarily

represent the official views of the NIH.
