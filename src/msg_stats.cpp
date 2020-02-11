#include <ros/ros.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <trims/PROP.h>
#include <trims/PROP_stats.h>
#include <geometry_msgs/Twist.h> //Message Type out : Velocity
#include <ctime> //needed for time() in random number generator
#include <unistd.h>
#include <sensor_msgs/Imu.h>
#include <cmath>

using namespace std;

//--------------Class------------------------------
class calStats
{
  public: 
    //Strain gauge signal (STRAINX)
    uint s_old[4];
    uint s [4];
    int s_sigma [4];
    uint s_EMA_old [4];
    uint s_EMA [4];
    double s_alpha [4];
    double s_EMV [4];
    double s_EMV_old [4];
    double s_EMSD [4];

    //Load Cell signal (THRUSTX)
    uint t_old[4];
    uint t [4];
    int t_sigma [4];
    uint t_EMA_old [4];
    uint t_EMA [4];
    double t_alpha [4];
    double t_EMV [4];
    double t_EMV_old [4];
    double t_EMSD [4];

    void mavPropGrab(const trims::PROP::ConstPtr& msg);


  calStats();

};
//----------------Constructor----------------------------
calStats::calStats()
{
  //Strain gauge signal (STRAINX)
  s_old[0] = 0; s_old[1] = 0; s_old[2] = 0; s_old[3] = 0;
  s[0] = 0; s[1] = 0; s[2] = 0; s[3] = 0;
  s_sigma[0] = 0; s_sigma[1] = 0; s_sigma[2] = 0; s_sigma[3] = 0;
  s_EMA[0] = s[0]; s_EMA[1] = s[1]; s_EMA[2] = s[2]; s_EMA[3] = s[3];
  s_EMA_old[0] = 0; s_EMA_old[1] = 0; s_EMA_old[2] = 0; s_EMA_old[3] = 0;

  s_alpha[0] = 0.5; s_alpha[1] = 0.5; s_alpha[2] = 0.5; s_alpha[3] = 0.5;
  
  s_EMV[0] = 0; s_EMV[1] = 0; s_EMV[2] = 0; s_EMV[3] = 0;
  s_EMV_old[0] = 0; s_EMV_old[1] = 0; s_EMV_old[2] = 0; s_EMV_old[3] = 0;
  s_EMSD[0] = 0; s_EMSD[1] = 0; s_EMSD[2] = 0; s_EMSD[3] = 0;

  //Load Cell signal (THRUSTX)
  t_old[0] = 0; t_old[1] = 0; t_old[2] = 0; t_old[3] = 0;
  t[0] = 0; t[1] = 0; t[2] = 0; t[3] = 0;
  t_sigma[0] = 0; t_sigma[1] = 0; t_sigma[2] = 0; t_sigma[3] = 0;
  t_EMA[0] = s[0]; t_EMA[1] = s[1]; t_EMA[2] = s[2]; t_EMA[3] = s[3];
  t_EMA_old[0] = 0; t_EMA_old[1] = 0; t_EMA_old[2] = 0; t_EMA_old[3] = 0;

  t_alpha[0] = 0.5; t_alpha[1] = 0.5; t_alpha[2] = 0.5; t_alpha[3] = 0.5;
  
  t_EMV[0] = 0; t_EMV[1] = 0; t_EMV[2] = 0; t_EMV[3] = 0;
  t_EMV_old[0] = 0; t_EMV_old[1] = 0; t_EMV_old[2] = 0; t_EMV_old[3] = 0;
  t_EMSD[0] = 0; t_EMSD[1] = 0; t_EMSD[2] = 0; t_EMSD[3] = 0;


}

//------------- CALLBACK function to receive item_order data---------------

void calStats::mavPropGrab(const trims::PROP::ConstPtr& msg)
{
  ROS_INFO("start-----------------");  
  //Strain gauge signal (STRAINX) 
  s[0] = msg->STRAIN1;
  s[1] = msg->STRAIN2;
  s[2] = msg->STRAIN3;
  s[3] = msg->STRAIN4;
  ROS_INFO("STRAIN1 %.2i",s[0]);  
  ROS_INFO("STRAIN2 %.2i",s[1]);  
  ROS_INFO("STRAIN3 %.2i",s[2]);  
  ROS_INFO("STRAIN4 %.2i",s[3]);  
  //Load Cell signal (THRUSTX)
  t[0] = msg->THRUST1;
  t[1] = msg->THRUST2;
  t[2] = msg->THRUST3;
  t[3] = msg->THRUST4;
  ROS_INFO("THRUST1 %.2i",t[0]);  
  ROS_INFO("THRUST2 %.2i",t[1]);  
  ROS_INFO("THRUST3 %.2i",t[2]);  
  ROS_INFO("THRUST4 %.2i",t[3]); 

  ROS_INFO("done------------------");  

}


