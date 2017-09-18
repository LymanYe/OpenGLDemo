package com.lyman.hellotriangle.lib;

/**
 * Author: lyman
 * Email: lymenye@gmail.com
 * Date: 2017/9/18
 * Description:
 */

public class TriangleLib {
    static {
        System.loadLibrary("triangle-lib");
    }

    //初始化GLES
    public static native boolean init();

    //设置GLES宽高
    public static native void resize(int wight, int height);

    //绘制三角形
    public static native void drawTriangle();
}
