#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Empty.h"
#include "sensor_msgs/Joy.h"
#include "geometry_msgs/Twist.h"
#include <sstream>

double lin_vel_constant = 0.2;
double ang_vel_constant = 0.4;
double analog_thresh = 0.3;


class SubscribeAndPublish
{
public:
  SubscribeAndPublish()
  {
    // Publishers for takeoff and land Drone
    cmd_takeoff_ = n_.advertise<std_msgs::Empty>("ardrone/takeoff", 1);
    cmd_land_ = n_.advertise<std_msgs::Empty>("ardrone/land", 1);
    // Publisher for controling movement
    cmd_vel_ = n_.advertise<geometry_msgs::Twist>("cmd_vel", 1);

    sub_ = n_.subscribe("joy", 1, &SubscribeAndPublish::joyCallback, this);  
  }

  void joyCallback(const sensor_msgs::Joy::ConstPtr& msg)
  {
    // Intitalize msgs
    std_msgs::Empty msg_takeoff_land;
    geometry_msgs::Twist ardrone_vel;
    if(msg->buttons[2]==1){
      ROS_INFO("Land");
      cmd_land_.publish(msg_takeoff_land);
    }else if(msg->buttons[3]==1){
      ROS_INFO("TakeOFF");
      cmd_takeoff_.publish(msg_takeoff_land);
    }else if(abs(msg->axes[4])>analog_thresh){
      double vel = calculate_lin_input(msg->axes[4]);
      ardrone_vel.angular.z=0;
      ardrone_vel.angular.y=0;
      ardrone_vel.angular.x=0;
      ardrone_vel.linear.z=0;
      ardrone_vel.linear.y=0;
      ardrone_vel.linear.x=vel;
      cmd_vel_.publish(ardrone_vel);
    }else if(abs(msg->axes[3])>analog_thresh){
      double vel = calculate_lin_input(msg->axes[3]);
      ardrone_vel.angular.z=0;
      ardrone_vel.angular.y=0;
      ardrone_vel.angular.x=0;
      ardrone_vel.linear.z=0;
      ardrone_vel.linear.y=vel;
      ardrone_vel.linear.x=0;
      cmd_vel_.publish(ardrone_vel);
    }else if(abs(msg->axes[1])>analog_thresh){
      double vel = calculate_lin_input(msg->axes[1]);
      ardrone_vel.angular.z=0;
      ardrone_vel.angular.y=0;
      ardrone_vel.angular.x=0;
      ardrone_vel.linear.z=vel;
      ardrone_vel.linear.y=0;
      ardrone_vel.linear.x=0;
      cmd_vel_.publish(ardrone_vel);
    }else if(abs(msg->axes[0])>analog_thresh){
      double vel = calculate_ang_input(msg->axes[0]);
      ardrone_vel.angular.z=vel;
      ardrone_vel.angular.y=0;
      ardrone_vel.angular.x=0;
      ardrone_vel.linear.z=0;
      ardrone_vel.linear.y=0;
      ardrone_vel.linear.x=0;
      cmd_vel_.publish(ardrone_vel);
    }else if(abs(msg->axes[4])<analog_thresh||abs(msg->axes[3])<analog_thresh||abs(msg->axes[0])<analog_thresh||abs(msg->axes[1])<analog_thresh){
      ardrone_vel.angular.z=0;
      ardrone_vel.angular.y=0;
      ardrone_vel.angular.x=0;
      ardrone_vel.linear.z=0;
      ardrone_vel.linear.y=0;
      ardrone_vel.linear.x=0;
      cmd_vel_.publish(ardrone_vel);
    }
  }

  double calculate_lin_input(double vel){
    double out;
    if(vel<0){
      out = lin_vel_constant*(vel+analog_thresh);
    }else{
      out = lin_vel_constant*(vel-analog_thresh);
    }
    return out;
  }

  double calculate_ang_input(double vel){
    double out;
    if(vel<0){
      out = ang_vel_constant*(vel+analog_thresh);
    }else{
      out = ang_vel_constant*(vel-analog_thresh);
    }
    return out;
  }

private:
  ros::NodeHandle n_;
  ros::Publisher cmd_takeoff_;
  ros::Publisher cmd_land_;
  ros::Publisher cmd_vel_;
  ros::Subscriber sub_;
};


int main(int argc, char **argv)
{
  ros::init(argc, argv, "control_joy_to_parrot");
  SubscribeAndPublish SAPObject;
  ros::spin();
  return 0;
}