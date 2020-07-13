package com.example.abdullah.doa_cnn_app;

/**
 * Created by Abdullah Kucuk on 10/5/18.
 */


import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;
import org.tensorflow.contrib.android.TensorFlowInferenceInterface;
import org.tensorflow.Graph;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;


public class ActivityInferenceDOA {
    static {
        System.loadLibrary("tensorflow_inference");
    }

    private static final String TAG = "TensorFlowDOAClassifier";

    private static ActivityInferenceDOA activityInferenceInstance;
    private TensorFlowInferenceInterface inferenceInterface;
    private static final String MODEL_FILE =  "file:///android_asset/optimized_frozenModel_feb_19.pb";//frozen_model_2pool_3rnn.pb  optimized_frozenModel_feb_19.pb
    private static final String INPUT_NODE = "inputs/x-input";
    private static final String[] OUTPUT_NODES = {"output_t"};
    private static final String OUTPUT_NODE = "output_t";
    private static final long[] INPUT_SIZE = {1,Settings.WaitFrame*2052};
    private static final int OUTPUT_SIZE = 10;
    private static AssetManager assetManager;
    InputStream is = null;

    private int xx=0;

    private static Graph graph;

    public static ActivityInferenceDOA getInstance(final Context context)
    {
        if (activityInferenceInstance == null)
        {
            activityInferenceInstance = new ActivityInferenceDOA(context);
        }
        return activityInferenceInstance;
    }

    public ActivityInferenceDOA(final Context context) {



        this.assetManager = context.getAssets();
        inferenceInterface = new TensorFlowInferenceInterface(assetManager, MODEL_FILE);
        int nClasses = (int) inferenceInterface.graph().operation(OUTPUT_NODE).output(0).shape().size(1);
        Log.i(TAG, "Read " + nClasses);

        graph = new Graph();


    }

    public float
            [] getActivityProb(float[] input_signal) throws IOException {
        float[] result = new float[OUTPUT_SIZE];
        inferenceInterface.feed(INPUT_NODE,input_signal,INPUT_SIZE);
        inferenceInterface.run(OUTPUT_NODES);
        inferenceInterface.fetch(OUTPUT_NODE,result);
/*
        if (xx==35){
            File root = new File("sdcard");
            File gpxfile = new File(root, "samples.txt");
            FileWriter writer = new FileWriter(gpxfile);
            for (int ii=0;ii<5*2052;ii++){
            writer.append(String.valueOf(input_signal[ii]));
            writer.append("\n");

            }

            writer.append("RESULTS");

            for (int ik=0;ik<10;ik++){

                writer.append(String.valueOf(result[ik])+" \n");
            }

            writer.flush();
            writer.close();



        }
        xx++;//*/
        return result;
    }
}
