#include <oxin_filters/RadiusOutlierRemoval.hpp>
#include <pluginlib/class_list_macros.h>

PLUGINLIB_EXPORT_CLASS(oxin_filters::RadiusOutlierRemovalFilter, filters::FilterBase<sensor_msgs::PointCloud2>)
