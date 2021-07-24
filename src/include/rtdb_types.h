#ifndef RTDB_TYPES_H
#define RTDB_TYPES_H

// This header expresses the data types exchanged between robot via RtDB.
// This header shall *NOT* contain any robotsports specific includes, to allow
// 3rd-party applications to monitor the data.
#include "RtDB2.h"
#include <stdint.h>

#define KEY_METADATA "METADATA"
#define KEY_HW_STATUS "HW_STATUS"
#define KEY_PLAYER_STATUS "PLAYER_STATUS"

#define KEY_LOCAL_SELF "LOCAL_SELF"
#define KEY_LOCAL_BALL "LOCAL_BALL"
#define KEY_LOCAL_OBSTACLES "LOCAL_OBSTACLES"

#define KEY_FUSED_BALL "FUSED_BALL"
#define KEY_FUSED_US "FUSED_US"
#define KEY_FUSED_THEM "FUSED_THEM"
#define KEY_FUSED_SELFLOC "FUSED_SELFLOC"
#define KEY_FUSED_SELFLOC_OMNI "FUSED_SELFLOC_OMNI"

#define KEY_PASS_DETAIL "PASS_DETAIL"
#define KEY_PASS_REQUEST "PASS_REQUEST"

#define KEY_BALL_PICKUP "BALL_PICKUP"
#define KEY_PLANNED_PATH "PLANNED_PATH"
#define KEY_TIME_IN_OWN_PENALTY_AREA "TIME_IN_OWN_PENALTY_AREA"
#define KEY_TIME_IN_OPPONENT_PENALTY_AREA "TIME_IN_OPPONENT_PENALTY_AREA"


#define RDT_VENDOR_ID 56
#define RDT_VERSION 9

