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
//#include <cmath>

using namespace std;

//--------------Class------------------------------
class calStats
{
  public: 
    uint strain1_old;
    uint strain1_cur;
    uint strain1_run_sum;
    uint strain1_avg;
    int n;
   

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
  n = 0;
/*  sb_mavProp = nh.subscribe("/arm_4",10, &calStats::mavPropGrab,this);

  pb_calStats = nh.advertise<trims::PROP_stamped>("/arm_4/stamped", 1);*/
}

//------------- CALLBACK function to receive item_order data---------------

void calStats::mavPropGrab(const trims::PROP::ConstPtr& msg)
{
  ROS_INFO("start-----------------");  
  
  strain1_cur = msg->STRAIN1;
  ROS_INFO("CallBackStrain %.2i",strain1_cur);  


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
  calStats calStatsObject; // ROS_INFO(" instantiated object");
  // ros::Subscriber sb_mavProp = nh.subscribe("/arm_4",10, &calStats::mavPropGrab,this);
  ros::Subscriber sb_mavProp = nh.subscribe<>("/arm_4",1, &calStats::mavPropGrab, &calStatsObject);

  calStatsObject.strain1_run_sum = 0;
  calStatsObject.strain1_old = 0;
  calStatsObject.strain1_cur = 0;
  calStatsObject.n = 0;
  calStatsObject.strain1_avg = 0;

  while(ros::ok)
  {

    ros::spinOnce();
    calStatsObject.strain1_old = calStatsObject.strain1_run_sum;
    calStatsObject.strain1_run_sum = calStatsObject.strain1_cur + calStatsObject.strain1_old;
    calStatsObject.n = calStatsObject.n+1;
    calStatsObject.strain1_avg = (int) round(calStatsObject.strain1_run_sum/calStatsObject.n);

    ROS_INFO("The old value is: %.2i",calStatsObject.strain1_old);
    ROS_INFO("The current value is: %.2i",calStatsObject.strain1_cur);
    ROS_INFO("The Run Sum is: %.2i",calStatsObject.strain1_run_sum);
    ROS_INFO("counter is: %.2i",calStatsObject.n);
    ROS_INFO("The average value is: %.2i",calStatsObject.strain1_avg);
    
    loop_rate.sleep();
    // ros::spin();

  }

  return 0;

}

//https://answers.ros.org/question/252763/use-variables-in-main-from-callback/