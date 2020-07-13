package com.example.abdullah.doa_cnn_app;

/**
 * Created by axk166230 on 9/11/2017.
 */

import android.view.animation.Animation;
import android.view.animation.Transformation;


public class Circleanimation extends Animation {

    private Circle circle;

    private float oldAngle;
    private float newAngle;

    public Circleanimation(Circle circle, float newAngle) {
        this.oldAngle = circle.getAngle();
        this.newAngle = newAngle;
        this.circle = circle;
    }

    @Override
    public void applyTransformation(float interpolatedTime, Transformation transformation) {
        float angle = oldAngle + ((newAngle - oldAngle) * interpolatedTime);

        //circle.;
        circle.setAngle(angle);
        circle.requestLayout();
    }




}
