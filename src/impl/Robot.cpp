#include <Robot.hpp>
#include <string>
#include <sstream>

const int RSOPEN_CLIENTID = 569;

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
    : m_id(id), m_rtdb_in(getRtdb(id)), m_rtdb_out(getRtdb(id, "rsopenapi")), m_hash(int_to_hex(hash))
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

    return result == RTDB2_SUCCESS;
}

void Robot::writeVelocity(float vx, float vy, float vrz)
{
    pos3d_t v;
    v.x = vx;
    v.y = vy;
    v.z = vrz;

    std::cout << m_rtdb_out->put(m_hash + ".VELOCITY", &v) << std::endl;
}
