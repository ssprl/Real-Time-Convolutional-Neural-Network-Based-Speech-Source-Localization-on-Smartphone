package com.example.abdullah.doa_cnn_app;

/**
 * Created by Abdullah Kucuk on 10/4/18.
 */



public class SignalProcessing {

    private long pointer;
    public SignalProcessing(){
        pointer = paramInitilization(Settings.Fs,Settings.stepSize,Settings.frameSize, Settings.stepFactor, Settings.noisetype, Settings.ThreadTime, Settings.DurationTime,Settings.Image_H,Settings.Image_W);
    }

    public void frameProcess(short[] input){
        realtimeProcessing(pointer,input);
    }

    public float getTime(){
        return getTime(pointer);
    }

    public float getComputeTime(){
        return getComputeTime(pointer);
    }

    public float getFilteringTime(){
        return getFilteringTime(pointer);
    }

    public void release(){
        paramElimination(pointer);
    }

    public short[] getSoundOutput(){
        return soundOutput(pointer,Settings.debugLevel);
    }

    public float[] getInput(){
        return GetCNNInput(pointer);
    }
    public int[] getCounter(){
        return Getframecounter(pointer);
    }

    public boolean[] getVADdecision(){
        return getVADdecision(pointer);
    }

    public short[] getDataOutput(){
        return dataOutput(pointer,Settings.debugLevel);
    }


    public static native long paramInitilization(int frequency, int stepSize,int frameSize,float stepFactor, int noisetype, float ThreadTime, float DurationTime, int Image_H, int Image_W);
    public static native void paramElimination(long memoryPointer);
    public static native float getTime(long memoryPointer);
    public static native float getComputeTime(long memoryPointer);
    public static native float getFilteringTime(long memoryPointer);
    public static native void realtimeProcessing(long memoryPointer,short[] input);
    public static native short[] soundOutput(long memoryPointer,int outputSelection);

    public static native float[] GetCNNInput(long memoryPointer);
    public static native int[] Getframecounter(long memoryPointer);
    public static native boolean[] getVADdecision(long memoryPointer);

    public static native short[] dataOutput(long memoryPointer,int outputSelection);


}


