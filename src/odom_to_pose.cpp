#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>
#include <ros/ros.h>

nav_msgs::Odometry odom_data;
geometry_msgs::PoseStamped position_data;

void state_callback(const nav_msgs::Odometry msg){
    odom_data = msg;
    odom_data.header.frame_id = "odom";
    odom_data.child_frame_id = "base_link_frd";
    position_data.pose = odom_data.pose.pose;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "odom_to_pose_node");
    ros::NodeHandle nh;

    ros::Subscriber state_sub = nh.subscribe<nav_msgs::Odometry>
            ("/camera/odom/sample", 10, state_callback);
    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>
            ("/mavros/vision_pose/pose", 10);
    ros::Publisher odom_position_pub = nh.advertise<nav_msgs::Odometry>
            ("/mavros/odometry/out", 10);
    ros::Rate rate(20.0);

    while(ros::ok()){
        state_sub;
        local_pos_pub.publish(position_data);
        odom_position_pub.publish(odom_data);
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}