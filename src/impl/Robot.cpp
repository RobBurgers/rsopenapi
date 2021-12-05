#include <Robot.hpp>
#include <string>
#include <sstream>
#include <rsopenapi_types.h>

const int RSOPEN_CLIENTID = 125;
const std::string KEY_KICK = "KICK";
const std::string KEY_VELOCITY = "VELOCITY";

using namespace rsopen;

std::unique_ptr<RtDB2> getRtdb(const int id, const std::string &database = "default")
{
    RtDB2Context ctx = RtDB2Context::Builder(RSOPEN_CLIENTID)
                           .withoutConfigFile()
                           .withDatabase(database)
                           .build();
    return std::unique_ptr<RtDB2>(new RtDB2(ctx));
}

template <typename T>
std::string int_to_hex(T i)
{
    std::stringstream stream;
    stream << "0x"
           << std::setfill('0') << std::setw(sizeof(T) * 2)
           << std::hex << i;
    return stream.str();
}

Robot::Robot(int id, uint64_t hash)
    : m_id(id)
    , m_rtdb_in(getRtdb(id))
    , m_rtdb_out(getRtdb(id, "rsopenapi"))
    , m_hash(int_to_hex(hash))
    , m_last_tick(0)
    , m_is_lob(false)
{
}

bool Robot::read(interrobot_t &data)
{
    int result = RTDB2_SUCCESS;
    result = std::min(m_rtdb_in->get(KEY_METADATA, &data.metadata, m_id), result);
    result = std::min(m_rtdb_in->get(KEY_HW_STATUS, &data.hw_status, m_id), result);
    result = std::min(m_rtdb_in->get(KEY_PLAYER_STATUS, &data.player_status, m_id), result);

    result = std::min(m_rtdb_in->get(KEY_LOCAL_SELF, &data.self, m_id), result);
    result = std::min(m_rtdb_in->get(KEY_LOCAL_BALL, &data.ball, m_id), result);
    result = std::min(m_rtdb_in->get(KEY_LOCAL_OBSTACLES, &data.obstacles, m_id), result);

    result = std::min(m_rtdb_in->get(KEY_PASS_DETAIL, &data.pass_detail, m_id), result);
    result = std::min(m_rtdb_in->get(KEY_PASS_REQUEST, &data.pass_request, m_id), result);

    result = std::min(m_rtdb_in->get(KEY_TIME_IN_OWN_PENALTY_AREA, &data.time_in_own_penalty_area, m_id), result);
    result = std::min(m_rtdb_in->get(KEY_TIME_IN_OPPONENT_PENALTY_AREA, &data.time_in_opponent_penalty_area, m_id), result);

    if (result == RTDB2_SUCCESS) {
        m_last_tick = data.metadata.tick;
    }

    return result == RTDB2_SUCCESS;
}

void Robot::writeVelocity(float vx, float vy, float vrz)
{
    pose2d_t vel;
    vel.x = vx;
    vel.y = vy;
    vel.rz = vrz;

    m_rtdb_out->put(m_hash + "." + KEY_VELOCITY, &vel);
}

void Robot::writeKick(float speed)
{
    kick_t kick;
    kick.tick = m_last_tick;
    kick.speed = speed;
    kick.is_lob = m_is_lob;

    m_rtdb_out->put(m_hash + "." + KEY_KICK, &kick);
}

void Robot::writeKickElevation(bool isLobShot)
{
    m_is_lob = isLobShot;
    writeKick(0);
}
