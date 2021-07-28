#ifndef _INCLUDE_RSOPEN_ROBOT_HPP_
#define _INCLUDE_RSOPEN_ROBOT_HPP_

// standard/system headers
#include <string>
#include <rtdb_types.h>

namespace rsopen
{

class Robot
{
public:
    Robot(int id, uint64_t hash);

    std::string describe() const;
    bool read(interrobot_t& data);
    void writeVelocity(float vx, float vy, float vrz);
    void writeKickElevation(bool isLobShot);
    void writeKick(float speed);

private:
	int m_id; // id for this robot
	std::unique_ptr<RtDB2> m_rtdb_in;
	std::unique_ptr<RtDB2> m_rtdb_out;
    std::string m_hash;
    uint32_t m_last_tick;
    bool m_is_lob;
}; // end of class Robot

} // end of namespace mtp

#endif