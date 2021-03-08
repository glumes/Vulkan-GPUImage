package com.glumes.vulkancamera.activity;

import android.os.Bundle;

import android.util.DisplayMetrics;
import android.util.SparseIntArray;
import android.view.MotionEvent;
import android.view.Surface;


import androidx.fragment.app.FragmentActivity;

import com.glumes.vulkancamera.capture.PreviewFrameHandler;
import com.glumes.vulkancamera.capture.VideoCameraPreview;


public abstract class BaseActivity extends FragmentActivity implements PreviewFrameHandler {

    private static final SparseIntArray ORIENTATIONS = new SparseIntArray();
    protected VideoCameraPreview mPreview;

    static {
        ORIENTATIONS.append(Surface.ROTATION_0, 90);
        ORIENTATIONS.append(Surface.ROTATION_90, 0);
        ORIENTATIONS.append(Surface.ROTATION_180, 270);
        ORIENTATIONS.append(Surface.ROTATION_270, 180);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);

        mPreview = new VideoCameraPreview(this);
        mPreview.init(displayMetrics.widthPixels, displayMetrics.heightPixels);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        return super.onTouchEvent(event);
    }

    protected int getOrientation() {
        int rotation = getWindowManager().getDefaultDisplay().getRotation();
        return (ORIENTATIONS.get(rotation) + mPreview.getSensorOrientation() + 270) % 360;
    }

}