// main program---------------------------------------------------------------------------------
int main(int argc, char** argv)
{
  
  ROS_INFO(" initializing calStats node");   // initializing ros
  ros::init(argc, argv, "calStats"); //initiating node with unique node name
  ros::NodeHandle nh;
  ros::Rate loop_rate(5);
  calStats cso; // ROS_INFO(" instantiated object");
  ros::Subscriber sb_mavProp = nh.subscribe<>("/arm_4",1, &calStats::mavPropGrab, &cso);

  while(ros::ok)
  {

    ros::spinOnce();
    
    for(int i = 0; i < 4; ++i)
    {  
    //Strain gauge signal (STRAINX)  
    cso.s_sigma[i] = cso.s[i] - cso.s_EMA_old[i];
    cso.s_EMA[i] = cso.s_EMA_old[i] + cso.s_alpha[i] * cso.s_sigma[i];
    cso.s_EMV[i] = (1-cso.s_alpha[i]) * (cso.s_EMV_old[i] + cso.s_alpha[i] * pow(cso.s_sigma[i],2));
    cso.s_EMSD[i] = pow(cso.s_EMV[i], 0.5);

    //ROS_INFO("----- n = %.2i",i);
    //ROS_INFO("The current STRAIN value is: %.2i",cso.s[i]);
    //ROS_INFO("s_sigma is: %.2i",cso.s_sigma[i]);
    //ROS_INFO("s_alpha is: %2.2f",cso.s_alpha[i]);

    ROS_INFO("~current s_EMA[%.1i] is: %.2i",i,cso.s_EMA[i]);
    //ROS_INFO("old s_EMA is: %.2i",cso.s_EMA_old[i]);
    //ROS_INFO("current s_EMV is: %.2f",cso.s_EMV[i]);
    //ROS_INFO("old s_EMV is: %.2f",cso.s_EMV_old[i]);
    ROS_INFO("s_EMSD[%.1i] is: %.2f",i,cso.s_EMSD[i]);

    cso.s_EMA_old[i] = cso.s_EMA[i];
    cso.s_EMV_old[i] = cso.s_EMV[i];

    //Load Cell signal (THRUSTX)
    cso.t_sigma[i] = cso.t[i] - cso.t_EMA_old[i];
    cso.t_EMA[i] = cso.t_EMA_old[i] + cso.t_alpha[i] * cso.t_sigma[i];
    cso.t_EMV[i] = (1-cso.t_alpha[i]) * (cso.t_EMV_old[i] + cso.t_alpha[i] * pow(cso.t_sigma[i],2));
    cso.t_EMSD[i] = pow(cso.t_EMV[i], 0.5);

    //ROS_INFO("----- n = %.2i",i);
    //ROS_INFO("The current THRUST value is: %.2i",cso.t[i]);
    //ROS_INFO("t_sigma is: %.2i",cso.t_sigma[i]);
    //ROS_INFO("t_alpha is: %2.2f",cso.t_alpha[i]);

    ROS_INFO("~current t_EMA[%.1i] is: %.2i",i,cso.t_EMA[i]);
    //ROS_INFO("old t_EMA is: %.2i",cso.t_EMA_old[i]);
    //ROS_INFO("current t_EMV is: %.2f",cso.t_EMV[i]);
    //ROS_INFO("old t_EMV is: %.2f",cso.t_EMV_old[i]);
    ROS_INFO("t_EMSD[%.1i] is: %.2f",i,cso.t_EMSD[i]);

    cso.t_EMA_old[i] = cso.t_EMA[i];
    cso.t_EMV_old[i] = cso.t_EMV[i];

    }
    loop_rate.sleep();

  }

  return 0;

}

//https://answers.ros.org/question/252763/use-variables-in-main-from-callback/
//https://en.wikipedia.org/wiki/Moving_average#Exponentially_weighted_moving_variance_and_standard_deviation