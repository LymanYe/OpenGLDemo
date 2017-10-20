package com.lyman.vertexdemo.lib;

/**
 * Author: lyman
 * Email: lymenye@gmail.com
 * Date: 2017/9/19
 * Description:
 */

public class VertexLib {
    static {
        System.loadLibrary("vertexarray-lib");
    }

    public static native boolean init();

    public static native void resize(int wight,int height);

    public static native void draw();
}
