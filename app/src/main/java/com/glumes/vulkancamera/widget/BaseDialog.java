package com.glumes.vulkancamera.widget;

import android.app.Dialog;
import android.content.Context;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.glumes.vulkancamera.R;
import com.glumes.vulkancamera.activity.BaseActivity;

public class BaseDialog extends Dialog {
    RecyclerView mRecyclerView;
    TextView mTextView;

    BaseDialog(@NonNull Context context) {
        super(context);

        setContentView(R.layout.dialog);

        if (getWindow() != null) {
            getWindow().setBackgroundDrawableResource(android.R.color.transparent);
        }
        mRecyclerView = findViewById(R.id.recycler_view);
        mRecyclerView.setHasFixedSize(true);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(context));

        mTextView = findViewById(R.id.text_view);
    }
}
