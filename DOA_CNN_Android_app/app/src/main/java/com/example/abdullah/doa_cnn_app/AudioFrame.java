package com.example.abdullah.doa_cnn_app;

/**
 * Created by Abdullah on 10/4/18.
 */

import java.util.Arrays;

public class AudioFrame {
    private short[] debugData;
    private short[] audioSamples;
    private static boolean stopFlag;
    /*
    public AudioFrame(){
        this.debugData = null;
        this.audioSamples = new short[Settings.stepSize];
    }
    */
    public AudioFrame(short[] ssamples,boolean stopFlag){
        this.debugData = null;
        this.stopFlag = stopFlag;
        this.audioSamples = Arrays.copyOf(ssamples,ssamples.length);
    }
    public short[] getAudio(){
        return audioSamples;
    }

    public void setDebug(short[] debugdata){
        this.debugData = debugdata;
    }

    public short[] getDebug(){
        return debugData;
    }

    public boolean getStopFlag(){
        return stopFlag;
    }

    public void setAduio(short[] samples){
        this.audioSamples = samples;
        /*
        for(int i=0;i<Settings.stepSize;i++){
            this.audioSamples[i] = samples[i];
        }
        */
    }
}
