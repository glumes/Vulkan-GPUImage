package com.glumes.vulkancamera.activity;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.SeekBar;

import androidx.core.app.ActivityCompat;
import androidx.fragment.app.DialogFragment;

import com.glumes.vulkancamera.R;
import com.glumes.vulkancamera.filter.FilterType;
import com.glumes.vulkancamera.filter.FilterUtil;
import com.glumes.vulkancamera.render.VKVideoRenderer;
import com.glumes.vulkancamera.widget.FilterTypeDialog;


public class VKActivity extends BaseActivity {

    private VKVideoRenderer mVideoRenderer;
    private SeekBar mSeekBar;
    private Button mChooseFilter;
    private FilterTypeDialog mFilterDialog;

    private static final int REQUEST_CAMERA_PERMISSION = 1;
    private static final String FRAGMENT_DIALOG = "dialog";

    private static final String[] CAMERA_PERMISSIONS = {
            Manifest.permission.CAMERA,
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.READ_EXTERNAL_STORAGE
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_vk);

        SurfaceView surfaceView = findViewById(R.id.surface_view);
        mVideoRenderer = new VKVideoRenderer();
        mVideoRenderer.init(surfaceView,getAssets());

        mSeekBar = findViewById(R.id.seekBar);
        mChooseFilter = findViewById(R.id.button_choose_filter);

        mFilterDialog = new FilterTypeDialog(this);
        mFilterDialog.setItems(FilterUtil.getFilterList());

        mFilterDialog.setItemClickAction(new FilterTypeDialog.itemClickAction() {
            @Override
            public void onItemClick(FilterType size) {
                mVideoRenderer.setFilterType(size.ordinal());
            }
        });

        mSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                mVideoRenderer.updateFilterValue(progress);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        mChooseFilter.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mFilterDialog.show();
            }
        });

        ((FrameLayout) findViewById(R.id.preview)).addView(mPreview);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void onResume() {
        super.onResume();
        if (!hasPermissionsGranted(CAMERA_PERMISSIONS)) {
            requestCameraPermission();
        } else {
            mPreview.startCamera();
        }
    }

    @Override
    public void onPause() {
        mPreview.stopCamera();
        super.onPause();
    }

    private void requestCameraPermission() {
        if (shouldShowRequestPermissionRationale(Manifest.permission.CAMERA)) {
            new ConfirmationDialog().show(getSupportFragmentManager(), FRAGMENT_DIALOG);
        } else {
            requestPermissions(CAMERA_PERMISSIONS, REQUEST_CAMERA_PERMISSION);
        }
    }

    private boolean hasPermissionsGranted(String[] permissions) {
        for (String permission : permissions) {
            if (ActivityCompat.checkSelfPermission(this, permission)
                    != PackageManager.PERMISSION_GRANTED) {
                return false;
            }
        }
        return true;
    }

    @Override
    public void onPreviewFrame(byte[] data, int width, int height) {
        mVideoRenderer.drawVideoFrame(data, width, height, getOrientation());
    }

    public static class ConfirmationDialog extends DialogFragment {

        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState) {
            final Activity activity = getActivity();
            return new AlertDialog.Builder(activity)
                    .setMessage(R.string.request_permission)
                    .setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            ActivityCompat.requestPermissions(activity, CAMERA_PERMISSIONS,
                                    REQUEST_CAMERA_PERMISSION);
                        }
                    })
                    .setNegativeButton(android.R.string.cancel,
                            new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {
                                    activity.finish();
                                }
                            })
                    .create();
        }
    }

}
