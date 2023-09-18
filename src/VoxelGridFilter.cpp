#include <point_cloud2_filters/VoxelGridFilter.hpp>
#include <pluginlib/class_list_macros.h>

PLUGINLIB_EXPORT_CLASS(point_cloud2_filters::VoxelGridFilter, filters::FilterBase<sensor_msgs::PointCloud2>)
