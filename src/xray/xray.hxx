#ifndef NOEDEN_XRAY_HXX
#define NOEDEN_XRAY_HXX

#include "xray/xray_interface.hxx"

struct MouseDownEvent;
struct MouseUpEvent;
struct KeyDownEvent;
struct KeyUpEvent;

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

    static void init(Events* events);
    static void draw();
    static void shutdown();
private:
    /**
     * @brief Whether the Xray interface is currently visible
     */
    static inline bool show_xray_ = true;

    static EventResult on_mouse_down(MouseDownEvent& e);
    static EventResult on_mouse_up(MouseUpEvent& e);
    static EventResult on_key_down(KeyDownEvent& e);
    static EventResult on_key_up(KeyUpEvent& e);

    static inline std::vector<std::unique_ptr<IXray>> xrays_;
};

#endif
