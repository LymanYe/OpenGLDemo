package com.lyman.vertexdemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.lyman.vertexdemo.view.VertexView;

public class MainActivity extends AppCompatActivity {
    private VertexView mShaderView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mShaderView = new VertexView(this);
        setContentView(mShaderView);
    }

    @Override
    protected void onResume() {
        super.onResume();
        mShaderView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mShaderView.onPause();
    }
}
