#include <oxin_filters/PassThroughFilter.hpp>
#include <pluginlib/class_list_macros.h>

PLUGINLIB_EXPORT_CLASS(oxin_filters::PassThroughFilter, filters::FilterBase<sensor_msgs::PointCloud2>)
