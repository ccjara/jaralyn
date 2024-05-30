#ifndef JARALYN_WINDOW_HXX
#define JARALYN_WINDOW_HXX

class IWindowHandle;
class ResizeEvent;

/**
 * @brief Represents the game window by owning an SDL_Window
 */
class Window {
public:
    friend class Platform;

    void attach(EventManager& events);

    std::unique_ptr<IWindowHandle>& handle();

    Vec2<u32> size() const;

    void close();

    ~Window();
private:
    Window() = default;
    bool on_resize(const ResizeEvent& event);

    Vec2<u32> size_;
    std::string title_;
    std::unique_ptr<IWindowHandle> handle_ = nullptr;
};

#endif
