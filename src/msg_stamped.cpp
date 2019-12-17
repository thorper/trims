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
#include <trims/PROP_stamped.h>
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
class propStamper
{
  ros::Subscriber sb_mavProp;
  ros::Publisher pb_mavPropStamper;
  ros::NodeHandle nh;

   // class callback function
  void mavPropGrab(const trims::PROP msg);

  public: 
    propStamper();

};
//----------------Constructor----------------------------
propStamper::propStamper()
{
 
  sb_mavProp = nh.subscribe("/arm_4",10, &propStamper::mavPropGrab,this);

  pb_mavPropStamper = nh.advertise<trims::PROP_stamped>("/arm_4/stamped", 1);
}

//------------- CALLBACK function to receive item_order data---------------

void propStamper::mavPropGrab(const trims::PROP msg)
{
  // ROS_INFO("Hey Im in the Callback!");  
  
  ros::Time stamped_start_time = ros::Time::now();

  trims::PROP_stamped stampedVals;

  stampedVals.header.stamp = stamped_start_time;

  stampedVals.V1 = msg.V1;
  stampedVals.V2 = msg.V2;
  stampedVals.V3 = msg.V3;
  stampedVals.V4 = msg.V4;

  stampedVals.C1 = msg.C1;
  stampedVals.C2 = msg.C2;
  stampedVals.C3 = msg.C3;
  stampedVals.C4 = msg.C4;
  stampedVals.THRUST1 = msg.THRUST1;
  stampedVals.THRUST2 = msg.THRUST2;
  stampedVals.THRUST3 = msg.THRUST3;
  stampedVals.THRUST4 = msg.THRUST4;

  stampedVals.STRAIN1 = msg.STRAIN1;
  stampedVals.STRAIN2 = msg.STRAIN2;
  stampedVals.STRAIN3 = msg.STRAIN3;
  stampedVals.STRAIN4 = msg.STRAIN4;

  stampedVals.RPM1 = msg.RPM1;
  stampedVals.RPM2 = msg.RPM2;
  stampedVals.RPM3 = msg.RPM3;
  stampedVals.RPM4 = msg.RPM4;

  pb_mavPropStamper.publish(stampedVals);

  // ROS_INFO("Call Back done!");  
  // ros::Duration(0.25).sleep();  // Sleep for one second

}

// main program---------------------------------------------------------------------------------
int main(int argc, char** argv)
{

  // initializing ros
  ROS_INFO(" initializing propStamper node");
  ros::init(argc, argv, "propStamper"); //initiating node with unique node name
  
  // ROS_INFO(" instantiated object");
  propStamper propStamperObject;

  ros::spin();
  return 0;

}
