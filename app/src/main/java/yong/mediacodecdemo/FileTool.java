package yong.mediacodecdemo;

import android.os.Environment;
import android.util.Log;

import java.io.File;

/**
 * Created by hasee on 2017/10/30.
 */

public class FileTool {
    //http://blog.csdn.net/mad1989/article/details/37568667
    /**
     * 判断SDCard是否存在 [当没有外挂SD卡时，内置ROM也被识别为存在sd卡]
     *
     * @return
     */
    public static boolean isSdCardExist() {
        return Environment.getExternalStorageState().equals(
                Environment.MEDIA_MOUNTED);
    }

    /**
     * 获取SD卡根目录路径
     *
     * @return
     */
    public static String getExternalStoragePath() {
        boolean exist = isSdCardExist();
        String sdpath = "";
        if (exist) {
            sdpath = Environment.getExternalStorageDirectory()
                    .getAbsolutePath();
        } else {
            sdpath = "error";
        }
        return sdpath;

    }


    /**
     * 得到sdcard的路径
     * @return  返回一个字符串数组   下标0:内置sdcard   下标1:外置sdcard
     * //测试不能通过这种方法获取，会出现空指针
     */
    public static String[] getSDCardPath(){
        String[] sdCardPath=new String[2];
        File sdFile=Environment.getExternalStorageDirectory();
        File[] files=sdFile.getParentFile().listFiles();
        //Log.i("MainActivity:","sdFile:"+sdFile.getParentFile().getParentFile().getAbsolutePath());
        for(File file:files){
            Log.i("MainActivity","123"+file.getAbsolutePath());
            if(file.getAbsolutePath().equals("sdcard1")){//外置
                sdCardPath[1]=sdFile.getAbsolutePath();
            }else if(file.getAbsolutePath().contains("sdcard0")){//得到内置sdcard
                sdCardPath[0]=file.getAbsolutePath();
            }
        }
        return sdCardPath;
    }





    /**
     * 获取默认的文件对象
     *
     * @return
     */
    public static File  getPathFile(String filePath) {
        File file = new File(filePath);
        if (file.exists()) {
            return file;
        } else {
            return null;
        }

    }



}
