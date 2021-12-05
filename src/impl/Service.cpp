#include <Service.hpp>
#include "comm.hpp"
#include "RtDB2.h"
#include <string>
#include <sstream>
#include <sys/stat.h>

const int RSOPEN_CLIENTID = 125;
const std::string CONFIGFILENAME = "rtdb_configuration.xml";

using namespace rsopen;

Service::Service()
: m_configfile(configFile()), m_subscriber(nullptr), m_publisher(nullptr)
{
    if (!existsFile(m_configfile))
    {
        throw std::runtime_error("Config file missing: " + m_configfile);
    }
}

void Service::start()
{
    startSubscriber();
    startPublisher();
}

void Service::startSubscriber()
{
    // start shared data subscriber
    RtDB2Context ctx = RtDB2Context::Builder(RSOPEN_CLIENTID, RtDB2ProcessType::comm)
                           .withConfigFileName(m_configfile)
                           .build();
    m_subscriber = std::make_unique<Comm>(ctx);
    m_subscriber->settings.diagnostics = false;
    rtdb_threads.push_back(m_subscriber->start());
}

void Service::startPublisher()
{
    // start rsopenapi publisher
    RtDB2Context ctx = RtDB2Context::Builder(RSOPEN_CLIENTID, RtDB2ProcessType::comm)
                           .withConfigFileName(m_configfile)
                           .withNetwork("rsopenapi")
                           .build();
    m_publisher = std::make_unique<Comm>(ctx);
    m_publisher->settings.diagnostics = false;
    rtdb_threads.push_back(m_publisher->start());
}

void Service::stop()
{
	Comm::shutdown();
	// wait for all threads to finish
	for (std::vector<boost::thread>::iterator it = rtdb_threads.begin();
			it != rtdb_threads.end(); ++it) {
		it->join();
	}
    m_subscriber.reset(nullptr);
    m_publisher.reset(nullptr);
}

std::string Service::configFile() const
{
    std::string root = binPath();
    std::string name = root + "/" + CONFIGFILENAME;
    if (existsFile(name)) {
        return name;
    }
    name = root + "/../config/" + CONFIGFILENAME;
    if (existsFile(name)) {
        return name;
    }
    return CONFIGFILENAME;
}

std::string Service::binPath() const
{
    char buf[1024];
    ssize_t result = readlink("/proc/self/exe", buf, 1024);
    std::string path(buf, result);
    std::size_t found = path.find_last_of("/");
    return path.substr(0, found);
}

bool Service::existsFile(const std::string& name) const
{
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0);     
}
