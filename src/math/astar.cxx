#include "astar.hxx"

[[nodiscard]] bool operator==(const AStarSearch& lhs, const AStarSearch& rhs) {
    return lhs.start == rhs.start && lhs.dest == rhs.dest;
}

AStarSearch astar_search(Grid<AStarCell>& grid, const Vec2<i32>& start, const Vec2<i32>& dest) {
    AStarSearch search;
    search.start = start;
    search.dest = dest;

    if (start == dest) {
        return search;
    }
    if (!grid.in_bounds(start)) {
        search.error = AStarSearchError::StartInvalid;
        return search;
    }
    if (!grid.in_bounds(dest)) {
        search.error = AStarSearchError::DestInvalid;
        return search;
    }
    auto start_cell = grid.at(start);
    if (start_cell->blocked()) {
        search.error = AStarSearchError::StartBlocked;
        return search;
    }
    const auto dest_cell = grid.at(dest);
    if (grid.at(dest)->blocked()) {
        search.error = AStarSearchError::DestBlocked;
        return search;
    }
    constexpr auto cost_priority = [](const AStarCell* a, const AStarCell* b) -> bool {
        return a->f > b->f; // note: min_heap so
    };
    std::priority_queue<AStarCell*, std::vector<AStarCell*>, decltype(cost_priority)> fringe;

    start_cell->f = 0;
    start_cell->g = 0;
    fringe.emplace(start_cell);

    while (!fringe.empty()) {
        AStarCell* cur_cell = fringe.top();
        fringe.pop();
        cur_cell->visit();

        Vec2<i32> offset;

        for (offset.y = -1; offset.y <= 1; ++offset.y) {
            for (offset.x = -1; offset.x <= 1; ++offset.x) {
                const Vec2<i32> neighbor_pos(cur_cell->position + offset);
                if (!grid.in_bounds(neighbor_pos)) {
                    continue;
                }
                auto neighbor_cell = grid.at(neighbor_pos);

                if (neighbor_pos == dest) {
                    // build path and end...FIXME: messy code (we need to go from end to start but the list should be start to end)
                    neighbor_cell->g = cur_cell->g + 1;
                    neighbor_cell->parent = cur_cell;
                    AStarCell* cell = neighbor_cell;
                    search.path.resize(neighbor_cell->g + 1);
                    while (cell != nullptr) {
                        search.path[static_cast<i32>(cell->g)] = cell->position;
                        cell = cell->parent;
                    }
                    return search;
                }
                if (!neighbor_cell->visited() && !neighbor_cell->blocked()) {
                    const u32 gNew = cur_cell->g + 1;
                    const float fNew = static_cast<float>(gNew) + (neighbor_pos - dest).length();

                    if (fNew < neighbor_cell->f) {
                        if (!neighbor_cell->considered()) {
                            fringe.emplace(neighbor_cell);
                            neighbor_cell->consider();
                        }
                        neighbor_cell->g = gNew;
                        neighbor_cell->f = fNew;
                        neighbor_cell->parent = cur_cell;
                    }
                }
            }
        }
    }
    search.error = AStarSearchError::PathNotFound;
    return search;
}

void AStarCell::visit() {
    flags |= CellFlag::Visited;
}

void AStarCell::block() {
    flags |= CellFlag::Blocked;
}

void AStarCell::consider() {
    flags |= CellFlag::Considered;
}

bool AStarCell::passable() const {
    return (flags & CellFlag::Blocked) == 0;
}

bool AStarCell::blocked() const {
    return (flags & CellFlag::Blocked) == CellFlag::Blocked;
}

bool AStarCell::visited() const {
    return (flags & CellFlag::Visited) == CellFlag::Visited;
}

bool AStarCell::considered() const {
    return (flags & CellFlag::Considered) == CellFlag::Considered;
}
