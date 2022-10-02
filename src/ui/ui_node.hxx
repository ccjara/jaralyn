#ifndef JARALYN_UI_NODE_HXX
#define JARALYN_UI_NODE_HXX

/**
 * @brief Affects position of elements, allowing relative positioning
 */
enum class AnchorOrigin {
    Top,
    TopRight,
    Right,
    BottomRight,
    Bottom,
    BottomLeft,
    Left,
    TopLeft,
    Center,
};

enum class AlignX {
    Left,
    Center,
    Right,
};

enum class AlignY {
    Top,
    Center,
    Bottom,
};

/**
 * @brief RTTI for ui nodes
 */
enum class UiNodeType {
    generic_wrapper,
    window,
    text,
};

class UiNode {
    friend class UiTree;
public:
    /**
     * @brief Readonly accessor for the id_ attribute
     *
     * Written to by the factory after construction
     */
    const std::string& id() const;

    /**
     * @brief Readonly accessor for the type_ attribute
     *
     * Written to by the factory after construction
     */
    UiNodeType type() const;

    /**
     * @brief Updates the current lua handler for this node
     */
    void set_handler_id(Id handler_id);

    /**
     * @brief Sets the current, relative (untranslated) position of this node
     *
     * Its absolute position will be calculated based on its anchor.
     *
     * Also recursively moves all anchored nodes based on their anchor settings.
     */
    void move(Vec2<i32> position);
    void move(i32 x, i32 y);

    /**
     * @brief Writing accessor for the size_ attribute
     */
    virtual void resize(Vec2<u32> size);
    virtual void resize(u32 width, u32 height);

    /**
     * @brief Returns the position relative to its anchor
     *
     * This position may be modified using move()
     *
     * @see move()
     */
    Vec2<i32> relative_position() const;

    /**
     * @brief Returns the translated position (as it will appear on screen)
     *
     * This value depends on the relative position and anchoring and is updated
     * whenever the move() function adjusts the relative position of a node.
     *
     * @see move()
     */
    Vec2<i32> absolute_position() const;

    /**
     * @brief Anchors this node to the given node
     *
     * When given a nullptr the node will be anchored to the root node
     * Existing anchor references will be updated accordingly.
     */
    void anchor_to(UiNode* node);

    /**
     * @brief Updates the anchor origin and recalculates its absolute position
     */
    void set_anchor_origin(AnchorOrigin origin);

    /**
     * @brief Returns the node this node is currently anchored to
     */
    UiNode* anchored_to() const;

    /**
     * @brief Readonly accessor for the anchored_by_ attribute
     */
    const std::vector<UiNode*>& anchored_by() const;

    /**
     * @brief Readonly accessor for the origin_ attribute
     */
    AnchorOrigin anchor_origin() const;

    /**
     * @brief Readonly accessor for the children_ attribute
     */
    const std::vector<UiNode*>& children() const;

    /**
     * @brief Returns true if this node can be anchored to the given one
     */
    bool can_anchor_to(UiNode* node) const;

    /**
     * @brief Sets the x alignment
     */
    void set_align_x(AlignX align_x);

    /**
     * @brief Sets the y alignment
     */
    void set_align_y(AlignY align_y);

    /**
     * @brief Readonly accessor for the size_ attribute
     */
    Vec2<u32> size() const;

    /**
     * @brief Returns true if the node is the root node
     */
    bool is_root() const;

    /**
     * @brief Readonly accessor for the visible_ attribute
     */
    bool visible() const;

    /**
     * @brief Readonly accessor for the handler_id_ attribute
     */
    [[nodiscard]] Id handler_id() const;

    /**
     * @brief Assigns the parent and updates the references in the parent and the child
     *
     * When given a nullptr, the root node will become the parent.
     */
    void set_parent(UiNode* parent);

    /**
     * @brief Updates the width of the node
     *
     * @see resize()
     */
    virtual void set_width(u32 width);

    /**
     * @brief Updates the height of the node
     *
     * @see resize()
     */
    virtual void set_height(u32 height);

    /**
     * @brief Sets visibility to true
     */
    void show();

    /**
     * @brief Sets visibility to false
     */
    void hide();

    /**
     * @brief Readonly accessor for the align_x_ attribute
     */
    AlignX align_x() const;

    /**
     * @brief Readonly accessor for the align_y_ attribute
     */
    AlignY align_y() const;
protected:
    /**
     * @brief A list of nodes anchored to this node
     *
     * When accessing this list, check for nullptrs as the list is not
     * defragmented on every frame.
     */
    std::vector<UiNode*> anchored_by_;

    /**
     * @brief A destination node this node is anchored to
     *
     * If the destination node is removed and this node is not a child node
     * thereof this node will be re-anchored to the root node.
     */
    UiNode* anchored_to_ { nullptr };

    /**
     * @brief Origin used to calculate an absolute position
     */
    AnchorOrigin anchor_origin_ = AnchorOrigin::TopLeft;

    /**
     * @brief Unique id referencing this node
     */
    std::string id_;

    /**
     * @brief RTTI
     */
    UiNodeType type_ = UiNodeType::generic_wrapper;

    /**
     * @brief Logical grouping of several nodes, does not affect positioning, but existence.
     *
     * If a parent container is removed, all child containers are also removed.
     */
    std::vector<UiNode*> children_;

    /**
     * @brief Containing parent node of this node, always set unless root
     */
    UiNode* parent_ { nullptr };

    /**
     * @brief Pointer to the root node
     *
     * Guaranteed to be non-null after initialization of the root node.
     * Points to itself if the instance is the root node.
     */
    UiNode* root_ { nullptr };

    /**
     * @brief Relative position as specified by the ui code
     *
     * An absolute position will be calculated using anchor information.
     *
     * @see absolute_position
     * @see anchor
     */
    Vec2<i32> relative_position_;

    /**
     * @brief Absolute position, used by the renderer
     *
     * The absolute position should not be written to directly by ui code.
     * The UiTree implementation is responsible to arrange all nodes
     * based on their relative position and anchor.
     *
     * @see relative_position
     */
    Vec2<i32> absolute_position_;

    /**
     * @brief Size of this node, affects positioning of anchored nodes
     */
    Vec2<u32> size_;

    /**
     * @brief Visibility behavior is node specific
     */
    bool visible_ = false;

    /**
     * @brief Cleanup marker
     */
    bool destroyed_ = false;

    /**
     * @brief Updates the position of this node's anchors
     */
    void move_anchors();

    /**
     * @brief Id of a lua callback that will be invoked to update this node
     */
    Id handler_id_ = null_id;

    /**
     * @brief Horizontal alignment (requires known size)
     */
    AlignX align_x_ = AlignX::Left;

    /**
     * @brief Vertical alignment (requires known size)
     */
    AlignY align_y_ = AlignY::Top;
private:
    /**
     * @brief Assigns an absolute position to this node
     *
     * If the node has anchored nodes, these will be repositioned
     * subsequently, repeating the process recursively for all anchors
     * until no anchors are left.
     *
     * Note that this method assigns the position as-is so the given absolute
     * position must be verified.
     */
    void move_abs(Vec2<i32> pos);
};

Vec2<i32> calc_anchor_offset(
    const UiNode& target,
    const UiNode& parent
);

#endif
