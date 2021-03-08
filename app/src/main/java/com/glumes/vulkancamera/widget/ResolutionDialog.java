package com.glumes.vulkancamera.widget;

import android.content.Context;
import android.util.Size;

import androidx.annotation.NonNull;

import com.glumes.vulkancamera.R;
import com.glumes.vulkancamera.adapter.ItemAdapter;

import java.util.ArrayList;
import java.util.List;

public class ResolutionDialog extends BaseDialog {
    private ItemAdapter<Size> mAdapter;

    private itemClickAction itemClickAction = null;
    ResolutionDialog(@NonNull Context context) {
        super(context);

        mTextView.setText(R.string.select_resolution);
        ArrayList<Size> items = new ArrayList<>();
        mAdapter = new ItemAdapter<>(items, mListener, R.layout.size_list_item);
        mRecyclerView.setAdapter(mAdapter);
    }

    void setItems(List<Size> items) {
        mAdapter.setItems(items);
    }

    void setItemClickAction(itemClickAction action){
        itemClickAction = action;
    }

    private ItemAdapter.ItemListener<Size> mListener = new ItemAdapter.ItemListener<Size>() {
        @Override
        public void onItemClick(Size item) {
            dismiss();
            if (itemClickAction != null){
                itemClickAction.onItemClick(item);
            }
//            mPreview.changeSize(item);
        }
    };

    public interface itemClickAction{
        void onItemClick(Size size);
    }
}