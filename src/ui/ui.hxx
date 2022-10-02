#ifndef JARALYN_UI_HXX
#define JARALYN_UI_HXX

#include "../gfx/gfx_event.hxx"
#include "../scripts/script_event.hxx"
#include "../gfx/renderer.hxx"
#include "../scripts/scripting.hxx"
#include "ui_tree.hxx"
#include "ui_window.hxx"
#include "ui_text.hxx"

class Ui {
public:
    static void init();
    static void shutdown();
    static void update();

    static void draw_node(UiNode* node);

    static UiTree& tree();
private:
    Ui() = delete;

    static inline UiTree ui_tree_;

    static bool on_display_resized(DisplayResizedEvent& e);
    static bool on_script_reset(ScriptResetEvent& e);
    static bool on_post_world_render(PostWorldRenderEvent& e);
};

#endif
