#include "instrumentation/profiler.hxx"

PerfTimer& Profiler::timer(const char* name) {
    return timers_.try_emplace(name, name).first->second;
}

const std::unordered_map<const char*, PerfTimer>& Profiler::timers() {
    return timers_;
}
