#include <ros/ros.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
//#include <anemometer/ImuConverged.h>
#include <trims/PROP.h>
#include <trims/PROP_stats.h>
#include <geometry_msgs/Twist.h> //Message Type out : Velocity
#include <ctime> //needed for time() in random number generator
#include <unistd.h>
#include <sensor_msgs/Imu.h>
//#include <fcntl.h>  /* File Control Definitions          */
//#include <termios.h>/* POSIX Terminal Control Definitions*/
//#include <unistd.h> /* UNIX Standard Definitions         */
//#include <errno.h>  /* ERROR Number Definitions          */
//#include <sys/ioctl.h> //Use error checks for serial data
#include <cmath>

using namespace std;

//--------------Class------------------------------
class calStats
{
  public: 
    uint strain1_old;
    uint strain1;
    // uint strain1_run_sum;
    // uint strain1_avg;
    // int n;

    int sigma;
    uint EMA_old;
    uint EMA;
    double alpha;
    double EMV;
    double EMV_old;
    double EMSD;

    void mavPropGrab(const trims::PROP::ConstPtr& msg);

  // ros::Subscriber sb_mavProp;
  // ros::Publisher pb_calStats;
  // ros::NodeHandle nh;

   // class callback function



  calStats();

};
//----------------Constructor----------------------------
calStats::calStats()
{

  strain1_old = 0;
  strain1 = 0;
  sigma = 0;
  EMA = strain1;
  EMA_old = 0;
  alpha = .5;
  EMV = 0;
  EMV_old = 0;
  EMSD = 0;
  // n = 0;

/*  sb_mavProp = nh.subscribe("/arm_4",10, &calStats::mavPropGrab,this);

  pb_calStats = nh.advertise<trims::PROP_stamped>("/arm_4/stamped", 1);*/
}

//------------- CALLBACK function to receive item_order data---------------

void calStats::mavPropGrab(const trims::PROP::ConstPtr& msg)
{
  ROS_INFO("start-----------------");  
  
  strain1 = msg->STRAIN1;
  ROS_INFO("CallBackStrain %.2i",strain1);  


  ROS_INFO("done------------------");  
  // ros::Duration(0.25).sleep();  // Sleep for one second

}


// main program---------------------------------------------------------------------------------
int main(int argc, char** argv)
{

  
  ROS_INFO(" initializing calStats node");   // initializing ros
  ros::init(argc, argv, "calStats"); //initiating node with unique node name
  ros::NodeHandle nh;
  ros::Rate loop_rate(10);
  calStats cso; // ROS_INFO(" instantiated object");
  // ros::Subscriber sb_mavProp = nh.subscribe("/arm_4",10, &calStats::mavPropGrab,this);
  ros::Subscriber sb_mavProp = nh.subscribe<>("/arm_4",1, &calStats::mavPropGrab, &cso);

  // cso.strain1_run_sum = 0;
  // cso.strain1_old = 0;
  // cso.strain1 = 0;
  // cso.n = 0;
  // cso.strain1_avg = 0;
  // cso.alpha = 0.5; 

  while(ros::ok)
  {

    ros::spinOnce();
    cso.sigma = cso.strain1 - cso.EMA_old;
    cso.EMA = cso.EMA_old + cso.alpha * cso.sigma;
    cso.EMV = (1-cso.alpha) * (cso.EMV_old + cso.alpha * pow(cso.sigma,2));
    cso.EMSD = pow(cso.EMV, 0.5);
    // cso.strain1_old = cso.strain1_run_sum;
    // cso.strain1_run_sum = cso.strain1 + cso.strain1_old;
    // cso.n = cso.n+1;
    // cso.strain1_avg = (int) round(cso.strain1_run_sum/cso.n);

    // ROS_INFO("The old Run Sum is: %.2i",cso.strain1_old);
    ROS_INFO("The current value is: %.2i",cso.strain1);
    ROS_INFO("sigma is: %.2i",cso.sigma);
    ROS_INFO("alpha is: %2.2f",cso.alpha);
    
    // ROS_INFO("The new Run Sum is: %.2i",cso.strain1_run_sum);
    // ROS_INFO("counter is: %.2i",cso.n);
    // ROS_INFO("The average value is: %.2i",cso.strain1_avg);

    ROS_INFO("current EMA is: %.2i",cso.EMA);
    ROS_INFO("old EMA is: %.2i",cso.EMA_old);
    ROS_INFO("current EMV is: %.2f",cso.EMV);
    ROS_INFO("old EMV is: %.2f",cso.EMV_old);
    ROS_INFO("EMSD is: %.2f",cso.EMSD);

    cso.EMA_old = cso.EMA;
    cso.EMV_old = cso.EMV;

    loop_rate.sleep();
    // ros::spin();

  }

  return 0;

}

//https://answers.ros.org/question/252763/use-variables-in-main-from-callback/
//https://en.wikipedia.org/wiki/Moving_average#Exponentially_weighted_moving_variance_and_standard_deviation