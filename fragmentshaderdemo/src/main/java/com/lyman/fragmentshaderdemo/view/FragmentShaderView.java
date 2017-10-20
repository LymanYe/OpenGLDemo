package com.lyman.fragmentshaderdemo.view;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import com.lyman.fragmentshaderdemo.lib.FragmentShaderLib;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Author: lyman
 * Email: lymenye@gmail.com
 * Date: 2017/9/19
 * Description:
 */

public class FragmentShaderView extends GLSurfaceView {
    private AssetManager mAssetManager;
    public FragmentShaderView(Context context) {
        this(context, null);
        mAssetManager = context.getAssets();
    }

    public FragmentShaderView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3);
        setRenderer(new ShaderRender());
    }


    class ShaderRender implements Renderer {

        @Override
        public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
            FragmentShaderLib.init(mAssetManager);
        }

        @Override
        public void onSurfaceChanged(GL10 gl10, int wight, int height) {
            FragmentShaderLib.resize(wight, height);
        }

        @Override
        public void onDrawFrame(GL10 gl10) {
            FragmentShaderLib.draw();
        }
    }
}
