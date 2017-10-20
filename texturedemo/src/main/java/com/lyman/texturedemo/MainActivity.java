package com.lyman.texturedemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.lyman.texturedemo.view.MyTextureView;

public class MainActivity extends AppCompatActivity {
    MyTextureView myTextureView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        myTextureView = new MyTextureView(this);
        setContentView(myTextureView);
    }

    @Override
    protected void onResume() {
        super.onResume();
        myTextureView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        myTextureView.onPause();
    }
}
