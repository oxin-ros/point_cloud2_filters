#ifndef FILTER_POINT_CLOUD_HPP
#define FILTER_POINT_CLOUD_HPP

#include <oxin_filters/FilterBase.hpp>
#include <pcl/filters/filter.h>

namespace oxin_filters
{

    class Filter : public FilterBase
    {
    public:
        Filter() = default;
        ~Filter() = default;

    public:
    protected:
        std::shared_ptr<pcl::Filter<pcl::PCLPointCloud2>> filter_;

        virtual bool execute() override;

    private:
    };

    bool Filter::execute()
    {
        filter_->setInputCloud(cloud_out_);
        filter_->filter(*temp_cloud_);
        pcl::copyPointCloud(*temp_cloud_, *cloud_out_);
        return true;
    };

} // namespace oxin_filters

#endif // FILTER_POINT_CLOUD_HPP
