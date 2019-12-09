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
#include <trims/PROP_eng_units.h>
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
class engUnits
{
  ros::Subscriber sb_mavProp;
  ros::Publisher pb_mavPropEngUnits;
  ros::NodeHandle nh;

   // class callback function
  void mavPropGrab(const trims::PROP msg);

  public: 
    engUnits();

};
//----------------Constructor----------------------------
engUnits::engUnits()
{
 
  sb_mavProp = nh.subscribe("/arm_4",10, &engUnits::mavPropGrab,this);

  pb_mavPropEngUnits = nh.advertise<trims::PROP_eng_units>("/arm_4/engUnits", 1);
}

//------------- CALLBACK function to receive item_order data---------------

void engUnits::mavPropGrab(const trims::PROP msg)
{
  // ROS_INFO("Hey Im in the Callback!");  
  
  ros::Time units_start_time = ros::Time::now();

  trims::PROP_eng_units engUnitVals;

  engUnitVals.header.stamp = units_start_time;

//---------------------------------------------
  double v_m_1 = 0.006350; //volts/bit
  double v_m_2 = 0.006331;
  double v_m_3 = 0.006327;
  double v_m_4 = 0.006340;

  double v_b_1 = 0.012859; //volts
  double v_b_2 = 0.004312;
  double v_b_3 = 0.016666;
  double v_b_4 = 0.008379;

//---------------------------------------------
  double c_b = 0.02035; //amps/bit
  int c_offset = 410; //zero amp adc reading

//---------------------------------------------
  double t_m_1 = 0; //newtons/bit
  double t_m_2 = 0;
  double t_m_3 = 0;
  double t_m_4 = 0;

  double t_o_1 = 0; //zero thrust adc reading
  double t_o_2 = 0;
  double t_o_3 = 0;
  double t_o_4 = 0;

//---------------------------------------------
  double s_m_1 = -0.002282; //newtons/bit
  double s_m_2 = -0.002076;
  double s_m_3 = -0.002450;
  double s_m_4 = -0.002344;

  double s_o_1 = 21292; //zero thrust adc reading
  double s_o_2 = 19291;
  double s_o_3 = 21626;
  double s_o_4 = 20252;


  engUnitVals.V1 = msg.V1*v_m_1+v_b_1;
  engUnitVals.V2 = msg.V2*v_m_2+v_b_2;
  engUnitVals.V3 = msg.V3*v_m_3+v_b_3;
  engUnitVals.V4 = msg.V4*v_m_4+v_b_4;

  engUnitVals.C1 = (msg.C1-c_offset)*c_b;
  engUnitVals.C2 = (msg.C2-c_offset)*c_b;
  engUnitVals.C3 = (msg.C3-c_offset)*c_b;
  engUnitVals.C4 = (msg.C4-c_offset)*c_b;

  engUnitVals.THRUST1 = (msg.THRUST1-t_o_1)*t_m_1;
  engUnitVals.THRUST2 = (msg.THRUST2-t_o_2)*t_m_2;
  engUnitVals.THRUST3 = (msg.THRUST3-t_o_3)*t_m_3;
  engUnitVals.THRUST4 = (msg.THRUST4-t_o_4)*t_m_4;

  engUnitVals.STRAIN1 = (msg.STRAIN1-s_o_1)*s_m_1;
  engUnitVals.STRAIN2 = (msg.STRAIN2-s_o_2)*s_m_2;
  engUnitVals.STRAIN3 = (msg.STRAIN3-s_o_3)*s_m_3;
  engUnitVals.STRAIN4 = (msg.STRAIN4-s_o_4)*s_m_4;

  engUnitVals.RPM1 = msg.RPM1;
  engUnitVals.RPM2 = msg.RPM2;
  engUnitVals.RPM3 = msg.RPM3;
  engUnitVals.RPM4 = msg.RPM4;

  pb_mavPropEngUnits.publish(engUnitVals);

  // ROS_INFO("Call Back done!");  
  ros::Duration(0.25).sleep();  // Sleep for one second

}

// main program---------------------------------------------------------------------------------
int main(int argc, char** argv)
{

  // initializing ros
  ROS_INFO(" initializing engUnits node");
  ros::init(argc, argv, "engUnits"); //initiating node with unique node name
  
  // ROS_INFO(" instantiated object");
  engUnits engUnitsObject;

  ros::spin();
  return 0;

}
