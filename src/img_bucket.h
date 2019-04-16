#ifndef IMG_BUCKET
#define IMG_BUCKET

#include "ros/ros.h"
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include "ros/package.h"


class Img_stream {
    public:
    Img_stream();
    const float dist[5] = {-0.011276, -0.002467, -0.004967, 0.033951, 0.000000};
    const float camera[9] = { 2677.117911, 0.000000, 937.740481, 0.000000, 2638.726872, 455.023139, 0.000000, 0.000000, 1.000000 };
    const float rect[9]= {1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000 };
    const float proj[12] = {2615.817139, 0.000000, 952.909396, 0.000000, 0.000000, 2657.769043, 453.327589, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000 };
    const std::string default_filter = "*.jpg";
    std::string default_path = ros::package::getPath("img_bucket") + "/test/";
    std::vector<cv::String> fn;
    bool load_img(std::string filter, std::vector<cv::Mat> &images);
    image_transport::CameraPublisher cam_pub;
    std::string param_name = "path"; 

    private:
    ros::NodeHandle nh;
    image_transport::ImageTransport imgTrs;
    
   

};

#endif // IMG_BUCKET
