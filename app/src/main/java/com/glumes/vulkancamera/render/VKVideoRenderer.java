package com.glumes.vulkancamera.render;

import android.content.res.AssetManager;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class VKVideoRenderer extends VideoRenderer implements SurfaceHolder.Callback {


    AssetManager assetManager;

    public VKVideoRenderer() {

    }

    public void init(SurfaceView surface,AssetManager manager) {
        surface.getHolder().addCallback(this);
        assetManager = manager;
    }

    public void drawVideoFrame(byte[] data, int width, int height, int rotation) {
        draw(data, width, height, rotation);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        create(Type.VK_YUV420.getValue());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        init(holder.getSurface(), width, height,assetManager);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        destroy();
    }


    public void setFilterType(int param){
        setParameters(param);
    }

    public void updateFilterValue(int progress){
        setProgress(progress);
    }
}

