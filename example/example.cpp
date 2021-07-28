#include <iostream>
#include <unistd.h>
#include "RtDB2.h"
#include "comm.hpp"
#include "Robot.hpp"
#include "Service.hpp"
#include "boost/program_options.hpp"

namespace po = boost::program_options;

uint64_t string2uint(const std::string& s)
{
    if (s.find("0x") == 0)
    {
        uint64_t result;
        std::stringstream ss;
        ss << std::hex << s;
        ss >> result;
        return result;
    }
    return atol(s.c_str());
}

int main(int argc, char **argv)
{
    int robotid;
    std::string hashstr;

    // Create program   options
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("hash,h", po::value<std::string>(&hashstr)->default_value(""), "hash")
        ("robot,r", po::value<int>(&robotid)->default_value(0), "robot");

    // Parse options
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // Extract hash
    uint64_t hash = string2uint(hashstr);
    std::cout << "Target robot: " << robotid << std::endl;
    std::cout << "Using robot hash: " << std::hex << hash << std::dec << std::endl;

    // Start communication service
    rsopen::Service service;
    service.start();

    // Create proxy to robot
    rsopen::Robot robot(robotid, hash);
    robot.writeKickElevation(true); // lob shot

    // Control robot
    float t = 0;
    while (true)
    {
        rsopen::interrobot_t data;
        if (robot.read(data))
        {
            // successful read
            std::cout << "Robot at (" << data.self.pose.x << ", " << data.self.pose.y << ", " << data.self.pose.rz << ")" 
                      << "; Control ball: " << data.player_status.control_ball << std::endl;
            usleep(250000);

            if (data.player_status.control_ball)
            {
                std::cout << "Kicking ball..." << std::endl;
                robot.writeKick(5);
            }
        }
        else
        {
            // unsuccessful read
            std::cout << "No data" << std::endl;
            usleep(1000000);
        }
        t += 0.1;
        robot.writeVelocity(0, sin(t), 0);
    }

    return 0;
}
