#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

//define a global client that can request services
ros::ServiceClient client;
//This function calls the command_robot service to drive the robot in the specific direction
void drive_robot(float lin_x, float ang_z)
{
    //ToDo:Request a service and pass the velocities to it to drive the robot.
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = (float)lin_x;
    srv.request.angular_z = (float)ang_z;

    if (!client.call(srv))
        ROS_ERROR("Failed to call service drive_robot");
}


void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    bool catch_ball = false;
    int image_position = 0;
    float forward = 1.75;
    float stop = 0.0;
    float turn_left = 0.75;
    float turn_right = -1.75;
    float no_turn = 0.0;



    for (int i = 0; i < img.height * img.step; i += 3) {
        if (img.data[i] == 255 && img.data[i + 1] == 255 && img.data[i + 2] == 255)
        {
            image_position = i % img.step;

              if (image_position < img.step / 3)
                   drive_robot(forward, turn_left);
               else if (image_position < (img.step / 3 * 2))
                   drive_robot(forward, no_turn);
               else //(image_position == (img.step / 3 * 3))
                   drive_robot(forward, turn_right);
               catch_ball = true;
                   //drive_robot(stop, stop);
               break;

        }
    }

    if (catch_ball == false)
        drive_robot(0.0, 0.0);
//  else if (catch_ball == true)  // look around for the ball
//     {
//		 drive_robot(forward, turn_right);
// }


}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    ros::spin();

    return 0;

}