#include <functional>

#ifndef __time_t_defined
typedef unsigned long time_t;
#endif

struct time {
    time();
    time(std::function<time_t(void)> sync_provider, const uint32_t sync_interval);
    ~time();
    void handle();
    time_t now();
    
private:
    bool _initialized = false;
    uint32_t _sync_interval;
    std::function<time_t(void)> _sync_provider;
    uint32_t _time;
};