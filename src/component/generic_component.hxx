#ifndef JARALYN_GENERIC_COMPONENT_HXX
#define JARALYN_GENERIC_COMPONENT_HXX

#include "component/component.hxx"

/**
 * @brief CRTP helper which must be the direct parent of all component specializations.
 */
template<typename Derived, ComponentType Type>
class GenericComponent : public Component {
public:
    explicit GenericComponent() : Component(Type) {
    }

    /**
     * @brief Returns the ComponentType template argument value.
     *
     * NOTE: Is there a better way of accessing it?
     */
    static consteval ComponentType static_type() {
        return Type;
    }

    [[nodiscard]] std::unique_ptr<Component> clone() const override {
        return std::make_unique<Derived>(static_cast<const Derived&>(*this));
    }
};

#endif
