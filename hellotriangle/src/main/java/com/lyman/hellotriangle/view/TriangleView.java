package com.lyman.hellotriangle.view;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import com.lyman.hellotriangle.lib.TriangleLib;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Author: lyman
 * Email: lymenye@gmail.com
 * Date: 2017/9/18
 * Description:
 */

public class TriangleView extends GLSurfaceView {
    public TriangleView(Context context) {
        this(context, null);
    }

    public TriangleView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
        //设置颜色模式为RGB_888,透明度,色度深度为16
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        //设置EGL版本为3
        setEGLContextClientVersion(3);
        //创建GLSurfaceView的渲染器，现在不知道具体有啥作用
        setRenderer(new TriangleRender());
    }

    class TriangleRender implements Renderer {

        @Override
        public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
            TriangleLib.init();
        }

        @Override
        public void onSurfaceChanged(GL10 gl10, int wight, int height) {
            TriangleLib.resize(wight, height);
        }

        @Override
        public void onDrawFrame(GL10 gl10) {
            TriangleLib.drawTriangle();
        }
    }
}
