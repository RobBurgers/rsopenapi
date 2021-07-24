#ifndef _INCLUDE_RSOPEN_SERVICE_HPP_
#define _INCLUDE_RSOPEN_SERVICE_HPP_

// standard/system headers
#include <string>
#include <rtdb_types.h>
#include <boost/thread.hpp>
#include "comm.hpp"
#include "RtDB2Context.h"

namespace rsopen
{

class Service
{
public:
    Service();

    void start();
    void stop();

private:
    std::string m_configfile;
    std::unique_ptr<Comm> m_subscriber;
    std::unique_ptr<Comm> m_publisher;
    std::vector<boost::thread> rtdb_threads;

    void startSubscriber();
    void startPublisher();
    std::string binPath() const;
    std::string configFile() const;
    bool existsFile(const std::string& name) const;
}; // end of class Robot

} // end of namespace mtp

#endif