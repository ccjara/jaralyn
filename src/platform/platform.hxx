#ifndef JARALYN_PLATFORM_HXX
#define JARALYN_PLATFORM_HXX

class Input;

class Platform {
public:
    explicit Platform(EventManager* events, Input* input);

    /**
     * @brief Initializes the platform (SDL, OpenGL, ImGui, etc.)
     */
    void initialize();

    /**
     * @brief Prepares a new loop iteration. Must be called at the beginning of the main loop.
     * 
     * @return false if the application should exit.
     */
    bool prepare();

    /**
     * @brief Presents the current frame. 
     * 
     * Must be called at the end of the loop after all drawing operations.
     */
    void present();

    /**
     * @brief Shuts down the platform.
     */
    void shutdown();

    Vec2<u32> window_size() const;

    ~Platform();

    Platform(const Platform&) = delete;
    Platform& operator=(const Platform&) = delete;
    Platform(Platform&&) = delete;
    Platform& operator=(Platform&&) = delete;
private:
    /**
     * @brief Processes incoming OS events
     */
    bool process_events();

    bool sdl_initialized_ = false;
    EventManager* events_ = nullptr;
    SDL_Window* sdl_window_ = nullptr;
    SDL_GLContext gl_context_ = nullptr;
    ImGuiContext* imgui_context_ = nullptr;
    Input* input_ = nullptr;
};



#endif
