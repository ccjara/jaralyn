#ifndef JARALYN_SCRIPT_XRAY_HXX
#define JARALYN_SCRIPT_XRAY_HXX

#include "xray_interface.hxx"
#include "xray_style.hxx"
#include "../scripts/scripting.hxx"

class ScriptXray : public IXray {
public:
    ScriptXray(EventManager* events);

    void update() override;
private:
    void render_current_script(Script *current);

    bool on_script_loaded(ScriptLoadedEvent& e);
    bool on_script_reset(ScriptResetEvent& e);

    struct GlobalsState {
        bool visible = false;
        int ticks = 0;
        std::vector<std::tuple<std::string, std::string, std::string>> rows;
    } globals_;

    // ui vars
    std::optional<u64> selected_script_id_;
    Script* current_script_;

    void update_script_globals_table();
};

#endif
