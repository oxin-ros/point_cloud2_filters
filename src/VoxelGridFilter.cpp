#include <oxin_filters/VoxelGridFilter.hpp>
#include <pluginlib/class_list_macros.h>

PLUGINLIB_EXPORT_CLASS(oxin_filters::VoxelGridFilter, filters::FilterBase<sensor_msgs::PointCloud2>)
