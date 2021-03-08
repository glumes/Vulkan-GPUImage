package com.glumes.vulkancamera.widget;

import android.content.Context;

import androidx.annotation.NonNull;

import com.glumes.vulkancamera.R;
import com.glumes.vulkancamera.adapter.ItemAdapter;
import com.glumes.vulkancamera.filter.FilterType;

import java.util.ArrayList;
import java.util.List;

public class FilterTypeDialog extends BaseDialog {
    private ItemAdapter<FilterType> mAdapter;

    private itemClickAction itemClickAction = null;

    public FilterTypeDialog(@NonNull Context context) {
        super(context);

        mTextView.setText(R.string.select_filter);
        ArrayList<FilterType> items = new ArrayList<>();
        mAdapter = new ItemAdapter<>(items, mListener, R.layout.filter_type_list_item);
        mRecyclerView.setAdapter(mAdapter);
    }

    public void setItems(List<FilterType> items) {
        mAdapter.setItems(items);
    }

    public void setItemClickAction(itemClickAction action){
        itemClickAction = action;
    }

    private ItemAdapter.ItemListener<FilterType> mListener = new ItemAdapter.ItemListener<FilterType>() {
        @Override
        public void onItemClick(FilterType item) {
            dismiss();
            if (itemClickAction != null){
                itemClickAction.onItemClick(item);
            }
        }
    };

    public interface itemClickAction{
        void onItemClick(FilterType size);
    }
}