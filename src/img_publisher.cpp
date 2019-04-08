#include "sensor_msgs/Image.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include "std_msgs/Float64.h"
#include "img_publisher.h"

Img_stream::Img_stream(): imgTrs(nh)
{
  cam_pub = imgTrs.advertiseCamera("image_raw",1);
}

bool Img_stream::load_img(std::string filter, std::vector<cv::Mat> &images)
{ 
  // FIND PARAMETER NAME I.E> Wheelbase
  std::string path;
  nh.param(param_name, path, default_path);
  cv::glob(path + filter, fn, false);
  ROS_INFO_STREAM(path+filter);
  size_t count = fn.size();
  bool b = false;
  for (size_t i=0;i<count;i++)
  {
    cv::Mat src = cv::imread(fn[i]);
    if (!src.data)
      {
        ROS_INFO_STREAM("PROBLEM LOADING IMAGES!");
        b = false;
      }
    else
    {
      images.push_back(cv::imread(fn[i]));
      if (i==count - 1)
      b = true;
    }
    
  }
  return b;
}


int main(int argc, char** argv)
{
        ros::init(argc,argv,"img_streaming");
        std::vector<cv::Mat> images;
        Img_stream img_str;
        if (img_str.load_img(img_str.default_filter, images)) 
        {
             ros::Rate rate(10);
            while (ros::ok())
            {
                for (std::vector<cv::Mat>::iterator it = images.begin(); it !=images.end(); it++)
              {
                      std_msgs::Header header;
                      header.frame_id = "";
                      sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", *it).toImageMsg();
                      sensor_msgs::CameraInfoPtr cam_msg(new sensor_msgs::CameraInfo());
                      cam_msg->header.stamp = ros::Time::now();
                      cam_msg->header.frame_id = "basler";
                      cam_msg->height = 1024;
                      cam_msg->width = 1280;
                      cam_msg->distortion_model = "plumb_bob";
                      
                      for (int i=0; i<5; i++)
                      {
                        cam_msg->D[i] = img_str.dist[i];
                      }
                      
                      for(int i=0; i<9; i++)
                      { 
                        cam_msg->K[i] = img_str.camera[i];
                        cam_msg->R[i] = img_str.rect[i];
                      }
                      
                      for(int i=0; i<12; i++)
                      {
                          cam_msg->P[i] = img_str.proj[i];
                      }
                      img_str.cam_pub.publish(msg, cam_msg);
                      ros::Duration(1).sleep();
                    }
                    ros::spinOnce();
                    rate.sleep();
                }   

          }
        else 
        {
          ROS_INFO_STREAM("Failed to load images. Exit program");
        }


        return 0;
}
