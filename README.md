# point_cloud_2_filters
Wrappers for some of the [pcl filters](https://pointclouds.org/documentation/group__filters.html) for `sensor_msgs/PointCloud2` ROS messages. The implementation and usage is based on the [filter](https://wiki.ros.org/filters) and [sensor_filter](https://wiki.ros.org/sensor_filters) packages, so it is different from the wrappers of the PCL filters provided by the package [pcl_ros](https://wiki.ros.org/pcl_ros/Tutorials/filters).

All the parameters are settable from the config file, but also online through the `dynamic_reconfigure` server.

No ROS2 version (yet).

## Usage example
See launch and config folders

## Filters list
### PassThroughFilter
Wrapper for the [pcl::PassThrough](https://pointclouds.org/documentation/classpcl_1_1_pass_through_3_01pcl_1_1_p_c_l_point_cloud2_01_4.html) filter 
#### Params
-  `active`*(bool, default: true)* Activate the filter or not.
-  `input_frame`*(str, default: "")* The input TF frame the data should be transformed into before processing
-  `output_frame`*(str, default: "")* The output TF frame the data should be transformed into after processing
-  `keep_organized`*(bool, default: true)* Keep the point cloud organized ([`pcl::FilterIndices<PointT>::setKeepOrganized	(bool keep_organized)`](https://pointclouds.org/documentation/classpcl_1_1_filter_indices.html#a21eb00357056c0cc432cd03afa84d08c)
-  `negative`*(bool, default: false)* Set to true to return the data outside the min max limits
-  `filter_field_name`*(str, default: z)* The field to be used for filtering data
-  `filter_limit_min`*(double, default: 0)* The minimum allowed field value a point will be considered
-  `filter_limit_min`*(double, default: 1)* The maximum allowed field value a point will be considered

### CropBoxFilter
Wrapper for the [pcl::CropBox](https://pointclouds.org/documentation/classpcl_1_1_crop_box_3_01pcl_1_1_p_c_l_point_cloud2_01_4.html) filter.  
**Warning** `pcl::CrobBox` parameter `keep_organized` is broken on ROS melodic (on noetic it is ok).
#### Params
-  `active`*(bool, default: true)* Activate the filter or not.
-  `input_frame`*(str, default: "")* The input TF frame the data should be transformed into before processing
-  `output_frame`*(str, default: "")* The output TF frame the data should be transformed into after processing
-  `keep_organized`*(bool, default: true)* Keep the point cloud organized ([`pcl::FilterIndices<PointT>::setKeepOrganized	(bool keep_organized)`](https://pointclouds.org/documentation/classpcl_1_1_filter_indices.html#a21eb00357056c0cc432cd03afa84d08c)
-  `negative`*(bool, default: false)* Set to true to return the data outside the min max limits
-  `min_x`*(double, default: -1.0)* The minimum allowed x value a point will be considered from. Range: -1000.0 to 1000.0
-  `max_x`*(double, default: -1.0)* The maximum allowed x value a point will be considered from. Range: -1000.0 to 1000.0
-  `min_y`*(double, default: -1.0)* The minimum allowed y value a point will be considered from. Range: -1000.0 to 1000.0
-  `max_y`*(double, default: -1.0)* The maximum allowed y value a point will be considered from. Range: -1000.0 to 1000.0
-  `min_z`*(double, default: -1.0)* The minimum allowed z value a point will be considered from. Range: -1000.0 to 1000.0
-  `max_z`*(double, default: -1.0)* The maximum allowed z value a point will be considered from. Range: -1000.0 to 1000.0

### VoxelGridFilter
Wrapper for the [pcl::VoxelGrid](https://pointclouds.org/documentation/classpcl_1_1_voxel_grid.html) filter.  
#### Params
-  `active`*(bool, default: true)* Activate the filter or not.
-  `input_frame`*(str, default: "")* The input TF frame the data should be transformed into before processing
-  `output_frame`*(str, default: "")* The output TF frame the data should be transformed into after processing
-  `negative`*(bool, default: false)* Set to true to return the data outside the min max limits
-  `leaf_size_x`*(double, default: 0.01)* The size of a leaf (on x) used for downsampling. Range: 0.0 to 1.0
-  `leaf_size_y`*(double, default: 0.01)* The size of a leaf (on y) used for downsampling. Range: 0.0 to 1.0
-  `leaf_size_z`*(double, default: 0.01)* The size of a leaf (on z) used for downsampling. Range: 0.0 to 1.0
-  `min_points_per_voxel`*(int, default:0)* Set the minimum number of points required for a voxel to be used
-  `downsample_all_data`*(int, default:0)* Set to true if all fields need to be downsampled, or false if just XYZ
-  `filter_field_name`*(str, default: z)* The field to be used for filtering data, acting like a passthrough
-  `filter_limit_min`*(double, default: 0)* The minimum allowed field value a point will be considered
-  `filter_limit_min`*(double, default: 1)* The maximum allowed field value a point will be considered

### SacSegmentationExtractFilter
Wrapper to extract a geometric model with [pcl::SACSegmentation](https://pointclouds.org/documentation/classpcl_1_1_s_a_c_segmentation.html) and [pcl::ExtractIndices](https://pointclouds.org/documentation/classpcl_1_1_extract_indices.html).
#### Params
-  `active`*(bool, default: true)* Activate the filter or not.
-  `input_frame`*(str, default: "")* The input TF frame the data should be transformed into before processing
-  `output_frame`*(str, default: "")* The output TF frame the data should be transformed into after processing
-  `negative`*(bool, default: false)* Set whether to filter out (remove) the model (true) or all the rest (false).
- `model_type` *(int, default: 16)* Geometric model to look for. Default to `SACMODEL_NORMAL_PARALLEL_PLANE`. Check [pcl official doc](https://pointclouds.org/documentation/group__sample__consensus.html).
- `method_type` *(int, default: 0)* Segmentation model to use for. Default to `SAC_RANSAC` . Check [pcl official doc](https://pointclouds.org/documentation/group__sample__consensus.html).
-  `axis_x`*(double, default: 0.0)* The x component of the normal to the model to be removed. Range: 0.0 to 1.0
-  `axis_y`*(double, default: 0.0)* The y component of the normal to the model to be removed. Range: 0.0 to 1.0
-  `axis_z`*(double, default: 1.0)* The z component of the normal to the model to be removed. Range: 0.0 to 1.0
-  `eps_angle`*(double, default: 0.15)* Tolerance angle (rad) to the model to be considered normal to the axis. Range: -3.15 to 3.15
-  `distance_threshold`*(double, default: 0.01)*  Range: 0 to 10
-  `optimize_coefficents`*(bool, default: 0.01)* Optimize the coefficents or not.
-  `max_iterations`*(bool, default: 50)* 
-  `probability`*(bool, default: 0.99)* 
-  `min_radius`*(bool, default: -1)* 
-  `max_radius`*(bool, default: 1000)* 

