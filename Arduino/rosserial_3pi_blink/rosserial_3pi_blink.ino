/*
 * rosserial Subscriber Example for Pololu 3pi robot
 * Blinks an LED on callback
 * 
 * http://wiki.ros.org/rosserial_arduino/Tutorials/Blink
 */

#include <ros.h>
#include <std_msgs/Empty.h>
#include <OrangutanAnalog.h>
#include <OrangutanLEDs.h>
#include <OrangutanLCD.h>

OrangutanLEDs leds;
OrangutanLCD lcd;

ros::NodeHandle nh;

void messageCb( const std_msgs::Empty& toggle_msg){
  leds.green(HIGH);    // turn green LED on
}

ros::Subscriber<std_msgs::Empty> sub("toggle_led", &messageCb );

void setup()
{
  // Display battery voltage
  int bat = OrangutanAnalog::readBatteryMillivolts();

  lcd.clear();
  lcd.print(bat);
  lcd.print("mV");
  lcd.gotoXY(0, 1);

  leds.green(LOW);       // turn green LED off
  nh.getHardware()->setBaud(115200);  // 115200bps
  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{
  nh.spinOnce();
  delay(1);
}
