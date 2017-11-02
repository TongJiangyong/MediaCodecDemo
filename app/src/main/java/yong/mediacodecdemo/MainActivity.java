package yong.mediacodecdemo;

import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.storage.StorageManager;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import yong.mediacodecdemo.IvideoCallBack.IvideoStatusCallBack;
import yong.mediacodecdemo.MediaCodecASync.MediaCodecASync;

//暂时将状态处理放在类中，以后再做调整
public class MainActivity extends AppCompatActivity implements IvideoStatusCallBack{


    private PlayerView mPlayerView;
    private SurfaceHolder mSurfaceHolder;
    private float videoLong;
    private float currentPTS;
    private final static String TAG = "MainActivity";
    private MediaCodecBase mediaCodecClient;
    private Button playeButton;
    private Button PauseButton;
    private SurfaceHolder surfaceHolder;
    private SeekBar mSeekbar;
    private TextView mTextShow;
    private String filePath;
    private int mProgressStatus = 0;
    private boolean isPlaying = false;
    private boolean isPause = false;
    private String URI = "/storage/emulated/0/Movies/mpeg_ac3.ts";
    //private String URI = "/storage/emulated/0/Movies/AVC_AAC.mp4";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        playeButton =(Button)findViewById(R.id.button);
        PauseButton =(Button)findViewById(R.id.pause);
        mPlayerView =(PlayerView)findViewById(R.id.playerView);
        playeButton.setOnClickListener(onClickListener);
        PauseButton.setOnClickListener(onClickListener);
        mSeekbar = (SeekBar) findViewById(R.id.seekbar);
        mTextShow = (TextView)findViewById(R.id.textShow);
        mSeekbar.setOnSeekBarChangeListener(new MySeekBar());
/*        new Thread(new Runnable() {
            @Override
            public void run() {
                while(mProgressStatus < 100){
                    //mProgressStatus = doWork();
                    mHandler.post(new Runnable(){
                        @Override
                        public void run() {
                            mProgress.setProgress(mProgressStatus);
                        }
                    });
                }
            }
        }).start();*/
    }
    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            Bundle b = msg.getData();
            float currentPTS = (float)b.get("currentPTS");
            int currentRatio = (int)((currentPTS/videoLong)*100);
            mTextShow.setText("当前进度数值是："+currentPTS/1000);
            mSeekbar.setProgress(currentRatio);
        }
    };



    //用于button判断
    private View.OnClickListener onClickListener=new View.OnClickListener() {
        @Override
        public void onClick(View v){
            switch(v.getId()){
                case R.id.button:
                    if(isPlaying){
                        stopPlaying();
                    }else{
                        startPlaying();
                    }
                    break;
                case R.id.pause:
                    if(isPause){
                        continuePlay();
                    }else{
                        pausePlay();
                    }
            }
        }
    };

    public void startPlaying(){
        //showToash("PlayVideo");
/*        String test[] = getExtSDCardPath();
        for (String s:test){
            Log.i(TAG,"s:"+s);
    }*/
        File videoFile = FileTool.getPathFile(URI);
        if(videoFile!=null){
            showToash("文件存在");
            //filePath = videoFile.getAbsolutePath();
            initSurfce();
        }else{
            showToash("文件不存在");
        }
    }

    public void stopPlaying(){
        if(mediaCodecClient !=null) {
            mediaCodecClient.stopPlay();
            mediaCodecClient =null;
            isPlaying = false;
            showToash("播放停止");
        }else{
            showToash("播放出错");
        }
    }


    public void pausePlay(){
        mediaCodecClient.setIsPausing(true);
        isPause = true;
        showToash("播放暂停");
    }
    public void continuePlay(){
        isPause = false;
        mediaCodecClient.setIsPausing(false);
        showToash("恢复播放");
    }

    public void seekTo(long timeMs){
        Log.i(TAG,"timeMs:"+timeMs);
        mediaCodecClient.seekTo(timeMs);
    }
    public void initSurfce(){
        mSurfaceHolder = mPlayerView.getHolder();
        if(mediaCodecClient ==null){
            mediaCodecClient = new MediaCodecASync(URI,mSurfaceHolder.getSurface(),this.mHandler);
            mediaCodecClient.setSizeCallback(mPlayerView);
            mediaCodecClient.setStatusCallback(this);
            mediaCodecClient.startPlay();
            isPlaying = true;
        }
    }



    public void showToash(String info){
        Toast.makeText(MainActivity.this,info,Toast.LENGTH_LONG).show();
    }




    //用于seek bar
    private class MySeekBar implements SeekBar.OnSeekBarChangeListener {

        @Override
        public void onProgressChanged(SeekBar seekBar, int progress,
                                      boolean fromUser) {
            //mTextShow.setText("当前进度数值是："+progress);

        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
            //mTextShow.setText("开始拖动");
            Log.i(TAG,"开始拖动");
        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            // TODO Auto-generated method stub
            //mTextShow.setText("停止拖动");
            //Log.i(TAG,"停止拖动");
            //Log.i(TAG,"videoLong is:"+videoLong +"seekBar:"+seekBar.getProgress());
            seekTo((long)((float)seekBar.getProgress()/100*videoLong*1000));
        }

    }
    /**
     * 处理返回的status状态的函数
     */
    @Override
    public void videoLong(float time) {
        videoLong = time;
    }



    /**
     * 获取外置SD卡路径
     * @return  应该就一条记录或空
     */
    //经过测试，这个方法是可用的.....用来获取内外的external均可以....
    public  String[] getExtSDCardPath() {
        StorageManager storageManager = (StorageManager) this.getSystemService(Context
                .STORAGE_SERVICE);
        try {
            Class<?>[] paramClasses = {};
            Method getVolumePathsMethod = StorageManager.class.getMethod("getVolumePaths", paramClasses);
            getVolumePathsMethod.setAccessible(true);
            Object[] params = {};
            Object invoke = getVolumePathsMethod.invoke(storageManager, params);
            return (String[])invoke;
        } catch (NoSuchMethodException e1) {
            e1.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
        return null;
    }

    @Override
    public void onPause() {
        super.onPause();  // Always call the superclass method first

        // Release the Camera because we don't need it when paused
        // and other activities might need to use it.
        if (mediaCodecClient != null) {
            mediaCodecClient.stopPlay();
            mediaCodecClient = null;
        }
    }
}
