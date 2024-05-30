#include "platform/window.hxx"
#include "platform/window_handle.hxx"
#include "platform/platform_event.hxx"

void Window::attach(EventManager& events) {
    events.on<ResizeEvent>(this, &Window::on_resize);
}

bool Window::on_resize(const ResizeEvent& event) {
    size_ = event.size;
    return false;
}

Vec2<u32> Window::size() const {
    return size_;
}

std::unique_ptr<IWindowHandle>& Window::handle() {
    return handle_;
}

Window::~Window() {
    close();
}

void Window::close() {
    if (handle_) {
        handle_.reset();
    }
}