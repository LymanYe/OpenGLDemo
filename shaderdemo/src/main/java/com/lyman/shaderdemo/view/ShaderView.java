package com.lyman.shaderdemo.view;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import com.lyman.shaderdemo.lib.ShaderLib;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Author: lyman
 * Email: lymenye@gmail.com
 * Date: 2017/9/19
 * Description:
 */

public class ShaderView extends GLSurfaceView {
    public ShaderView(Context context) {
        this(context, null);
    }

    public ShaderView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3);
        setRenderer(new ShaderRender());
    }

    class ShaderRender implements Renderer {

        @Override
        public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
            ShaderLib.init();
        }

        @Override
        public void onSurfaceChanged(GL10 gl10, int wight, int height) {
            ShaderLib.resize(wight, height);
        }

        @Override
        public void onDrawFrame(GL10 gl10) {
            ShaderLib.draw();
        }
    }
}
