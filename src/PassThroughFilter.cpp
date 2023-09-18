#include <point_cloud2_filters/PassThroughFilter.hpp>
#include <pluginlib/class_list_macros.h>

PLUGINLIB_EXPORT_CLASS(point_cloud2_filters::PassThroughFilter, filters::FilterBase<sensor_msgs::PointCloud2>)
