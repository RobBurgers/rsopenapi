# rsopenapi

rsopenapi offers an API to control a [robotsports](https://www.robotsports.nl) soccer robot. A client can retrieve with robot status information from the robot and send control commands to the robot.

The status information includes:

* Robot position
* Ball position
* Team mate positions
* Obstacle positions
* Ball possession status
* Hardware status

The control commands are:

* Speed setpoint (x, y, rz)
* Kick (speed, lob)

# build

To build the code in this repository:

* `git submodule init`
* `git submodule update`
* `mkdir build`
* `cd build`
* `cmake ..`
* `make`

# run example

This description assumes that robot number 1 is used.

To run the example client application from this repository:

* Start a robot (either a simulated robot or a real robot)
* Use the robogui to change its operation mode to rsopenapi: Tactics -> Behavior 'rsopenapi' -> Activate
* Retrieve the hash code of the running robot (this code makes sure that the intended robot responds to the control commands):
    * Simulated robot: `so -i 1 -r hash`
    * Real robot: `so -r hash`
* Start the example from this repository:  
  `cd build/bin`  
  `./example -h <hash> -r 1`
