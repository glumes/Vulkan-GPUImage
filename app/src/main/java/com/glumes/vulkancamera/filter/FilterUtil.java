package com.glumes.vulkancamera.filter;

import java.util.ArrayList;
import java.util.List;


public class FilterUtil {

    public static List<FilterType> getFilterList(){

        List<FilterType> filterTypes = new ArrayList<>();
        filterTypes.add(FilterType.ORIGIN_FILTER);
        filterTypes.add(FilterType.RGB_FILTER);
        filterTypes.add(FilterType.MIRROR_FILTER);
        filterTypes.add(FilterType.REVERT_FILTER_TYPE);
        filterTypes.add(FilterType.EXPOSURE_FILTER_TYPE);
        filterTypes.add(FilterType.GAMMA_FILTER_TYPE);
        filterTypes.add(FilterType.CONTRAST_FILTER_TYPE);
        filterTypes.add(FilterType.HAZE_FILTER_TYPE);



        return filterTypes;
    }
}
