package com.lyman.fragmentshaderdemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.lyman.fragmentshaderdemo.view.FragmentShaderView;

public class MainActivity extends AppCompatActivity {
    FragmentShaderView fragmentShaderView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        fragmentShaderView = new FragmentShaderView(this);
        setContentView(fragmentShaderView);
    }

    @Override
    protected void onResume() {
        super.onResume();
        fragmentShaderView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        fragmentShaderView.onPause();
    }
}
