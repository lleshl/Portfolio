package com.example.myapplication;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.BlurMaskFilter;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.EmbossMaskFilter;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Toast;

import androidx.annotation.Nullable;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;


public class MyCanvas extends View {
    Bitmap mBitmap;
    Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.emo_im_cool);;
    private Boolean stamp = false, blurring = false, embossing = false, penb = false, eraser = false;
    static float color = 1;
    enum pencolor {black, red, green, blue};
    Canvas mCanvas;
    Paint mPaint = new Paint();
    String opt = "";
    int check, mcheck = 0;
    public MyCanvas(Context context) {
        super(context);
        this.setLayerType(LAYER_TYPE_SOFTWARE, null);
        this.mPaint.setColor(Color.BLACK);
    }

    public MyCanvas(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        this.setLayerType(LAYER_TYPE_SOFTWARE, null);
        this.mPaint.setColor(Color.BLACK);
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        mBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
        mCanvas = new Canvas();
        mCanvas.setBitmap(mBitmap);
        mCanvas.drawColor(Color.WHITE);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        mPaint.setAntiAlias(true);
        if (blurring){
            BlurMaskFilter blur = new BlurMaskFilter(100, BlurMaskFilter.Blur.NORMAL);
            mPaint.setMaskFilter(blur);
        }
        else mPaint.setMaskFilter(null);
        if (embossing){
            mcheck = 1;
            EmbossMaskFilter eMask = new EmbossMaskFilter(new float[]{3, 3, 10}, 0.5f,
                    12, 30);
            mPaint.setMaskFilter(eMask);
        }
        else if(mcheck == 1) {
                mPaint.setMaskFilter(null);
                mcheck = 0;
            }
        if(penb) {
            mPaint.setStrokeWidth(15);
        }
        else mPaint.setStrokeWidth(3);
        if(eraser){
            check = 1;
            mPaint.setColor(Color.WHITE);
            mPaint.setStrokeWidth(20);
        }
        else if(check == 1){
                mPaint.setColor(Color.BLACK);
                check = 0;
                mPaint.setStrokeWidth(3);
        }
        if(mBitmap != null) canvas.drawBitmap(mBitmap, 0, 0, null);
    }

    public void setOptType(String opt){

        this.opt = opt;
    }

    public void setPenColor(pencolor pen) {
        if (pen == pencolor.black){
            mPaint.setColor(Color.BLACK);
        }
        else if (pen == pencolor.red) {
            mPaint.setColor(Color.RED);
        }
        else if (pen == pencolor.green) {
            mPaint.setColor(Color.GREEN);
        }
        else if (pen == pencolor.blue) {
            mPaint.setColor(Color.BLUE);
        }
    }

    public void clear(){
        mBitmap.eraseColor(Color.WHITE);
        invalidate();
    }

    public void setBlurring(boolean blurring){

        this.blurring = blurring;
    }

    public void setStamp(boolean stamp){
        this.stamp = stamp;
        if (!stamp)
            setOptType("");
    }

    public void setEmbossing(boolean embossing){

        this.embossing = embossing;
    }

    public void setB(boolean penb){

        this.penb = penb;
    }

    public void setEraser(boolean eraser){

        this.eraser = eraser;
    }
    @SuppressLint("WrongThread")
    public boolean Save(String file_name) {
        try {
            FileOutputStream out = new FileOutputStream(file_name);
            mBitmap.compress(Bitmap.CompressFormat.JPEG, 100, out);
            out.close();
            return true;

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return false;
    }

    public boolean Load(String file_name) {
        try {
            Bitmap bittemp = BitmapFactory.decodeFile(file_name).copy(Bitmap.Config.ARGB_8888, true);
            mCanvas.drawBitmap(bittemp, 0, 0, null);
            invalidate();
            return true;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    int oldX = -1, oldY = -1;
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int X = (int) event.getX();
        int Y = (int) event.getY();
        if(event.getAction() == MotionEvent.ACTION_DOWN){
            oldX = X; oldY = Y;
        }
        else if(event.getAction() == MotionEvent.ACTION_MOVE){
            if (oldX != -1 && !stamp) {
                mCanvas.drawLine(oldX, oldY, X, Y, mPaint);
                oldX = X;
                oldY = Y;
                invalidate();
            }
        }
        else if(event.getAction() == MotionEvent.ACTION_UP){
            if (oldX != -1 && !stamp) {
                mCanvas.drawLine(oldX, oldY, X, Y, mPaint);
                invalidate();
            }
            else if (stamp){
                mCanvas.save();
                if (opt.equals("rotate")) {
                    mCanvas.rotate(30,this.getWidth()/2, this.getHeight()/2);
                }
                else if (opt.equals("bright")){
                    color += 0.2f;
                    float[] array =   {     color,  0,      0,      0,      0,
                                            0,      color,  0,      0,      0,
                                            0,      0,      color,  0,      0,
                                            0,      0,      0,      1,      0};
                    ColorMatrix cm = new ColorMatrix(array);
                    mPaint.setColorFilter(new ColorMatrixColorFilter(cm));
                }
                else if (opt.equals("dark")){
                    color -= 0.2f;
                    float[] array =   {     color,  0,      0,      0,      0,
                            0,      color,  0,      0,      0,
                            0,      0,      color,  0,      0,
                            0,      0,      0,      1,      0};
                    ColorMatrix cm = new ColorMatrix(array);
                    mPaint.setColorFilter(new ColorMatrixColorFilter(cm));
                }
                else if (opt.equals("skew")){
                    mCanvas.skew(0.5f,0);
                }
                mCanvas.drawBitmap(bitmap, X, Y, mPaint);
                invalidate();
                mCanvas.restore();
            }
            oldX = -1;
            oldY = -1;
        }
        return true;
    }
}
