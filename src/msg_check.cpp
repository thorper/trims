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
  
  trims::PROP_check checkVals;

  unsigned int v_ADC_min = 3500;
  unsigned int v_ADC_max = 4000;

  unsigned int c_ADC_min = 400;
  unsigned int c_ADC_max = 1200;

  unsigned int t_ADC_min = 20000;
  unsigned int t_ADC_max = 24000;

  unsigned int s_ADC_min = 10000;
  unsigned int s_ADC_max = 24000;

  unsigned int r_min = 0;
  unsigned int r_max = 8000;


  if(msg.V1 > v_ADC_min && msg.V1 < v_ADC_max){checkVals.V1 = "OK";}
  else{checkVals.V1 = "BAD";} 
  if(msg.V2 > v_ADC_min && msg.V2 < v_ADC_max){checkVals.V2 = "OK";}
  else{checkVals.V2 = "BAD";} 
  if(msg.V3 > v_ADC_min && msg.V3 < v_ADC_max){checkVals.V3 = "OK";}
  else{checkVals.V3 = "BAD";} 
  if(msg.V4 > v_ADC_min && msg.V4 < v_ADC_max){checkVals.V4 = "OK";}
  else{checkVals.V4 = "BAD";} 

  if(msg.C1 > c_ADC_min && msg.C1 < c_ADC_max){checkVals.C1 = "OK";}
  else{checkVals.C1 = "BAD";} 
  if(msg.C2 > c_ADC_min && msg.C2 < c_ADC_max){checkVals.C2 = "OK";}
  else{checkVals.C2 = "BAD";} 
  if(msg.C3 > c_ADC_min && msg.C3 < c_ADC_max){checkVals.C3 = "OK";}
  else{checkVals.C3 = "BAD";} 
  if(msg.C4 > c_ADC_min && msg.C4 < c_ADC_max){checkVals.C4 = "OK";}
  else{checkVals.C4 = "BAD";} 

  if(msg.THRUST1 > t_ADC_min && msg.THRUST1 < t_ADC_max){checkVals.THRUST1 = "OK";}
  else{checkVals.THRUST1 = "BAD";} 
  if(msg.THRUST2 > t_ADC_min && msg.THRUST2 < t_ADC_max){checkVals.THRUST2 = "OK";}
  else{checkVals.THRUST2 = "BAD";} 
  if(msg.THRUST3 > t_ADC_min && msg.THRUST3 < t_ADC_max){checkVals.THRUST3 = "OK";}
  else{checkVals.THRUST3 = "BAD";} 
  if(msg.THRUST4 > t_ADC_min && msg.THRUST1 < t_ADC_max){checkVals.THRUST4 = "OK";}
  else{checkVals.THRUST4 = "BAD";} 

  if(msg.STRAIN1 > s_ADC_min && msg.STRAIN1 < s_ADC_max){checkVals.STRAIN1 = "OK";}
  else{checkVals.STRAIN1 = "BAD";} 
  if(msg.STRAIN2 > s_ADC_min && msg.STRAIN2 < s_ADC_max){checkVals.STRAIN2 = "OK";}
  else{checkVals.STRAIN2 = "BAD";} 
  if(msg.STRAIN3 > s_ADC_min && msg.STRAIN3 < s_ADC_max){checkVals.STRAIN3 = "OK";}
  else{checkVals.STRAIN3 = "BAD";} 
  if(msg.STRAIN4 > s_ADC_min && msg.STRAIN1 < s_ADC_max){checkVals.STRAIN4 = "OK";}
  else{checkVals.STRAIN4 = "BAD";} 

  if(msg.RPM1 > r_min && msg.RPM1 < r_max){checkVals.RPM1 = "OK";}
  else{checkVals.RPM1 = "BAD";} 
  if(msg.RPM2 > r_min && msg.RPM2 < r_max){checkVals.RPM2 = "OK";}
  else{checkVals.RPM2 = "BAD";} 
  if(msg.RPM3 > r_min && msg.RPM3 < r_max){checkVals.RPM3 = "OK";}
  else{checkVals.RPM3 = "BAD";} 
  if(msg.RPM4 > r_min && msg.RPM4 < r_max){checkVals.RPM4 = "OK";}
  else{checkVals.RPM4 = "BAD";} 


  pb_mavPropCheck.publish(checkVals);

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