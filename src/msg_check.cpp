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
#include <trims/PROP_check.h>
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
class checker
{
  ros::Subscriber sb_mavProp;
  ros::Publisher pb_mavPropCheck;
  ros::NodeHandle nh;

 
  // class callback function
  void mavPropGrab(const trims::PROP msg);

  public: 
    checker();

};
//----------------Constructor----------------------------
checker::checker()
{
 
  sb_mavProp = nh.subscribe("/arm_4",10, &checker::mavPropGrab,this);

  pb_mavPropCheck = nh.advertise<trims::PROP_check>("/arm_4/check", 1);
}


//------------- CALLBACK function to receive item_order data---------------

void checker::mavPropGrab(const trims::PROP msg)
{
  // ROS_INFO("Hey Im in the Callback!");  
  
  ros::Time check_start_time = ros::Time::now();

  trims::PROP_check checkVals;

  checkVals.header.stamp = check_start_time;

  unsigned int v_ADC_min = 3500;
  unsigned int v_ADC_max = 4000;

  unsigned int c_ADC_min = 400;
  unsigned int c_ADC_max = 1200;

  unsigned int t_ADC_min = 20000;
  unsigned int t_ADC_max = 24000;

  unsigned int s_ADC_min = 10000;
  unsigned int s_ADC_max = 24000;

  unsigned int r_min = 10;
  unsigned int r_max = 8000;


  if(msg.V1 > v_ADC_max){checkVals.V1 = "ERROR High";}else if(msg.V1 < v_ADC_min){checkVals.V1 = "ERROR Low";}else{checkVals.V1 = "OK";}
  if(msg.V2 > v_ADC_max){checkVals.V2 = "ERROR High";}else if(msg.V2 < v_ADC_min){checkVals.V2 = "ERROR Low";}else{checkVals.V2 = "OK";}
  if(msg.V3 > v_ADC_max){checkVals.V3 = "ERROR High";}else if(msg.V3 < v_ADC_min){checkVals.V3 = "ERROR Low";}else{checkVals.V3 = "OK";}
  if(msg.V4 > v_ADC_max){checkVals.V4 = "ERROR High";}else if(msg.V4 < v_ADC_min){checkVals.V4 = "ERROR Low";}else{checkVals.V4 = "OK";}

  if(msg.C1 > c_ADC_max){checkVals.C1 = "ERROR High";}else if(msg.C1 < c_ADC_min){checkVals.C1 = "ERROR Low";}else{checkVals.C1 = "OK";}
  if(msg.C2 > c_ADC_max){checkVals.C2 = "ERROR High";}else if(msg.C2 < c_ADC_min){checkVals.C2 = "ERROR Low";}else{checkVals.C2 = "OK";}
  if(msg.C3 > c_ADC_max){checkVals.C3 = "ERROR High";}else if(msg.C3 < c_ADC_min){checkVals.C3 = "ERROR Low";}else{checkVals.C3 = "OK";}
  if(msg.C4 > c_ADC_max){checkVals.C4 = "ERROR High";}else if(msg.C4 < c_ADC_min){checkVals.C4 = "ERROR Low";}else{checkVals.C4 = "OK";}

  if(msg.THRUST1 > t_ADC_max){checkVals.THRUST1 = "ERROR High";}else if(msg.THRUST1 < t_ADC_min){checkVals.THRUST1 = "ERROR Low";}else{checkVals.THRUST1 = "OK";}
  if(msg.THRUST2 > t_ADC_max){checkVals.THRUST2 = "ERROR High";}else if(msg.THRUST2 < t_ADC_min){checkVals.THRUST2 = "ERROR Low";}else{checkVals.THRUST2 = "OK";}
  if(msg.THRUST3 > t_ADC_max){checkVals.THRUST3 = "ERROR High";}else if(msg.THRUST3 < t_ADC_min){checkVals.THRUST3 = "ERROR Low";}else{checkVals.THRUST3 = "OK";}
  if(msg.THRUST4 > t_ADC_max){checkVals.THRUST4 = "ERROR High";}else if(msg.THRUST4 < t_ADC_min){checkVals.THRUST4 = "ERROR Low";}else{checkVals.THRUST4 = "OK";}

  if(msg.STRAIN1 > s_ADC_max){checkVals.STRAIN1 = "ERROR High";}else if(msg.STRAIN1 < s_ADC_min){checkVals.STRAIN1 = "ERROR Low";}else{checkVals.STRAIN1 = "OK";}
  if(msg.STRAIN2 > s_ADC_max){checkVals.STRAIN2 = "ERROR High";}else if(msg.STRAIN2 < s_ADC_min){checkVals.STRAIN2 = "ERROR Low";}else{checkVals.STRAIN2 = "OK";}
  if(msg.STRAIN3 > s_ADC_max){checkVals.STRAIN3 = "ERROR High";}else if(msg.STRAIN3 < s_ADC_min){checkVals.STRAIN3 = "ERROR Low";}else{checkVals.STRAIN3 = "OK";}
  if(msg.STRAIN4 > s_ADC_max){checkVals.STRAIN4 = "ERROR High";}else if(msg.STRAIN4 < s_ADC_min){checkVals.STRAIN4 = "ERROR Low";}else{checkVals.STRAIN4 = "OK";}

  if(msg.RPM1 > r_max){checkVals.RPM1 = "ERROR High";}else if(msg.RPM1 < r_min){checkVals.RPM1 = "ERROR Low";}else{checkVals.RPM1 = "OK";}
  if(msg.RPM2 > r_max){checkVals.RPM2 = "ERROR High";}else if(msg.RPM2 < r_min){checkVals.RPM2 = "ERROR Low";}else{checkVals.RPM2 = "OK";}
  if(msg.RPM3 > r_max){checkVals.RPM3 = "ERROR High";}else if(msg.RPM3 < r_min){checkVals.RPM3 = "ERROR Low";}else{checkVals.RPM3 = "OK";}
  if(msg.RPM4 > r_max){checkVals.RPM4 = "ERROR High";}else if(msg.RPM4 < r_min){checkVals.RPM4 = "ERROR Low";}else{checkVals.RPM4 = "OK";}


  pb_mavPropCheck.publish(checkVals);
  ros::Duration(0.25).sleep();  // Sleep for one second

  // ROS_INFO("Call Back done!");  

}

// main program---------------------------------------------------------------------------------
int main(int argc, char** argv)
{

  // initializing ros
  ROS_INFO(" initializing checker node");
  ros::init(argc, argv, "checker"); //initiating node with unique node name
  
  // ROS_INFO(" instantiated object");
  checker checkerObject;

  ros::spin();
  return 0;

}
