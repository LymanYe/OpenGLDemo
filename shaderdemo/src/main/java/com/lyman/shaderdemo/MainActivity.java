package com.lyman.shaderdemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.lyman.shaderdemo.view.ShaderView;

public class MainActivity extends AppCompatActivity {
    private ShaderView mShaderView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mShaderView = new ShaderView(this);
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
