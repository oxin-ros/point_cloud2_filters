#ifndef RADIUS_OUTLIER_REMOVAL_FILTER_HPP
#define RADIUS_OUTLIER_REMOVAL_FILTER_HPP

#include <oxin_filters/FilterIndices.hpp>
#include <pcl/filters/radius_outlier_removal.h>

#include <oxin_filters/RadiusOutlierRemovalConfig.h>

namespace oxin_filters
{

    class RadiusOutlierRemovalFilter : public FilterIndices
    {
    public:
        RadiusOutlierRemovalFilter();
        ~RadiusOutlierRemovalFilter() = default;

    public:
        virtual bool configure() override;

    private:
        std::shared_ptr<pcl::RadiusOutlierRemoval<pcl::PCLPointCloud2>> ror_;

        int min_neighbors_ = 5;
        double radius_search_ = 0.1;

        /** \brief Pointer to a dynamic reconfigure service. */
        std::unique_ptr<dynamic_reconfigure::Server<oxin_filters::RadiusOutlierRemovalConfig>> dynamic_reconfigure_srv_;
        dynamic_reconfigure::Server<oxin_filters::RadiusOutlierRemovalConfig>::CallbackType dynamic_reconfigure_clbk_;
        void dynamicReconfigureClbk(oxin_filters::RadiusOutlierRemovalConfig &config, uint32_t level);
        boost::recursive_mutex dynamic_reconfigure_mutex_;
    };

    RadiusOutlierRemovalFilter::RadiusOutlierRemovalFilter() : FilterIndices()
    {
        filter_ = std::make_shared<pcl::RadiusOutlierRemoval<pcl::PCLPointCloud2>>();
    };

    bool RadiusOutlierRemovalFilter::configure()
    {
        FilterIndices::configure();

        ror_ = std::dynamic_pointer_cast<pcl::RadiusOutlierRemoval<pcl::PCLPointCloud2>>(filter_);

        filters::FilterBase<sensor_msgs::PointCloud2>::getParam(std::string("min_neighbors"), min_neighbors_);
        filters::FilterBase<sensor_msgs::PointCloud2>::getParam(std::string("radius_search"), radius_search_);

        ror_->setMinNeighborsInRadius(min_neighbors_);
        ror_->setRadiusSearch(radius_search_);

        // dynamic reconfigure
        dynamic_reconfigure_srv_ = std::make_unique<dynamic_reconfigure::Server<oxin_filters::RadiusOutlierRemovalConfig>>(
            dynamic_reconfigure_mutex_,
            ros::NodeHandle(dynamic_reconfigure_namespace_root_ + "/" + getName()));

        dynamic_reconfigure_clbk_ = boost::bind(&RadiusOutlierRemovalFilter::dynamicReconfigureClbk, this, _1, _2);

        oxin_filters::RadiusOutlierRemovalConfig initial_config;
        initial_config.min_neighbors = min_neighbors_;
        initial_config.radius_search = radius_search_;

        dynamic_reconfigure_srv_->setConfigDefault(initial_config);
        dynamic_reconfigure_srv_->updateConfig(initial_config);

        // put this after updateConfig!
        dynamic_reconfigure_srv_->setCallback(dynamic_reconfigure_clbk_);

        return true;
    };

    void RadiusOutlierRemovalFilter::dynamicReconfigureClbk(oxin_filters::RadiusOutlierRemovalConfig &config, uint32_t /*level*/)
    {

        boost::recursive_mutex::scoped_lock lock(dynamic_reconfigure_mutex_);

        if (min_neighbors_ != config.min_neighbors)
        {
            min_neighbors_ = config.min_neighbors;
            ROS_DEBUG_NAMED(getName(), "[%s] Setting the number of neighbors in radius: %d.", getName().c_str(), min_neighbors_);
        }

        if (radius_search_ != config.radius_search)
        {
            radius_search_ = config.radius_search;
            ROS_DEBUG_NAMED(getName(), "[%s] Setting the radius to search neighbors: %f.", getName().c_str(), radius_search_);
        }

        ror_->setMinNeighborsInRadius(min_neighbors_);
        ror_->setRadiusSearch(radius_search_);
    }

} // namespace oxin_filters

#endif // RADIUS_OUTLIER_REMOVAL_FILTER_HPP
