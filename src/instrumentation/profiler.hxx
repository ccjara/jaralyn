#ifndef NOEDEN_PROFILER_HXX
#define NOEDEN_PROFILER_HXX

#include "perf_timer.hxx"

class Profiler {
public:
    static PerfTimer& timer(const char* name);

    static const std::unordered_map<const char*, PerfTimer>& timers();
private:
    static inline std::unordered_map<const char*, PerfTimer> timers_;
};

#endif