package com.lyman.shaderdemo.lib;

/**
 * Author: lyman
 * Email: lymenye@gmail.com
 * Date: 2017/9/19
 * Description:
 */

public class ShaderLib {
    static {
        System.loadLibrary("myshader-lib");
    }

    public static native boolean init();

    public static native void resize(int wight,int height);

    public static native void draw();
}
