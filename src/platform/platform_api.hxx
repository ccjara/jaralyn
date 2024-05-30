#ifndef JARALYN_PLATFORM_API_HXX
#define JARALYN_PLATFORM_API_HXX

class IWindowHandle;

class IPlatformApi {
public:
    virtual std::unique_ptr<IWindowHandle> create_window(const std::string& title, Vec2<i32> size) = 0;

    virtual void destroy_window(IWindowHandle& handle) = 0;

    // TODO: remove after renderer abstraction
    virtual SDL_Window* sdl_window(IWindowHandle& handle) = 0;
};

std::unique_ptr<IPlatformApi> create_platform_api();

#endif
