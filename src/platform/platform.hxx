#ifndef JARALYN_PLATFORM_HXX
#define JARALYN_PLATFORM_HXX

class IPlatformApi;
class Window;

/*
TODO: renderer doesnt need to know anything about platform initialization (i.e. SDL Init, GL Context Init)
      this should be handled by the platform class.

TODO: Swapping is also a platform concern, so the renderer should not be responsible for this either.
      Swap windows after rendering in the game loop

TODO: Remove Window class and SIMPLIFY platform.

*/

class Platform {
public:
    explicit Platform(std::unique_ptr<IPlatformApi>&& api);

    std::shared_ptr<Window> create_window(const std::string& title, Vec2<i32> size);
    std::shared_ptr<Window> window();

    // TODO: remove after renderer abstraction
    SDL_Window* sdl_window();

    void shutdown();

    ~Platform();
private:
    std::shared_ptr<Window> window_ = nullptr;
    std::unique_ptr<IPlatformApi> api_ = nullptr;
};



#endif
