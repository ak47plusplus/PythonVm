#ifndef LOGGER_CONFIG_HPP__
#define LOGGER_CONFIG_HPP__

#include <functional>

class LoggerConfig {
public:
    static void init();
    static void register_format_specifier(char *fmtSp, std::function<std::string(const LogMessage*)>);
};

#endif
