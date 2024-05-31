#ifndef JARALYN_GFX_EVENT_HXX
#define JARALYN_GFX_EVENT_HXX

/**
 * @brief Triggered after the display has been resized
 *
 * The display is resized in the gfx system just after the low level event
 * ResizeEvent has been handled there (for example due to a window resize).
 */
struct DisplayResizedEvent {
    /**
     * @brief The new display size in cell units
     */
    Vec2<u32> size;

    constexpr DisplayResizedEvent(Vec2<u32> size) : size(size) {
    }
};

#endif
