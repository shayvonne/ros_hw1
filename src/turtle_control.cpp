#include "ros/ros.h"

// include msg library
#include <geometry_msgs/Twist.h>

// include cpp library 
#include <cstdio>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

geometry_msgs::Twist vel_msg;

char getch()
{
  int flags = fcntl(0, F_GETFL, 0);
  fcntl(0, F_SETFL, flags | O_NONBLOCK);

  char buf = 0;
  struct termios old = {0};
  if (tcgetattr(0, &old) < 0) {
      perror("tcsetattr()");
  }
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0) {
      perror("tcsetattr ICANON");
  }
  if (read(0, &buf, 1) < 0) {
      //perror ("read()");
  }
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0) {
      perror ("tcsetattr ~ICANON");
  }
  return (buf);
}

void KeyboardControl()
{
  int c = getch();
  if (c != EOF)
  {
    switch (c)
    {
      case 119:    // key w - move forward
        vel_msg.linear.x = 2.0;
        vel_msg.angular.z = 0.0;
        break;
      case 115:    // key s - move backward
        vel_msg.linear.x = -2.0;
        vel_msg.angular.z = 0.0;
        break;
      case 100:    // key d - turn right
        vel_msg.linear.x = 0.0;
        vel_msg.angular.z = -2.0;
        break;
      case 97:    // key a - turn left
        vel_msg.linear.x = 0.0;
        vel_msg.angular.z = 2.0;
        break;
      case 114:    // key r - stop the turtle 
        vel_msg.linear.x = 0.0;
        vel_msg.angular.z = 0.0;
        break;
    }
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "turtle_control");
  ros::NodeHandle n;

  // Replace 'your_turtle_name' with your actual turtle name
  ros::Publisher turtlesim_pub = n.advertise<geometry_msgs::Twist>("/your_turtle_name/cmd_vel", 100);

  ros::Rate loop_rate(100);

  printf("KeyboardControl start\n");

  int count = 0;

  while (ros::ok()){
    KeyboardControl();
    turtlesim_pub.publish(vel_msg);
    printf("\ncount : %d\n",count);
    printf("linear\t %f \n",vel_msg.linear.x);
    printf("angular\t %f \n",vel_msg.angular.z);
    count++;
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
