#ifndef NOEDEN_PERF_TIMER_HXX
#define NOEDEN_PERF_TIMER_HXX

class PerfTimer {
public:
    explicit PerfTimer(std::string name);

    const std::string& name() const;

    void start();
    void stop();

    u128 duration() const;
private:
    std::string name_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    u128 duration_ = 0;
};

#endif