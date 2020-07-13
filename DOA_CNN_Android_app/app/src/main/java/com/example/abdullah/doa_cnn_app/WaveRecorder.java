package com.example.abdullah.doa_cnn_app;

/**
 * Created by Abdullah Kucuk on 10/4/18.
 */


import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.util.Log;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;



public class WaveRecorder implements Runnable {
    private short[] abuf_short;
    private AtomicBoolean recordFlag;
    private int recBufLen;
    private AudioRecord recorder;
    private BlockingQueue<AudioFrame> output;
    private Thread recThread;
    private static String TAG = WaveRecorder.class.getSimpleName();
    public WaveRecorder(BlockingQueue<AudioFrame> output){
        this.output = output;
        recordFlag = new AtomicBoolean();
        recBufLen=Settings.stepSize*2;
        recorder = new AudioRecord(MediaRecorder.AudioSource.MIC,Settings.Fs, AudioFormat.CHANNEL_IN_STEREO, AudioFormat.ENCODING_PCM_16BIT,recBufLen);
        Log.e(TAG, "onCreate initialize successfully!");
        recThread = new Thread(this);
        recThread.start();
    }

    public void stopRecording(){
        recordFlag.set(false);
    }

    public boolean recordFlag(){
        return recordFlag.get();
    }

    public void run() {
        try{
            if(recorder!=null){
                recorder.stop();
                recorder.startRecording();

                recordFlag.set(true);

            }
        }catch(IllegalStateException e){
            recordFlag.set(false);
        }
        try{
            loop:while(true){
                if(recordFlag.get()){
                    abuf_short = new short[2*Settings.stepSize];
                    recorder.read(abuf_short,0,2*Settings.stepSize);
                    output.put(new AudioFrame(abuf_short,false));
                }else{
                    output.put(Settings.STOP);
                    break loop;
                }
            }
        }catch (InterruptedException e){
            Thread.currentThread().interrupt();
            e.printStackTrace();
        }

        try{
            if(recorder!=null){
                recorder.stop();
                recorder.release();
                recorder=null;

                Log.e("WaveRecorder"," has been eliminated successfully!");
            }
        }catch (IllegalStateException e){
            e.printStackTrace();
        }
    }
}

