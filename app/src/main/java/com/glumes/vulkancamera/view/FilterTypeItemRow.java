package com.glumes.vulkancamera.view;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Size;

import com.glumes.vulkancamera.adapter.ItemAdapter;
import com.glumes.vulkancamera.filter.FilterType;

public class FilterTypeItemRow extends ItemRow implements ItemAdapter.RecyclerViewRow<FilterType> {
    public FilterTypeItemRow(Context context) {
        super(context);
    }

    public FilterTypeItemRow(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public FilterTypeItemRow(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @Override
    public void setData(FilterType item) {
        mTextView.setText(item.toString());
    }
}