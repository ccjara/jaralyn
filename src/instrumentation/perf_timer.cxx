#include "instrumentation/perf_timer.hxx"

PerfTimer::PerfTimer(std::string name) : name_(std::move(name)) {
    start_ = std::chrono::high_resolution_clock::now();
}

const std::string& PerfTimer::name() const {
    return name_;
}

void PerfTimer::start() {
    start_ = std::chrono::high_resolution_clock::now();
}

void PerfTimer::stop() {
    duration_ = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_).count();
}

u128 PerfTimer::duration() const {
    return duration_;
}

