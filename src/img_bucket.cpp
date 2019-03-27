#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include "std_msgs/Float64.h"

using namespace cv;
/*void msgCallback(const sensor_msgs::Image::ConstPtr& msg)
{
        ROS_INFO("height = %d, width = %d",msg->height, msg->width);
}
*/
int main(int argc, char** argv)
{
        ros::init(argc,argv,"img_bucket");
        ros::NodeHandle nh;
        image_transport::ImageTransport imgTrs(nh);
        ros::Publisher pub = nh.advertise<std_msgs::Float64>("test_topic", 100);
        image_transport::Publisher img_pub = imgTrs.advertise("bucket", 1);
        //ros::Publisher pub2 = nh.advertise<sensor_msgs::Image>("image_topic", 100);
        //ros::Subscriber cam_sub = nh.subscribe("/usb_cam/image_raw",100,msgCallback);
        //ros::spin();
        std::vector<cv::String> fn;
        glob("/home/neu/pylon_camera/Validation/AprilTags/27/*.jpg",fn, false);
        std::vector<Mat> images;
        size_t count = fn.size(); //number of png files in images golder
        for (size_t i=0;i<count;i++)
        images.push_back(imread(fn[i]));
        ros::Rate rate(10);

        //std_msgs::Float64 msg;
      


        //msg.data = static_cast<float>(count);

        while (ros::ok())
        {
          for (std::vector<Mat>::iterator it = images.begin(); it !=images.end(); it++)
          {
            std_msgs::Header header;
            header.frame_id = "";

            sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", *it).toImageMsg();
            img_pub.publish(msg);
            //ros::Duration(1).sleep();
          }
        //  pub.publish(msg);
          ros::spin();
          rate.sleep();
        }

        return 0;
}
