#include <velocity_planner/velocity_visualizer.h>
#include <color_names/color_names.h>

namespace velocity_planner
{
    VelocityVisualizer::VelocityVisualizer(std::string node_name)
    : generator_(0.0)
    {
        node_name_ = node_name;
    }

    visualization_msgs::msg::MarkerArray VelocityVisualizer::generateMarker(
        hermite_path_msgs::msg::HermitePathStamped path)
    {
        std_msgs::msg::ColorRGBA default_ref_color = color_names::makeColorMsg("lime",0.8);
        return generateMarker(path,default_ref_color);
    }

    visualization_msgs::msg::MarkerArray VelocityVisualizer::generateMarker(
        hermite_path_msgs::msg::HermitePathStamped path,
        std_msgs::msg::ColorRGBA color_ref_velocity)
    {
        visualization_msgs::msg::MarkerArray marker;
        std::sort
        (
            path.reference_velocity.begin(),
            path.reference_velocity.end(),
            [](const auto& x, const auto& y)
                {return x.t < y.t;}
        );
        for(auto itr = path.reference_velocity.begin(); itr != path.reference_velocity.end(); itr++)
        {
            visualization_msgs::msg::Marker box_marker;
            box_marker.header = path.header;
            box_marker.ns = "reference_velocity";
            box_marker.type = box_marker.CUBE;
            std::size_t index = std::distance(path.reference_velocity.begin(), itr);
            box_marker.id = index;
            geometry_msgs::msg::Point point = generator_.getPointOnHermitePath(path.path,itr->t);
            box_marker.action = box_marker.ADD;
            box_marker.pose.position = point;
            box_marker.pose.position.z = box_marker.pose.position.z + itr->linear_velocity*0.5;
            box_marker.scale.x = 0.1;
            box_marker.scale.y = 0.1;
            box_marker.scale.z = itr->linear_velocity;
            box_marker.color = color_ref_velocity;
            marker.markers.push_back(box_marker);
        }
        return marker;
    }
}