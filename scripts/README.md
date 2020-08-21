# Scripts to setup the px4 Firmware for simulation and to initialize the simulation
For these scripts to work correctly they must have the following caracteristics:
* The px4 Firmware must be installed in the following directory: 
        `~/src/Firmware`
* The package *simulation* must be installed in a catkin environment and be named mavros_ws (yeah a know thats a bit inconvenient, but if you want to name your environment the way you want you just need change de simulation.sh to the correct directory)


And thats it, you can enjoy simulating your stuff :)

Ps: The setup script to install the Firmware normally dont work on the first try, but you just have install every dependencies the error screen asks for. 
