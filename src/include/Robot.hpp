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

    // read the latest availabe robot data
    bool read(interrobot_t& data);

    // sets velocity in robot coordinates in [m/s] and [rad/s]
    void writeVelocity(float vx, float vy, float vrz);

    // sets horizontal (false) or lob (true) shot
    void writeKickElevation(bool isLobShot);

    // triggers a kick with the given ball speed [m/s]
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