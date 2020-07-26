cd ~/src/Firmware 

source Tools/setup_gazebo.bash $(pwd) $(pwd)/build/px4_sitl_default
export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:$(pwd)
export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:$(pwd)/Tools/sitl_gazebo

#roslaunch dronecontrol swarm2.launch
#roslaunch dronecontrol test.launch

## PX4 original launch
#roslaunch px4 mavros_posix_sitl.launch

# Lidar iris com lidar360

#roslaunch px4 mavros_posix_sitl_lidar.launch
#roslaunch px4 mavros_posix_sitl.launch vehicle:=iris_rplidar est:=lpe

# FPV cam
#roslaunch px4 mavros_posix_sitl.launch vehicle:=iris_fpv_cam

# Mundo com H
export GAZEBO_MODEL_PATH=${GAZEBO_MODEL_PATH}:~/mavros_ws/src/simulation/models
roslaunch simulation H_world.launch
#roslaunch simulation indoor_imav2019.launch
