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
#include <camera_info_manager.h>

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
        camera_info_manager_
        //camera_info_manager (new camera_info_manager::CameraInfoManager(nh))
        
        std::vector<cv::String> fn;
        glob("/home/neu/pylon_camera/Validation/AprilTags/27/*.jpg",fn, false);
        std::vector<Mat> images;
        size_t count = fn.size(); //number of png files in images golder
        for (size_t i=0;i<count;i++)
        images.push_back(imread(fn[i]));
        ros::Rate rate(10);


        while (ros::ok())
        {
          for (std::vector<Mat>::iterator it = images.begin(); it !=images.end(); it++)
          {
            std_msgs::Header header;
            header.frame_id = "";

            sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", *it).toImageMsg();
            

            //sensor_msgs::CameraInfoPtr cam_info(new sensor_msgs::CameraInfo(camera_info_manager_->getCameraInfo()));
            sensor_msgs::CameraInfoPtr cam_info(new sensor_msgs::CameraInfo(camera_info_mng))
            //cam_info->header.stamp = img_raw_msg_.header.stamp;
            // Publish via image_transport
            //img_raw_pub_.publish(img_raw_msg_, *cam_info);
            
            img_pub.publish(msg);
            ros::Duration(1).sleep();
          }
        //  pub.publish(msg);
          ros::spinOnce();
          rate.sleep();
        }

        return 0;
}
