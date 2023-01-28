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

To build the code in this repository (see required dependencies below):

```sh
git submodule init
git submodule update
mkdir build
cd build
cmake ..
make
```

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

```sh
cd scripts
./rsim.sh
```

The container starts in interactive mode, meaning that you can execute commands
inside the container while it is running. To put the simulated robot in the
right mode to interface with an rsopenapi based application run this command:

```sh
`./rsopenapi.sh -i 1
```

Note: This command also moves to robot to the center of the field, facing the
ball.

## graphical user interface

The graphical user interface start with these commands (in a separate terminal window):

```sh
cd scripts
./gui.sh
```

The user interface is published on [http://localhost:6080](http://localhost:6080/)
(it opens automatically when using Firefox).

After checking the checkbox on robot 1 and selecting '3d-Field view' the simulated robot is visible. It should look similar to this:

![robogui](images/docker-gui.png?raw=true)

## run software development environment

The docker container for the software development environment must be built locally (in yet another terminal window):

```sh
./docker-build.sh
```

After a successful build it starts with:

```sh
./docker-run.sh
```

The docker container runs in interactive mode and automatically mounts the
current working directory inside the container as directory `/workspace`.
It uses your local account and user id and all modified files are being stored on your host system.

Inside the container the example runs using (watch the gui while running the command):

```sh
./example.sh
```

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

# API

## coordinate system

The data exposed in `interrobot_t` uses a [right-handed coordinate system](https://en.wikipedia.org/wiki/Right-hand_rule) as described in chapter 2 of [MSL World Model data struct](https://msl.robocup.org/wp-content/uploads/2018/08/MSL_WMDataStruct.pdf).

Instructions send to a robot must use the robot's local coordinate system as described in [MSL World Model data struct](https://msl.robocup.org/wp-content/uploads/2018/08/MSL_WMDataStruct.pdf). As an example, a positive velocity setpoint in the y-direction moves the robot forward.


## interrobot_t

| name | type | description |
|------|------|-------------|
| metadata | metadata_t | robot meta data |
| hw_status | hw_status_t | hardware status |
| player_status | player_status_t |  |
| **_local_** |
| self | object2d_t | robot self localization |
| ball | object3d_t | ball observed by this robot |
| obstacles | objects2d_t | obstacles observed by this robot |
| **_fused_** |
| fball | object3d_t | fused ball |
| us | objects2d_t | fused locations of robots in my team |
| them | objects2d_t | fused location of robots in opponents team |
| selfloc | object2d_t | self localisation |
| selfloc_omni | object2d_t | self localisation omni vision |
| **_pass_** |
| pass_detail | pass_detail_t | pass details |
| pass_request | pass_request_t | pass request by this robot |
| **_planner_** |
| ball_pickup | ball_pickup_t | ball pickup details |
| planned_path | std::vector<pos2d_t> | planned path for this roboot |
| time_in_own_penalty_area | float | time [ms] player is in own penalty area. 0 if not in penalty area |
| time_in_opponent_penalty_area | float | time [ms] player is in opponent penalty area. 0 if not in penalty area |

### metadata_t

| name | type | description |
|------|------|-------------|
| version | uint8_t | interface version |
| hash | uint64_t | internal use |
| ts | float | timestamp in seconds since start of day |
| tick | uint32_t | robot task execution cycle count since start of service (task execution cycle runs at 40 Hz) |

### hw_status_t

| name | type | description |
|------|------|-------------|
| battery_voltage | float | current battery voltage |
| kicker_soc | float | kicker state of charge [0..1]. when 1 kicker is fully charged |
| is_freemode | bool | when true the robot is in push mode. control commands ignored |
| is_compass_healthy | bool | when false no compass data is available. affects self-localization |
| is_kicker_healthy | bool | when false the robot cannot kick |
| is_omni_healthy | bool | when false the omni camera image stream has been interrupted. affects self-localization |
| is_battery_low | bool | battery low status. when true robot needs charging |
| is_disk_low | bool | when true the robot's HDD is low on disk space. interrupts data collection. may affect ability to play  |
| kinect_num_balls | uint8_t | number of balls detected by kinect (if present) |

### player_status_t

| name | type | description |
|------|------|-------------|
| vendor_id | uint8_t | ID as assigned by MSL |
| shirt_number | uint8_t | shirt number as visible on the robot |
| shirt_color | uint8_t | 0: magenta; 1: cyan |
| assigned_role | uint8_t | role as assigned by the sideline. 0: reserve; 1: fieldplayer; 2: goalie |
| active_role | uint8_t | role which the robot currently has. 0: reserve; 1: fieldplayer; 2: goalie |
| dynamic_role | uint8_t | role during play; not relevant when controlling robot through this API |
| game_state | uint8_t | 	0: NONE; 1: NORMAL; 2: NORMAL_ATTACK; 3: NORMAL_DEFEND; 4: PARKING; 5: BEGIN_POSITION; 6: KICKOFF; 7: KICKOFF_AGAINST; 8: FREEKICK; 9: FREEKICK_AGAINST; 10: GOALKICK; 11: GOALKICK_AGAINST; 12: THROWIN; 13: THROWIN_AGAINST; 14: CORNER; 15: CORNER_AGAINST; 16: PENALTY; 17: PENALTY_AGAINST; 18: PENALTY_SHOOTOUT; 19: PENALTY_SHOOTOUT_AGAINST; 20: DROPPED_BALL; 21: YELLOW_CARD_AGAINST; 22: RED_CARD_AGAINST; 23: GOAL; 24: GOAL_AGAINST |
| behavior_state | uint16_t | behaviour state index from statics; not relevant when controlling robot through this API |
| control_ball | bool | indicator whether robot is in control of the ball |
| team_control_ball | bool | indicator whether team is in control of the ball |

### object2d_t

Object with orientation moving in 2D space.

| name | type | description |
|------|------|-------------|
| ts | float | timestamp of measurement |
| pose | pose2d_t | 2d pose of object |
| vel | pos2d_t | 2d velocity of object |
| confidence | float | measurement confidence [0..1] |

### object3d_t

Object without orientation moving in 3D space.

| name | type | description |
|------|------|-------------|
| ts | float | timestamp of measurement |
| pos | pos3d_t | 3d position of object |
| vel | pos3d_t | 3d velocity of object |
| confidence | float | measurement confidence [0..1] |

### objects2d_t

List of object2d_t.

### pos2d_t

2D position.

| name | type | description |
|------|------|-------------|
| x | float | x-value in [m] |
| y | float | y-value in [m] |

### pos3d_t

3D position.

| name | type | description |
|------|------|-------------|
| x | float | x-value in [m] |
| y | float | y-value in [m] |
| z | float | z-value in [m] |

### pose2d_t

2D pose.

| name | type | description |
|------|------|-------------|
| x | float | x-value in [m] |
| y | float | y-value in [m] |
| r | float | orientation in [rad] |

### pass_detail_t

Details about pass filled by robot making a pass attempt. Not supported by this API.

| name | type | description |
|------|------|-------------|
| valid | bool | true when robot in ball possession and aiming a pass |
| ts | float | timestamp when this information was constructed |
| target_id | short | shirt number of target robot. 0: aiming at goal |
| kicked | bool | true, when pass kicked |
| eta | float | estimated time of arrival at target robot/location |
| speed | float | speed at which the pass was kicked in [m/s] |
| angle | float | pitch angle at which the pass was kicked [rad] |
| origin | pos2d_t | origin of pass |
| target | pos2d_t | target of pass |

### pass_request_t

Pass request by robot. Not supported by this API.

| name | type | description |
|------|------|-------------|
| valid | bool | true, when making a pass request |
| ts | float | timestamp of pass request |
| eta | float | time when robot expects to be present at the target location |
| target | pos2d_t | location in field coordinates where robot wants to received the pass |

### ball_pickup_t

Report where robot picked up the ball. According to MSL rules a robot is only allowed to drive upto 3 [m] from the location where the ball was picked up.

| name | type | description |
|------|------|-------------|
| valid | bool | true, when data valid |
| ts | float | timestamp of pick up |
| pos | pos2d_t | location of pick up |

# license

This software is licenced under the MIT license, which can be found in [LICENSE](LICENSE). By using, distributing, or contributing to this project, you agree to the terms and conditions of this license.
