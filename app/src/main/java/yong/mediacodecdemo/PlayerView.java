package yong.mediacodecdemo;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import yong.mediacodecdemo.IvideoCallBack.IvideoSizeCallBack;

/**
 * Created by hasee on 2017/10/31.
 * //将这个surface做成一个可控的surface试试......
 * module是干啥用的？
 * fragment是干啥用的，这两个一定要弄懂.....
 * 这两天
 * 今天一定要将mediaCodec做出，并seek出......
 */

//继承surfaceView处理.....
public class PlayerView extends SurfaceView implements SurfaceHolder.Callback,IvideoSizeCallBack {
    private double aspectRatio;
    private final static String TAG = "PlayerView";
    public PlayerView(Context context) {
        super(context);
    }

    public PlayerView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public PlayerView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }



    //在这里设置不同的分辨率
    public void setAspect(double aspect) {
        if (aspect > 0) {
            this.aspectRatio = aspect;
            requestLayout();
        }
    }


    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.i(TAG,"surfaceCreated");
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.i(TAG,"surfaceChanged");
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.i(TAG,"surfaceDestroyed");
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        if (aspectRatio > 0) {
            int initialWidth = MeasureSpec.getSize(widthMeasureSpec);
            int initialHeight = MeasureSpec.getSize(heightMeasureSpec);

            final int horizPadding = getPaddingLeft() + getPaddingRight();
            final int vertPadding = getPaddingTop() + getPaddingBottom();
            initialWidth -= horizPadding;
            initialHeight -= vertPadding;

            final double viewAspectRatio = (double) initialWidth / initialHeight;
            final double aspectDiff = aspectRatio / viewAspectRatio - 1;

            if (Math.abs(aspectDiff) > 0.01) {
                if (aspectDiff > 0) {
                    // adjust heght from width
                    initialHeight = (int) (initialWidth / aspectRatio);
                } else {
                    // adjust width from height
                    initialWidth = (int) (initialHeight * aspectRatio);
                }
                initialWidth += horizPadding;
                initialHeight += vertPadding;
                widthMeasureSpec = MeasureSpec.makeMeasureSpec(initialWidth, MeasureSpec.EXACTLY);
                heightMeasureSpec = MeasureSpec.makeMeasureSpec(initialHeight, MeasureSpec.EXACTLY);
            }
        }

        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    }

    @Override
    public void videoSize(int width, int hight) {
        this.setAspect((float)width/(float)hight);
    }
}
