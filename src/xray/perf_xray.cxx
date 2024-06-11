#include "xray/perf_xray.hxx"

void PerfXray::update() {
    if (ImGui::Begin("Performance")) {
        for (const auto& t : Profiler::timers()) {
            const auto text = fmt::format("{}: {} µs", t.first, t.second.duration());
            ImGui::Text("%s", text.c_str());
        }
    }
    ImGui::End();
}

