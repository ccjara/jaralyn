#ifndef NOEDEN_PERF_XRAY_HXX
#define NOEDEN_PERF_XRAY_HXX

#include "xray/xray_interface.hxx"

class PerfXray : public IXray {
public:
    void update() override;
};

#endif
