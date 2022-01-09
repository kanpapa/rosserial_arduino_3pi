/*
 * rosserial for Pololu 3pi robot
 * 
 */

#include <OrangutanMotors.h>
#include <OrangutanAnalog.h>
#include <OrangutanLEDs.h>
#include <OrangutanLCD.h>
#include <OrangutanBuzzer.h>

OrangutanLEDs leds;
OrangutanBuzzer buzzer;
OrangutanMotors motors;

#include <ros.h>
#include <geometry_msgs/Twist.h>

int robo_liner_x;
int robo_liner_y;
int robo_liner_z;
int robo_angle_x;
int robo_angle_y;
int robo_angle_z;

ros::NodeHandle nh;

void motorcon(const geometry_msgs::Twist& msg) {
  robo_liner_x = (int)(msg.linear.x * 100);
//  robo_liner_y = (int)(msg.linear.y * 50);
//  robo_liner_z = (int)(msg.linear.z * 50);
//  robo_angle_x = (int)(msg.angular.x * 50);
//  robo_angle_y = (int)(msg.angular.y * 50);
  robo_angle_z = (int)(msg.angular.z * 50);

  if (robo_liner_x > 255) robo_liner_x = 255;
  if (robo_liner_x < -255) robo_liner_x = -255;
  if (robo_angle_z > 255) robo_angle_z = 255;
  if (robo_angle_z < -255) robo_angle_z = -255;

  unsigned char xspd, yspd;

  xspd = abs(robo_angle_z);
  yspd = abs(robo_liner_x);
 
  if ((robo_angle_z == 0) && (robo_liner_x == 0)) {   // STOP
    motors.setSpeeds(0, 0);
  }
  else if (robo_liner_x == 0) {
    if (robo_angle_z > 0) { // CW
      motors.setSpeeds(-xspd, xspd);
    }
    else {  // CCW
      motors.setSpeeds(xspd, -xspd);
    }
  }
  else if (robo_angle_z == 0) {
    if (robo_liner_x > 0) { // Forward
      motors.setSpeeds(yspd, yspd);
    }
    else {  // Reverse
      motors.setSpeeds(-yspd, -yspd);
    }
  }
}

ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &motorcon);

void setup() {
  // Display battery voltage
  int bat = OrangutanAnalog::readBatteryMillivolts();
  OrangutanLCD::clear();
  OrangutanLCD::print(bat);
  OrangutanLCD::print("mV");

  // Sound test 
  buzzer.playNote(NOTE_A(4),500,15);

  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::print("ROS init");

  nh.getHardware()->setBaud(115200);  // 115200bps
  nh.initNode();
  nh.subscribe(sub);
  
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::print("ROS Redy");
  delay(500);
}

void loop() {
  nh.spinOnce();
  delay(1);
}
