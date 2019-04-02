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
    
        float dist[5] = {-0.011276, -0.002467, -0.004967, 0.033951, 0.000000};
        float camera[3][3] = {
          { 2677.117911, 0.000000, 937.740481 }, 
          { 0.000000, 2638.726872, 455.023139 }, 
          { 0.000000, 0.000000, 1.000000 }
          };

        float rect[3][3] = {
          { 1.000000, 0.000000, 0.000000 }, 
          { 0.000000, 1.000000, 0.000000 }, 
          { 0.000000, 0.000000, 1.000000 }
          };
        
        float proj[3][4] = {
          { 2615.817139, 0.000000, 952.909396, 0.000000 }, 
          { 0.000000, 2657.769043, 453.327589, 0.000000 }, 
          { 0.000000, 0.000000, 1.000000, 0.000000 }
        };

        

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
            
            sensor_msgs::CameraInfo cam_msg;
            cam_msg.header.stamp = ros::Time::now();
            cam_msg.header.frame_id = "basler";
            cam_msg.height = 1024;
            cam_msg.width = 1280;
            cam_msg.distortion_model = "plumb_bob";
            
            for (int i=0; i<5; i++)
            {
              cam_msg.D[i] = dist[i];
            }
            
            for(int i=0; i<3; i++)
            { for (int j=0; j<3; j++)
              сam_msg.K[i][j] = rect[i][j];
            }
            

               for(int i=0; i<3; i++)
            {
              for (int j=0; j<3; j++)
              cam_msg.R[i][j] = rect[i][j];
            }

                for(int i=0; i<3; i++)
            {
              for (int j=0; j<4; j++)
              cam_msg.P[i][j] = proj[i][j];
            }

            // Publish via image_transport
            //img_raw_pub_.publish(img_raw_msg_, *cam_info);
            
            img_pub.publish(msg,cam_msg);
            ros::Duration(1).sleep();
          }
        //  pub.publish(msg);
          ros::spinOnce();
          rate.sleep();
        }

        return 0;
}
