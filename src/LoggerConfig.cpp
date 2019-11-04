#include "LoggerConfig.hpp"
#include "3rd/easylogging++/easylogging++.hpp"

void LoggerConfig::init()
{
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    // Values are always std::string
    defaultConf.set(el::Level::Info, el::ConfigurationType::Format, "%datetime %level %msg");
    // default logger uses default configurations
    el::Loggers::reconfigureLogger("default", defaultConf);
    LOG(INFO) << "Log using default file";
    // To set GLOBAL configurations you may use
    defaultConf.setGlobally(el::ConfigurationType::Format, "%date %msg");
    el::Loggers::reconfigureLogger("default", defaultConf);
}
