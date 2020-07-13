package com.example.abdullah.doa_cnn_app;

/**
 * Created by Abdullah on 10/4/18.
 */


import android.widget.EditText;
import android.widget.TextView;



public class Settings {
    public static int Fs = 48000;
    public static float stepTime = 20.0f;
    public static float frameTime = 2*stepTime;
    public static int stepSize = Math.round(Fs*stepTime*0.001f);
    public static int frameSize = Math.round(Fs*frameTime*0.001f);
    public static float stepFactor = 0.5f;
    public static final int queueSize = 10;

    public static float ThreadTime = 10;
    public static float DurationTime = 5;
    public static boolean isResultSaver = false;

    public static int noisetype = 1;
    public static int debugLevel = 0;

    public static EditText et_degree;
    public static EditText et_SFxavg;
    public static Circle et_circle;
    public static TextView ake_orientation;


    public static float output_angle = 0;
    public static float SFx_avg = 0;
    public static long angle_counter = 0;
    public static boolean playback = true;

    public static int WaitFrame = 10;
    public static int Image_H = WaitFrame*2;
    public static int Image_W = 513;
    public static int GT_angle=180;



    private static MainActivity main;
    public static final AudioFrame STOP = new AudioFrame(new short[] {1,2,4,8},true);
    public static void setCallbackInterface(MainActivity uiInterface) {
        main = uiInterface;
    }
    public static MainActivity getCallbackInterface() {
        return main;
    }

    public static boolean setupStepSize(float new_stepsize){
        if (new_stepsize>0){
            stepFactor = new_stepsize;
        }
        return false;
    }

    public static boolean setupThreadTime(float new_ThreadTime){
        if (new_ThreadTime>0){
            ThreadTime = new_ThreadTime;
        }
        return false;
    }

    public static boolean setupDurationTime(float new_ThreadTime){
        if (new_ThreadTime>0){
            DurationTime = new_ThreadTime;
        }
        return false;
    }

    public static boolean setupFrameTime(float new_frametime){
        if (new_frametime>0){
            frameTime = new_frametime;
            stepTime = frameTime/2;
            frameSize = Math.round(Fs*frameTime*0.001f);
            stepSize = Math.round(Fs*stepTime*0.001f);
            return true;
        }
        return false;
    }

    public static boolean setupNoiseType(int new_noisetype){
        if (new_noisetype!=noisetype){
            noisetype = new_noisetype;
            return true;
        }
        return false;
    }

    public static boolean setupPlayback(int new_playback){
        if(debugLevel!=new_playback){
            debugLevel = new_playback;
            return true;
        }
        return false;
    }
    public static boolean setupResultSaver(boolean new_isResultSaver){

        isResultSaver = new_isResultSaver;

        return isResultSaver;
    }
    public static void setPlayback(boolean flag){
        playback = flag;

    }
}


