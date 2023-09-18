#include <point_cloud2_filters/CropBoxFilter.hpp>
#include <pluginlib/class_list_macros.h>

PLUGINLIB_EXPORT_CLASS(point_cloud2_filters::CropBoxFilter, filters::FilterBase<sensor_msgs::PointCloud2>)
