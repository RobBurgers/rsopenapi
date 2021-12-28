# rsopenapi

rsopenapi offers an API to control a [robotsports](https://www.robotsports.nl) soccer robot. A client can retrieve status information from the robot and send control commands to the robot.

The status information includes:

* Robot position
* Ball position
* Team mate positions
* Obstacle positions
* Ball possession status
* Hardware status

The complete set of status information is listed in [rtdb_types.h](src/include/rtdb_types.h)

The control commands are:

* Speed setpoint (x, y, rz)
* Kick (speed, lob)

# build

To build the code in this repository (see required depedencies below):

* `git submodule init`
* `git submodule update`
* `mkdir build`
* `cd build`
* `cmake ..`
* `make`

## dependencies

The dependencies required for this project are:

```sh
apt install -y \
    cmake \
    g++ \
    libboost-all-dev \
    liblmdb-dev \
    liblz4-dev \
    libmsgpack-dev \
    libxerces-c3-dev \
    libzstd-dev \
    xsdcxx
```

# try example in docker environment

To try the example in a docker environment you need three docker containers:

* One running the robot simulation
* One running the graphical user interface
* One running the software development environment of rsopenapi

## robot simulation

The robot simulation runs one simulated robot and starts with these command:

* `cd scripts`
* `./rsim.sh`

The container starts in interactive mode, meaning that you can execute commands
inside the container while it is running. To put the simulated robot in the
right mode to interface with an rsopenapi based application run this command:

* `./rsopenapi.sh -i 1`

Note: This command also moves to robot to the center of the field, facing the
ball.

## graphical user interface

The graphical user interface start with these commands (in a separate terminal window):

* `cd scripts`
* `./gui.sh`

The user interface is published on [http://localhost:6080](http://localhost:6080/)
(it opens automatically when using Firefox).

After checking the checkbox on robot 1 and selecting '3d-Field view' the simulated robot is visible. It should look similar to this:

![robogui](images/docker-gui.png?raw=true)

## run software development environment

The docker container for the software development environment must be built locally (in yet another terminal window):

* `./docker-build.sh`

After a successful build it starts with:

* `./docker-run.sh`

The docker container runs in interactive mode and automatically mounts the
current working directory inside the container as directory `/workspace`.
It uses your local account and user id and all modified files are being stored on your host system.

Inside the container the example runs using (watch the gui while running the command):

* `./example.sh`

Note: This scripts builds the example and runs it. In case the build fails you need to clean your build directory before running again.

Tip: Examine the script to get familiar with the build and run steps.

# run example on robot 1

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

# license

This software is licenced under the MIT license, which can be found in [LICENSE](LICENSE). By using, distributing, or contributing to this project, you agree to the terms and conditions of this license.
