#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h" //include the ball_chaser "DriveToTarget" header file

ros::Publisher motor_command_publisher;  //ROS::Publisher motor commands;

/*
ToDo: Create a handle_drive_request callback function that executes whenever a drive_bot service is requested.This function should publish the requested linear x and angular velocities to the robot wheel joints. After publishing the requested, a message feedback should be returned with the requested wheel velocities.
*/

bool handle_drive_request(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res)
{
	ROS_INFO("DriveToTargetRequest received - linear_x:%1.2f, angular_z: %1.2f",
		(float)req.linear_x, (float)req.angular_z);



	geometry_msgs::Twist motor_command;
	motor_command.linear.x = req.linear_x;
	motor_command.angular.z = req.angular_z;
	motor_command_publisher.publish(motor_command);


	res.msg_feedback = "Velocity is set to linearX: " + std::to_string(req.linear_x)
		+ " , angularZ: " + std::to_string(req.angular_z);

	ROS_INFO_STREAM(res.msg_feedback);
	return true;

}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "drive_bot");	//initialize a ROS node
	ros::NodeHandle n;			//Create a ROS NodeHandle object

	motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

	ros::ServiceServer service = n.advertiseService
	("/ball_chaser/command_robot", handle_drive_request);
	ROS_INFO("Ready to drive robot");

	ros::spin();

	return 0;

}