namespace rsopen {

typedef struct metadata {
	uint8_t version = RDT_VERSION;
	uint64_t hash = 0;
	float ts = 0;
	uint32_t tick = 0;

	SERIALIZE_DATA_FIXED(version, hash, ts, tick);

	metadata(uint64_t ahash = -1, float ats = 0, uint32_t atick = 0)
	: hash(ahash), ts(ats), tick(atick)
	{}
} metadata_t;

typedef struct pose2d {
	float x = 0; // m
	float y = 0; // m
	float rz = 0; // rad

	SERIALIZE_DATA_FIXED(x, y, rz);

	pose2d(float ax = 0, float ay = 0, float arz = 0)
	: x(ax), y(ay), rz(arz)
	{}
} pose2d_t;

typedef struct pos2d {
	float x = 0; // m
	float y = 0; // m

	SERIALIZE_DATA_FIXED(x, y);

	pos2d(float ax = 0, float ay = 0)
	: x(ax), y(ay)
	{}
} pos2d_t;

typedef struct pos3d {
	float x = 0; // m
	float y = 0; // m
	float z = 0; // m

	SERIALIZE_DATA_FIXED(x, y, z);

	pos3d(float ax = 0, float ay = 0, float az = 0)
	: x(ax), y(ay), z(az)
	{}
} pos3d_t;

// object with orientation moving in 2d space
typedef struct object2d
{
	float       ts = 0; // s
	pose2d_t    pose;
	pos2d_t     vel;
	float       confidence = 0; // 0..1

	SERIALIZE_DATA_FIXED(ts, pose, vel, confidence);

	object2d(float ats = 0, pose2d_t apose = pose2d(), pos2d_t avel = pos2d_t(), float aconfidence = 0)
	: ts(ats), pose(apose), vel(avel), confidence(aconfidence)
	{}
} object2d_t;

// object without orientation moving in 3d space
typedef struct object3d
{
	float       ts = 0; // s
	pos3d_t     pos;
	pos3d_t     vel;
	float       confidence = 0; // 0..1

	SERIALIZE_DATA_FIXED(ts, pos, vel, confidence);

	object3d(float ats = 0, pos3d_t apos = pos3d(), pos3d_t avel = pos3d_t(), float aconfidence = 0)
	: ts(ats), pos(apos), vel(avel), confidence(aconfidence)
	{}
} object3d_t;

typedef struct hw_status {
	float battery_voltage = 0; // Volt
	float kicker_soc = 0; // 0..1
	bool is_freemode = false;
	bool is_compass_healthy = false;
	bool is_kicker_healthy = false;
	bool is_omni_healthy = false;
	bool is_battery_low = false;
	bool is_disk_low = false;
	uint8_t kinect_num_balls = 0;

	SERIALIZE_DATA_FIXED(battery_voltage, kicker_soc, is_freemode, is_compass_healthy,
			is_kicker_healthy, is_omni_healthy, is_battery_low, is_disk_low,
			kinect_num_balls);
} hw_status_t;

typedef struct player_status {
	uint8_t vendor_id = RDT_VENDOR_ID;
	uint8_t shirt_number = 0;
	uint8_t shirt_color = 0;
	uint8_t assigned_role = 0; // role as assigned by the sideline (0: reserve, 1: fieldplayer, 2: goalie)
	uint8_t active_role = 0; // role which the robot currently has (0: reserve, 1: fieldplayer, 2: goalie)
	uint8_t dynamic_role = 0;
	uint8_t game_state = 0;
	uint16_t behavior_state = 0;
	bool control_ball = false;
	bool team_control_ball = false;

	SERIALIZE_DATA_FIXED(vendor_id, shirt_number, shirt_color, assigned_role, active_role, dynamic_role,
			game_state, behavior_state, control_ball, team_control_ball);
} player_status_t;

typedef struct pass_detail {
	bool valid = false;
	float ts = 0; // s
	short target_id = 0; // shirt number
	bool kicked = false;
	float eta = 0; // s
	float speed = 0; // [m/s]
	float angle = 0; // rad
	pos2d origin;
	pos2d target;

	SERIALIZE_DATA_FIXED(valid, ts, target_id, kicked, eta, speed, angle, origin, target);

	pass_detail(bool avalid = false, float ats = 0, short atarget_id = 0, bool akicked = false,
			float aeta = 0, float aspeed = 0, float aangle = 0,
			pos2d aorigin = pos2d(), pos2d atarget = pos2d())
	: valid(avalid), ts(ats), target_id(atarget_id), kicked(akicked), eta(aeta)
	, speed(aspeed), angle(aangle), origin(aorigin), target(atarget)
	{}
} pass_detail_t;

typedef struct pass_request {
	bool valid = false;
	float ts = 0;
	float eta = 0;
	pos2d_t target;

	SERIALIZE_DATA_FIXED(valid, ts, eta, target);

	pass_request(bool avalid = false, float ats = 0, float e = 0, pos2d_t t = pos2d())
	: valid(avalid), ts(ats), eta(e), target(t)
	{}
} pass_request_t;

typedef struct ball_pickup {
	bool valid = false;
	float ts = 0;
	pos2d_t pos;

	SERIALIZE_DATA_FIXED(valid, ts, pos);

	ball_pickup(bool avalid = false, float ats = 0, pos2d_t t = pos2d())
	: valid(avalid), ts(ats), pos(t)
	{}
} ball_pickup_t;

typedef std::vector<object2d_t> objects2d_t;

typedef struct interrobot {
	metadata_t metadata;
	hw_status_t hw_status;
	player_status_t player_status;
	// local
	object2d_t self;
	object3d_t ball;
	objects2d_t obstacles;
	// fused
	object3d_t fball;
	objects2d_t us;
	objects2d_t them;
	object2d_t selfloc;
	object2d_t selfloc_omni;
	// pass
	pass_detail_t pass_detail;
	pass_request_t pass_request;
	// planner
	ball_pickup_t ball_pickup;
	std::vector<pos2d_t> planned_path;
	float time_in_own_penalty_area; // time [ms] player is in own penalty area. 0 if not in penalty area.
	float time_in_opponent_penalty_area;  // time [ms] player is in opponent penalty area. 0 if not in penalty area.

} interrobot_t;

}

#endif
