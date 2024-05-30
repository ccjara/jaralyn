#ifndef JARALYN_XRAY_HXX
#define JARALYN_XRAY_HXX

#include "gfx/gfx_event.hxx"
#include "platform/window.hxx"
#include "input/input_event.hxx"
#include "xray/xray_interface.hxx"
#include "xray/xray_event.hxx"

class EventManager;

class Xray {
public:
    Xray() = delete;

    /**
     * @brief Adds an xray implementation to the xray container
     */
    template<typename XrayClass, typename... XrayArgs>
    static void add(XrayArgs&&... xray_args) {
        xrays_.emplace_back(new XrayClass(std::forward<XrayArgs>(xray_args)...));
    }

    static void init(EventManager *events, SDL_Window* sdl_window, SDL_GLContext context);
    static void shutdown();
private:
    /**
     * @brief Whether the Xray interface is currently visible
     */
    static inline bool show_xray_ = true;

    static bool on_post_render(PostRenderEvent& e);
    static bool on_mouse_down(MouseDownEvent& e);
    static bool on_mouse_up(MouseUpEvent& e);
    static bool on_key_down(KeyDownEvent& e);
    static bool on_key_up(KeyUpEvent& e);

    static inline ImGuiContext* imgui_context_ = nullptr;
    static inline SDL_Window* sdl_window_ = nullptr;

    static inline std::vector<std::unique_ptr<IXray>> xrays_;
};

#endif
