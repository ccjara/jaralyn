#ifndef JARALYN_WINDOW_HANDLE_IMPL_HXX
#define JARALYN_WINDOW_HANDLE_IMPL_HXX

#include "platform/window_handle.hxx"

class WindowHandle : public IWindowHandle {
public:
    explicit WindowHandle(SDL_Window* handle) : handle_(handle) {
    }

    operator bool() const override {
        return handle_ != nullptr;
    }

    SDL_Window* as_sdl_window() {
        return handle_;
    }

    void destroy() {
        if (handle_) {
            SDL_DestroyWindow(handle_);
            handle_ = nullptr;
        }
    }

    ~WindowHandle() override {
        destroy();
    }

    WindowHandle(const WindowHandle&) = delete;
    WindowHandle& operator=(const WindowHandle&) = delete;
    WindowHandle(WindowHandle&&) = delete;
    WindowHandle& operator=(WindowHandle&&) = delete;
private:
    SDL_Window* handle_ = nullptr;
};

#endif
