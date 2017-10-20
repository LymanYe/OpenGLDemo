package com.lyman.texturedemo.view;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import com.lyman.texturedemo.lib.TextureLib;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Author: lyman
 * Email: lymenye@gmail.com
 * Date: 2017/9/19
 * Description:
 */

public class MyTextureView extends GLSurfaceView {
    public MyTextureView(Context context) {
        this(context, null);
    }

    public MyTextureView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3);
        setRenderer(new ShaderRender());
    }


    class ShaderRender implements Renderer {

        @Override
        public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
            TextureLib.init();
        }

        @Override
        public void onSurfaceChanged(GL10 gl10, int wight, int height) {
            TextureLib.resize(wight, height);
        }

        @Override
        public void onDrawFrame(GL10 gl10) {
            TextureLib.draw();
        }
    }
}
