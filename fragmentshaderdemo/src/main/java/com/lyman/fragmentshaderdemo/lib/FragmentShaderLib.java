package com.lyman.fragmentshaderdemo.lib;

import android.content.res.AssetManager;

/**
 * Author: lyman
 * Email: lymenye@gmail.com
 * Date: 2017/9/19
 * Description:
 */

public class FragmentShaderLib {
    static {
        System.loadLibrary("fragmentshader-lib");
    }

    public static native boolean init(AssetManager assetManager);

    public static native void resize(int wight, int height);

    public static native void draw();
}
