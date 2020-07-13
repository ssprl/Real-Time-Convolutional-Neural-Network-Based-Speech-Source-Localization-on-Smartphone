package com.example.abdullah.doa_cnn_app;

/**
 * Created by Abdullah Kucuk on 10/4/18.
 */


import android.app.Activity;
import android.media.MediaPlayer;
import android.util.Log;

import java.io.IOException;
import java.util.concurrent.BlockingQueue;
import android.content.Context;



public class Processing extends Activity implements Runnable {




    MainActivity myActivity = new MainActivity();


    private BlockingQueue<AudioFrame> input;

    private BlockingQueue<AudioFrame> output;
    private Context maincontext;
    private SignalProcessing signalProcessor;
    private Thread speechThread;
    private int counter,idx,mod_counter;
    private int[] temp_count;
    private float[] image;
    private float[] temp_arr;
    boolean[] isSpeech;
    int player_track_idx;
    String [] est_angles;
    double [] mse_angle;
    double sum_mse_angle;
    int accuracy;

    MediaPlayer mp;
    private final String[] listContent = { "degree_0", "degree_20", "degree_40", "degree_60",
            "degree_80", "degree_100", "degree_120", "degree_140", "degree_160", "degree_180" };
    private final int[] resID = { R.raw.degree_0, R.raw.degree_20,R.raw.degree_40, R.raw.degree_60,R.raw.degree_80, R.raw.degree_100,
            R.raw.degree_120, R.raw.degree_140,R.raw.degree_160, R.raw.degree_180};



    private double [] cnn_input;
    private ActivityInferenceDOA activityInferenceDOA;

    private static String TAG = Processing.class.getSimpleName();
    public Processing(BlockingQueue<AudioFrame> input, BlockingQueue<AudioFrame> output, Context context) {
        this.input = input;

        this.output = output;
        this.maincontext=context;
        signalProcessor = new SignalProcessing();
        speechThread = new Thread(this);
        speechThread.start();
        Log.e(TAG, "onCreate initialize successfully!");
        activityInferenceDOA = new ActivityInferenceDOA(maincontext);
        temp_count= new int[1];
        idx=Settings.Image_W*4;
        image = new float[idx*Settings.WaitFrame];
        temp_arr= new float[idx];
        isSpeech=new boolean[1];
        player_track_idx=0;
        mp = new MediaPlayer();
        sum_mse_angle=0.0;
        accuracy = 0;
        est_angles= new String[100];
        mse_angle = new double[100];



    }
    @Override
    public void run() {
        try{
            loop:while(true){
                AudioFrame currentFrame = null;



                currentFrame = input.take();



                if(currentFrame==Settings.STOP){

                    output.put(currentFrame);
                    break loop;
                }

                signalProcessor.frameProcess(currentFrame.getAudio());

                if(Settings.playback){
              
                    temp_count=signalProcessor.getCounter();
                    isSpeech = signalProcessor.getVADdecision();
                    counter=temp_count[0]; // counter for number of speech frame
                    mod_counter=(counter+1)%Settings.WaitFrame;
                    temp_arr=signalProcessor.getInput();
                    if ((mod_counter==0)&& (isSpeech[0])){

                        System.arraycopy(temp_arr,0,image,(Settings.WaitFrame-1)*idx, temp_arr.length); // mod_counter is Setting.WaitFrame

                        float prob[] = activityInferenceDOA.getActivityProb(image);
                        int maxIndex = 0;
                        for (int i = 1; i < prob.length; i++) {
                            float newnumber = prob[i];
                            if ((newnumber > prob[maxIndex])) {
                                maxIndex = i;
                            }
                        }

                        Log.e(TAG, String.valueOf((maxIndex*20)));
                        Settings.output_angle = maxIndex*20;


                        /* Accuracy and Error Calculation*/
                        if (player_track_idx<100){
                            est_angles[player_track_idx]=String.valueOf(Settings.output_angle);
                            mse_angle[player_track_idx]= Math.pow((maxIndex*20)-Settings.GT_angle,2);
                            sum_mse_angle=sum_mse_angle+mse_angle[player_track_idx];
                            if (mse_angle[player_track_idx]==0.0)
                                accuracy++;
                        }


                        if ((player_track_idx%(100)==0)&&(player_track_idx>0))
                            playSong(maxIndex);

                        player_track_idx++;

                        myActivity.runOnUiThread(new Runnable() {
                            public void run() {
                                myActivity.updateGUI();
                            }
                        });



                    }else{
                        if (isSpeech[0])
                            System.arraycopy(temp_arr,0,image,(mod_counter-1)*idx, temp_arr.length);

                    }


                }

            }
        }catch(InterruptedException e){
            Thread.currentThread().interrupt();
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public void playSong(int songIndex) {
        // Play song
        mp.reset();// stops any current playing song
        mp = MediaPlayer.create(maincontext, resID[songIndex]);// create's
        // new
        // mediaplayer
        // with
        // song.

        mp.start(); // starting mediaplayer

    }
    public int getAccuracy(){


        return accuracy;
    }

    public int getPlayertrackidx(){


        return player_track_idx+1;// it starts from zero
    }
    public double getSumMSE(){


        return sum_mse_angle;
    }
    public String[] getEst_angles(){


        return est_angles;
    }
}

