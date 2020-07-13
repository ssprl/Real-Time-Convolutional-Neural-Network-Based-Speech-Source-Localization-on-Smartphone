package com.example.abdullah.doa_cnn_app;

/**
 * Created by Abdullah Kucuk on 10/4/18.
 */


import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.DashPathEffect;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.View;
import android.widget.RelativeLayout;


public class Circle  extends View
{
    float angle1;
    float angle2;

    float Dash1_angle1;//MODIFIED 12/02/2016
    float Dash2_angle2;//MODIFIED 12/02/2016
    float Dash3_angle3;//MODIFIED 12/02/2016
    float Dash4_angle4;//MODIFIED 12/02/2016

    DashPathEffect dashPathEffect;//MODIFIED 12/02/2016

    private static final int START_ANGLE_POINT = 90;

    //private final Paint paint;
    private final RectF rect;

    private float angle;

    public Circle(Context context, AttributeSet attrs) {
        super(context, attrs);

        final int strokeWidth = 30;

        paint = new Paint();
        paint.setAntiAlias(true);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeWidth(strokeWidth);
        //Circle color
        paint.setColor(Color.RED);

        paint_dash = new Paint();//MODIFIED 12/02/2016
        paint_dash.setAntiAlias(true);//MODIFIED 12/02/2016
        paint_dash.setStyle(Paint.Style.STROKE);//MODIFIED 12/02/2016
        paint_dash.setStrokeWidth(5);//MODIFIED 12/02/2016

        rect = new RectF(strokeWidth, strokeWidth, 200 + strokeWidth, 200 + strokeWidth);

        //Initial Angle (optional, it can be zero)
        angle = 360;
        path1 = new Path();
        path2 = new Path();
        this.angle1=270;
        this.angle2=270;

        Dash_path1 = new Path();//MODIFIED 12/02/2016
        this.Dash1_angle1=0;//MODIFIED 12/02/2016

        Dash_path2 = new Path();//MODIFIED 12/02/2016
        this.Dash2_angle2=45;//MODIFIED 12/02/2016

        Dash_path3 = new Path();//MODIFIED 12/02/2016
        this.Dash3_angle3=90;//MODIFIED 12/02/2016

        Dash_path4 = new Path();//MODIFIED 12/02/2016
        this.Dash4_angle4=135;//MODIFIED 12/02/2016
    }



    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        float angle = 360;


        float radius = canvas.getWidth()/3.2f;
        float x = canvas.getWidth()/2.2f;
        float y = canvas.getHeight()/4.7f;
        final RectF oval = new RectF();



        paint.setFlags(Paint.ANTI_ALIAS_FLAG);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeWidth(30);

        oval.set(x - radius, y - radius, x + radius,y + radius);

        // Draw circle
        paint.setColor(Color.RED);
        canvas.drawArc(oval, 0, angle, false, paint);
        paint.setColor(Color.BLUE);
        canvas.drawArc(oval, angle1-15, 40, false, paint);
        canvas.drawArc(oval, angle2-15, 40, false, paint);






///////////////////////////// /MODIFIED 12/02/2016////////////////////////////////////////

        dashPathEffect = new DashPathEffect(
                new float[]{10.0f, 15.0f}, //interval
                50);

        float a1 = Dash1_angle1*(float)Math.PI/180;
        float a2 = (Dash1_angle1+180)*(float)Math.PI/180;

        paint_dash.setPathEffect(dashPathEffect);
        paint_dash.setColor(Color.GRAY);
        Dash_path1.moveTo(x+ (float)Math.cos(a1) *radius, y + (float)Math.sin(a1) * radius);
        Dash_path1.lineTo(x+ (float)Math.cos(a2) *radius, y + (float)Math.sin(a2) * radius);
        canvas.drawPath(Dash_path1, paint_dash);


        float a3 = Dash2_angle2*(float)Math.PI/180;
        float a4 = (Dash2_angle2+180)*(float)Math.PI/180;


        Dash_path2.moveTo(x+ (float)Math.cos(a3) *radius, y + (float)Math.sin(a3) * radius);
        Dash_path2.lineTo(x+ (float)Math.cos(a4) *radius, y + (float)Math.sin(a4) * radius);
        canvas.drawPath(Dash_path2, paint_dash);

        float a5 = Dash3_angle3*(float)Math.PI/180;
        float a6 = (Dash3_angle3+180)*(float)Math.PI/180;


        Dash_path3.moveTo(x+ (float)Math.cos(a5) *radius, y + (float)Math.sin(a5) * radius);
        Dash_path3.lineTo(x+ (float)Math.cos(a6) *radius, y + (float)Math.sin(a6) * radius);
        canvas.drawPath(Dash_path3, paint_dash);

        float a7 = Dash4_angle4*(float)Math.PI/180;
        float a8 = (Dash4_angle4+180)*(float)Math.PI/180;


        Dash_path4.moveTo(x+ (float)Math.cos(a7) *radius, y + (float)Math.sin(a7) * radius);
        Dash_path4.lineTo(x+ (float)Math.cos(a8) *radius, y + (float)Math.sin(a8) * radius);
        canvas.drawPath(Dash_path4, paint_dash);







    }
    private Path path1;
    private Path path2;
    private Paint paint;


    ///////////////////////////// /MODIFIED 12/02/2016////////////////////////////////////////
    private Path Dash_path1;
    private Path Dash_path2;
    private Path Dash_path3;
    private Path Dash_path4;

    private Paint paint_dash;//MODIFIED 12/02/2016
    ///////////////////////////////////////////////////////////////////////////////////////////
    public float getAngle() {
        return angle1;
    }

    public void setAngle(float angle) {
        this.angle1 = angle-90;

        this.angle2=270-angle;
    }


}

