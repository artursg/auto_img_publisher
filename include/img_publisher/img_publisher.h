#include <ros/package.h>
#include "ros/ros.h"
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>


const std::string default_filter = "*.jpg";
std::string default_path;
default_path = ros::package::getPath("img_bucket") + "test/";
std::string path;
std::string filter;
nh.param(param_name, path, default_path)
cv::glob(path + filter, fn, false);