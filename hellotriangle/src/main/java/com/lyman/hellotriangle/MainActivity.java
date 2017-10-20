package com.lyman.hellotriangle;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import com.lyman.hellotriangle.view.TriangleView;

public class MainActivity extends AppCompatActivity {
    private TriangleView mTriangleView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mTriangleView = new TriangleView(this);
        setContentView(mTriangleView);
    }

    @Override
    protected void onResume() {
        super.onResume();
        mTriangleView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mTriangleView.onPause();
    }
}
