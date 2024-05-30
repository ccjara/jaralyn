#include "component/render.hxx"

const DisplayInfo& Render::display_info() const {
    return display_info_;
}

DisplayInfo& Render::display_info() {
    return display_info_;
}
