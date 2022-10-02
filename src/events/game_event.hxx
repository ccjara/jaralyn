#ifndef JARALYN_GAME_EVENT_HXX
#define JARALYN_GAME_EVENT_HXX

struct InventoryEvent {
    explicit InventoryEvent(Id entity_id) : entity_id(entity_id) {

    }

    /**
     * @brief Id of inventory owning entity which may be null_id
     */
    Id entity_id = null_id;
};

#endif
