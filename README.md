# Skyrats simulation package

ROS package for PX4-Gazebo-ROS Software-In-The-Loop simulations for the Skyrats team.

## Contents

- `launch`: launchfiles for starting gazebo worlds/PX4 simulations with ROS
- `models`: SDF and URDF models
- `plugins`: Gazebo plugins
- `scripts`: Setup scripts
- `worlds`: Gazebo worlds

## Launching the simulation

```bash

# To configure the Skyrats Gazebo Interface
gedit ~/.gazebo/gui.ini
# Add this to the end of the file:
[overlay_plugins]
filenames=libskyrats_interface.so

# Convenience script for building the PX4 Firmware and setting up Environment Variables
source scripts/setup.bash

# Plain SITL simulation with Iris drone
roslaunch px4 mavros_posix_sitl.launch
```