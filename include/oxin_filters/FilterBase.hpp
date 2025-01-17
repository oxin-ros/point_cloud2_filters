#ifndef FILTER_BASE_POINT_CLOUD_HPP
#define FILTER_BASE_POINT_CLOUD_HPP

#include <memory>

#include <ros/ros.h>

#if ROS_VERSION_MINIMUM(1, 15, 0)
    #include <filters/filter_base.hpp>
#else
    #include <filters/filter_base.h>
#endif
#include <tf2_ros/transform_listener.h>
#include <pcl_ros/transforms.h>

// Dynamic reconfigure
#include <dynamic_reconfigure/server.h>
#include <boost/thread/recursive_mutex.hpp>
#include <oxin_filters/FilterBaseConfig.h>

#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>

namespace oxin_filters
{
    class FilterBase : public filters::FilterBase<sensor_msgs::PointCloud2>
    {
    public:
        FilterBase();
        ~FilterBase() = default;

    public:
        virtual bool configure();

        /** \brief Update the filter and return the data seperately
         * \param data_in T array with length width
         * \param data_out T array with length width
         */
        virtual bool update(const sensor_msgs::PointCloud2 &data_in, sensor_msgs::PointCloud2 &data_out) override final;

    protected:
        std::string dynamic_reconfigure_namespace_root_;

        virtual bool execute() = 0;

        pcl::PCLPointCloud2Ptr cloud_out_;
        pcl::PCLPointCloud2Ptr temp_cloud_;

    private:
        tf2_ros::Buffer tf_buffer_;
        std::unique_ptr<tf2_ros::TransformListener> tf_listener_;

        /** \brief Pointer to a dynamic reconfigure service. */
        std::unique_ptr<dynamic_reconfigure::Server<oxin_filters::FilterBaseConfig>> dynamic_reconfigure_srv_;
        dynamic_reconfigure::Server<oxin_filters::FilterBaseConfig>::CallbackType dynamic_reconfigure_clbk_;
        void dynamicReconfigureClbk(oxin_filters::FilterBaseConfig &config, uint32_t level);
        boost::recursive_mutex dynamic_reconfigure_mutex_;

        bool active_ = true;
        std::string input_frame_ = "";
        std::string output_frame_ = "";
    };

    FilterBase::FilterBase()
    {
        cloud_out_ = std::make_shared<pcl::PCLPointCloud2>();
        temp_cloud_ = std::make_shared<pcl::PCLPointCloud2>();
        tf_listener_ = std::make_unique<tf2_ros::TransformListener>(tf_buffer_);
    };

    bool FilterBase::configure()
    {

        if (filters::FilterBase<sensor_msgs::PointCloud2>::getParam(std::string("active"), active_))
        {
            ROS_INFO_NAMED(getName(), "[%s] Using active='%d'", getName().c_str(), active_);
        }

        if (filters::FilterBase<sensor_msgs::PointCloud2>::getParam(std::string("input_frame"), input_frame_))
        {
            ROS_INFO_NAMED(getName(), "[%s] Using input_frame='%s'", getName().c_str(), input_frame_.c_str());
        }

        if (filters::FilterBase<sensor_msgs::PointCloud2>::getParam(std::string("output_frame"), output_frame_))
        {
            ROS_INFO_NAMED(getName(), "[%s] Using output_frame='%s'", getName().c_str(), output_frame_.c_str());
        }

        // WARNING dynamic reconfigure, the base class one. Children can have their own server for their specific values, but
        // be sure to use another namespace to be passed to the dyn server constructor (eg ros::NodeHandle(dynamic_reconfigure_namespace_root_ + "/" + getName())
        dynamic_reconfigure_namespace_root_ = "/filter/" + getName();
        dynamic_reconfigure_srv_ = std::make_unique<dynamic_reconfigure::Server<oxin_filters::FilterBaseConfig>>(
            dynamic_reconfigure_mutex_,
            ros::NodeHandle(dynamic_reconfigure_namespace_root_ + "/base"));

        dynamic_reconfigure_clbk_ = boost::bind(&FilterBase::dynamicReconfigureClbk, this, _1, _2);

        oxin_filters::FilterBaseConfig initial_config;
        initial_config.active = active_;
        initial_config.input_frame = input_frame_;
        initial_config.output_frame = output_frame_;
        dynamic_reconfigure_srv_->setConfigDefault(initial_config);
        dynamic_reconfigure_srv_->updateConfig(initial_config);

        // put this after updateConfig!
        dynamic_reconfigure_srv_->setCallback(dynamic_reconfigure_clbk_);

        return true;
    };

    bool FilterBase::update(const sensor_msgs::PointCloud2 &data_in, sensor_msgs::PointCloud2 &data_out)
    {
        if (active_)
        {

            // TODO: do not transform if already that frame
            if (!input_frame_.empty())
            {
                // Transform to input frame.
                sensor_msgs::PointCloud2 output;
                pcl_ros::transformPointCloud(input_frame_, data_in, output, tf_buffer_);
                pcl_conversions::toPCL(output, *cloud_out_);
            }
            else
            {
                // Convert to PCL point type.
                pcl_conversions::toPCL(data_in, *cloud_out_);
            }

            if (!execute())
            {
                data_out = data_in;
                return false;
            }

            // TODO: do not transform if already that frame
            if (!output_frame_.empty())
            {
                // Transform to output frame.
                pcl_conversions::fromPCL(*cloud_out_, data_out);
                pcl_ros::transformPointCloud(output_frame_, data_out, data_out, tf_buffer_);
            }
            else
            {
                // Convert to ROS point type.
                pcl_conversions::fromPCL(*cloud_out_, data_out);
            }
        }
        else
        {
            data_out = data_in;
        }

        return true;
    };

    void FilterBase::dynamicReconfigureClbk(oxin_filters::FilterBaseConfig &config, uint32_t /*level*/)
    {

        boost::recursive_mutex::scoped_lock lock(dynamic_reconfigure_mutex_);

        if (active_ != config.active)
        {
            active_ = config.active;
            ROS_DEBUG_NAMED(getName(), "[%s] Setting active to: %d.", getName().c_str(), active_);
        }

        if (input_frame_.compare(config.input_frame) != 0)
        {
            input_frame_ = config.input_frame;
            ROS_DEBUG_NAMED(getName(), "[%s] Setting the input TF frame to: %s.", getName().c_str(), input_frame_.c_str());
        }

        if (output_frame_.compare(config.output_frame) != 0)
        {
            output_frame_ = config.output_frame;
            ROS_DEBUG_NAMED(getName(), "[%s] Setting the output TF frame to: %s.", getName().c_str(), output_frame_.c_str());
        }
    }

} // namespace oxin_filters

#endif // FILTER_BASE_POINT_CLOUD_HPP
