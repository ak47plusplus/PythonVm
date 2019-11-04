#ifndef LOGGER_CONFIG_HPP__
#define LOGGER_CONFIG_HPP__

class LoggerConfig {
public:
    static void init();
    static void register_format_specifier(char *fmtSp, (void *)(*func)(void));
};

#endif
