#ifndef JARALYN_WINDOW_HANDLE_HXX
#define JARALYN_WINDOW_HANDLE_HXX

class IWindowHandle {
public:
    /**
     * @return true if the handle is valid, false otherwise
     */
    virtual operator bool() const = 0;

    virtual ~IWindowHandle() = default;
};

#endif
