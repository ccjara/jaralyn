#include "ui_node.hxx"

void UiNode::move_anchors() {
    for (const auto n : anchored_by_) {
        if (!n) {
            continue;
        }
        auto position = n->relative_position_ + calc_anchor_offset(*n, *this);

        switch (n->align_x_) {
            case AlignX::Left:
            default:
                break;
            case AlignX::Center:
                position.x -= n->size_.x / 2;
                break;
            case AlignX::Right:
                position.x -= n->size_.x;
        }
        switch (n->align_y_) {
            case AlignY::Top:
            default:
                break;
            case AlignY::Center:
                position.y -= n->size_.y / 2;
                break;
            case AlignY::Bottom:
                position.y -= n->size_.y;
        }

        n->move_abs(position);
    }
}

void UiNode::move(Vec2<i32> position) {
    relative_position_ = position;
    if (anchored_to_) {
        position += calc_anchor_offset(*this, *anchored_to_);

        switch (align_x_) {
            case AlignX::Left:
            default:
                break;
            case AlignX::Center:
                position.x -= size_.x / 2;
                break;
            case AlignX::Right:
                position.x -= size_.x;
        }
        switch (align_y_) {
            case AlignY::Top:
            default:
                break;
            case AlignY::Center:
                position.y -= size_.y / 2;
                break;
            case AlignY::Bottom:
                position.y -= size_.y;
        }
    }
    move_abs(position);
}

void UiNode::move(i32 x, i32 y) {
    return move(Vec2<i32>(x, y));
}

void UiNode::move_abs(Vec2<i32> pos) {
    absolute_position_ = pos;
    move_anchors();
}

void UiNode::set_anchor_origin(AnchorOrigin origin) {
    anchor_origin_ = origin;
    move(relative_position_);
}

void UiNode::resize(Vec2<u32> size) {
    size_ = size;
    // anchored nodes may update their positions due to their anchor origin
    move_anchors();
}

void UiNode::resize(u32 width, u32 height) {
    return resize(Vec2<u32>(width, height));
}

void UiNode::anchor_to(UiNode* node) {
    if (node == nullptr) {
        node = root_;
    }
    if (!can_anchor_to(node)) {
        return;
    }
    if (anchored_to_ && anchored_to_ != this) {
        // remove this node from the current anchor's list
        auto& foreign_anchors { anchored_to_->anchored_by_ };
        for (auto iter = foreign_anchors.begin(); iter != foreign_anchors.end(); ++iter) {
            if (*iter == this) {
                iter = foreign_anchors.erase(iter);
                break;
            }
        }
    }
    // assign new references on both sides
    node->anchored_by_.emplace_back(this);
    anchored_to_ = node;
    // update position after anchor was changed
    move(relative_position_);
}

void UiNode::set_parent(UiNode* parent) {
    if (parent == nullptr) {
        parent = root_;
    }
    if (parent_) {
        for (auto it = parent_->children_.begin(); it != parent_->children_.end(); ++it) {
            if (*it == this) {
                parent_->children_.erase(it);
                break;
            }
        }
    }
    parent_ = parent;
    parent->children_.emplace_back(this);
}

void UiNode::set_handler_id(Id handler_id) {
    handler_id_ = handler_id;
}

Id UiNode::handler_id() const {
    return handler_id_;
}

const std::string& UiNode::id() const {
    return id_;
}

UiNodeType UiNode::type() const {
    return type_;
}

Vec2<i32> UiNode::relative_position() const {
    return relative_position_;
}

Vec2<i32> UiNode::absolute_position() const {
    return absolute_position_;
}

UiNode* UiNode::anchored_to() const {
    return anchored_to_;
}

AnchorOrigin UiNode::anchor_origin() const {
    return anchor_origin_;
}

const std::vector<UiNode*>& UiNode::children() const {
    return children_;
}

const std::vector<UiNode*>& UiNode::anchored_by() const {
    return anchored_by_;
}

Vec2<u32> UiNode::size() const {
    return size_;
}

bool UiNode::is_root() const {
    return parent_ == nullptr;
}

bool UiNode::can_anchor_to(UiNode* node) const {
    return !is_root() && node != this;
}

bool UiNode::visible() const {
    return visible_;
}

void UiNode::show() {
    visible_ = true;
}

void UiNode::hide() {
    visible_ = false;
}

void UiNode::set_align_x(AlignX align_x) {
    align_x_ = align_x;
    move(relative_position_);
}

void UiNode::set_align_y(AlignY align_y) {
    align_y_ = align_y;
    move(relative_position_);
}

void UiNode::set_width(u32 width) {
    size_.x = width;
}

void UiNode::set_height(u32 height) {
    size_.y = height;
}

AlignX UiNode::align_x() const {
    return align_x_;
}

AlignY UiNode::align_y() const {
    return align_y_;
}

Vec2<i32> calc_anchor_offset(
    const UiNode& target,
    const UiNode& anchor
) {
    switch (target.anchor_origin()) {
        case AnchorOrigin::TopLeft:
        default:
            return anchor.absolute_position();
        case AnchorOrigin::Top:
            return {
                (anchor.absolute_position().x + static_cast<i32>(anchor.size().x)) / 2,
                anchor.absolute_position().y
            };
        case AnchorOrigin::TopRight:
            return {
                anchor.absolute_position().x + static_cast<i32>(anchor.size().x),
                anchor.absolute_position().y
            };
        case AnchorOrigin::Right:
            return {
                anchor.absolute_position().x + static_cast<i32>(anchor.size().x),
                (anchor.absolute_position().y + static_cast<i32>(anchor.size().y)) / 2,
            };
        case AnchorOrigin::BottomRight:
            return anchor.absolute_position() + Vec2<i32>(anchor.size());
        case AnchorOrigin::Bottom:
            return {
                (anchor.absolute_position().x + static_cast<i32>(anchor.size().x)) / 2,
                anchor.absolute_position().y + static_cast<i32>(anchor.size().y),
            };
        case AnchorOrigin::BottomLeft:
            return {
                anchor.absolute_position().x,
                anchor.absolute_position().y + static_cast<i32>(anchor.size().y),
            };
        case AnchorOrigin::Left:
            return {
                anchor.absolute_position().x,
                (anchor.absolute_position().y + static_cast<i32>(anchor.size().y)) / 2,
            };
        case AnchorOrigin::Center:
            return {
                (anchor.absolute_position().x + static_cast<i32>(anchor.size().x)) / 2,
                (anchor.absolute_position().y + static_cast<i32>(anchor.size().y)) / 2,
            };
    }
}
