#include "platform/platform_api.hxx"
#include "platform/window_handle_impl.hxx"

class PlatformApi : public IPlatformApi {
public:
    std::unique_ptr<IWindowHandle> create_window(const std::string& title, Vec2<i32> size) override {
        SDL_Window* sdl_window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            size.x,
            size.y,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
        );

        return std::make_unique<WindowHandle>(sdl_window);
    }

    SDL_Window* sdl_window(IWindowHandle& handle) override {
        return static_cast<WindowHandle&>(handle).as_sdl_window();
    }

    void destroy_window(IWindowHandle& handle) override {
        auto& window_handle = static_cast<WindowHandle&>(handle);

        window_handle.destroy();
    }
};

std::unique_ptr<IPlatformApi> create_platform_api() {
    return std::make_unique<PlatformApi>();
}
