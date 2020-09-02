#!/bin/bash

if [ "$#" == 0 ]; then
    echo "Using default values"
    SIMULATION_DIR="$( cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd )"
    FIRMWARE_DIR="$HOME/src/Firmware"
    FIRMWARE_BUILD_DIR="$HOME/src/Firmware/build/px4_sitl_default"
elif [ "$#" == 3 ]; then
    SIMULATION_DIR=$1       # Path of the simulation package
    FIRMWARE_DIR=$2         # Path of the PX4 Firmware
    FIRMWARE_BUILD_DIR=$3   # Path of the SITL build directory
else
    echo -e "usage: source setup.bash simulation_dir firmware_dir firmware_build_dir"
    return 1
fi

pushd .

# Build PX4 SITL
cd ${FIRMWARE_DIR}
DONT_RUN=1 make px4_sitl_default gazebo

# Setup Gazebo to find this package's models and plugins
export GAZEBO_MODEL_PATH=${SIMULATION_DIR}/models:${GAZEBO_MODEL_PATH}
export GAZEBO_PLUGIN_PATH=${SIMULATION_DIR}/../../devel/lib:${GAZEBO_PLUGIN_PATH}

# Setup Gazebo to find PX4
source ${FIRMWARE_DIR}/Tools/setup_gazebo.bash ${FIRMWARE_DIR} ${FIRMWARE_BUILD_DIR}

# Setup ROS to find the PX4 packages
export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:${FIRMWARE_DIR}
export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:${FIRMWARE_DIR}/Tools/sitl_gazebo

echo -e "ROS_PACKAGE_PATH $ROS_PACKAGE_PATH"

popd
