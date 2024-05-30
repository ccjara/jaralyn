#include "platform/window_handle.hxx"
#include "platform/window.hxx"
#include "platform/platform.hxx"
#include "platform/platform_api.hxx"

Platform::Platform(std::unique_ptr<IPlatformApi>&& api) : api_(std::move(api)) {
    assert(api_);
}

std::shared_ptr<Window> Platform::create_window(const std::string& title, Vec2<i32> size) {
    if (window_) {
        return nullptr;
    }

    auto handle = api_->create_window(title, size);

    if (!handle) {
        return nullptr;
    }

    window_ = std::shared_ptr<Window>(new Window());
    window_->size_ = size;
    window_->title_ = title;
    window_->handle_ = std::move(handle);

    return window_;
}

std::shared_ptr<Window> Platform::window() {
    return window_;
}

SDL_Window* Platform::sdl_window() {
    if (!window_) {
        return nullptr;
    }
    return api_->sdl_window(*window_->handle().get());
}

void Platform::shutdown() {
    if (window_) {
        window_.reset();
    }
    SDL_Quit();
}

Platform::~Platform() {
    shutdown();
}
