SIMULATION_DIR="$( cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd )"
FIRMWARE_DIR="$HOME/src/Firmware"
FIRMWARE_BUILD_DIR="$HOME/src/Firmware/build/px4_sitl_default"

# Types of drones
declare -a TYPES_OF_DRONES
TYPES_OF_DRONES="ropped_iris ropped_swarm iris iris_depth_camera iris_downward_depth_camera iris_fpv_cam iris_rplidar iris_stereo_camera"

cd $FIRMWARE_DIR 

# Setup Gazebo to find PX4
source Tools/setup_gazebo.bash $FIRMWARE_DIR $FIRMWARE_BUILD_DIR

# Setup ROS to find the PX4 packages
export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:$FIRMWARE_DIR
export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:$FIRMWARE_DIR/Tools/sitl_gazebo

# Setup Gazebo to find this package's models and plugins
export GAZEBO_MODEL_PATH=${GAZEBO_MODEL_PATH}:${SIMULATION_DIR}/models
export GAZEBO_PLUGIN_PATH=${GAZEBO_PLUGIN_PATH}:${SIMULATION_DIR}/../../devel/lib

#roslaunch dronecontrol swarm2.launch
#roslaunch dronecontrol test.launch

## PX4 original launch
#roslaunch px4 mavros_posix_sitl.launch vehicle:=iris_rplidar world:=$SIMULATION_DIR/worlds/H.world

# Lidar iris com lidar360

#roslaunch px4 mavros_posix_sitl_lidar.launch
#roslaunch px4 mavros_posix_sitl.launch vehicle:=iris_rplidar est:=lpe

# FPV cam
#roslaunch px4 mavros_posix_sitl.launch vehicle:=iris_fpv_cam

#roslaunch simulation H_world.launch
#roslaunch simulation dynamic_obstacles.launch
#echo "Escolha a simulação desejada:
#1) H_world com iris com camera frontal;
#2) Dynamic_obstacle com iris com lidar360 e camera para baixo;"
#read numero
#if [ $numero -eq 1 ]
#then
#    roslaunch simulation H_world.launch
#fi
#if [ $numero -eq 2 ]
#then
#    roslaunch simulation dynamic_obstacles.launch
#fi
echo "Would you like to (1)run a specific launch file or (2)choose the vehicle and world? (numebers only)"
read resposta

if [ $resposta -eq 1 ]
then
    declare -i counter
    counter=1
    echo "Which launch file would you like to use? (numebers only)"
    for entry in `ls ${SIMULATION_DIR}/launch`
    do
        echo "$counter)$entry"
        counter=$counter+1
    done
    read launchfile

    counter=1
    for entry in `ls ${SIMULATION_DIR}/launch`
    do
        if [ $launchfile -eq $counter ]
        then
        echo "Choosed launch file $entry "
        roslaunch simulation $entry

        fi
        counter=$counter+1
    done
elif [ $resposta -eq 2 ]
then
    declare -i counter
    counter=1
    echo "Which vehicle do you want?"
    for entry in $TYPES_OF_DRONES
    do
        echo "$counter)$entry"
        counter=$counter+1
    done
    read numberdrone
    counter=1
    for entry in $TYPES_OF_DRONES
    do
        if [ $numberdrone -eq $counter ]
        then
            echo "Choosed drone: $entry "
            choosedvehicle=$entry
        fi
        counter=$counter+1
    done
    declare -i counter
    counter=1
    echo "Which world do you want?"
    for entry in "${FIRMWARE_DIR}/Tools/sitl_gazebo/worlds"/*
    do
        echo "$counter)$entry"
        counter=$counter+1
    done
    for entry in "${SIMULATION_DIR}/worlds"/*
    do
        echo "$counter)$entry"
        counter=$counter+1
    done

    read numberworld
    counter=1
    for entry in "${FIRMWARE_DIR}/Tools/sitl_gazebo/worlds"/*
    do
        if [ $numberworld -eq $counter ]
        then
            echo "Choosed world: $entry "
            choosedworld=$entry
        fi
        counter=$counter+1
    done
    for entry in "${SIMULATION_DIR}/worlds"/*
    do
        if [ $numberworld -eq $counter ]
        then
            echo "Choosed world: $entry "
            choosedworld=$entry
        fi
        counter=$counter+1
    done
    roslaunch px4 mavros_posix_sitl.launch vehicle:=iris world:=$choosedworld sdf:=${FIRMWARE_DIR}/Tools/sitl_gazebo/models/$choosedvehicle/$choosedvehicle.sdf
    

    

fi