package com.glumes.vulkancamera.capture;

/**
 * Created by oleg on 11/2/17.
 */

public interface PreviewFrameHandler {
    void onPreviewFrame(byte[] data, int width, int height);
}
