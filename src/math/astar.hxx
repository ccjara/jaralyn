#ifndef JARALYN_ASTAR_HXX
#define JARALYN_ASTAR_HXX

#include "../grid.hxx"

struct AStarCell {
    /**
     * @brief Estimated cost from start to dest through this node
     */
    float f = std::numeric_limits<float>::max();

    /**
     * @brief Cost from start to this node
     */
    u32 g = 0;

    /**
     * @brief Previous node in the walked path or nullptr on the start node
     */
    AStarCell* parent = nullptr;

    /**
     * @brief Position of this node inside the A* Grid
     */
    Vec2<i32> position;

    /**
     * @brief Marks this node as visited, i.e. walked to
     */
    void visit();

    /**
     * @brief Marks this node as a blocker meaning it cannot be walked to
     */
    void block();

    /**
     * @brief Marks this node as considered when constructing a path to the destination.
     *
     * Used as a unique constraint for the presence of AStarCells in the priority queue.
     */
    void consider();

    /**
     * @brief Returns whether this cell is passable for the path finder.
     *
     * Inverse of blocked()
     */
    [[nodiscard]] bool passable() const;

    /**
     * @brief Returns whether this cell is blocked for the path finder.
     *
     * Inverse of passable()
     */
    [[nodiscard]] bool blocked() const;

    /**
     * @brief Whether this cell has been visited in the last run of the trace
     */
    [[nodiscard]] bool visited() const;

    /**
     * @brief Whether this cell is considered for the trace and therefore present in the priority queue.
     */
    [[nodiscard]] bool considered() const;
private:
    enum CellFlag {
        None = 0,
        Blocked = 1,
        Considered = 2,
        Visited = 4,
    };
    i32 flags = CellFlag::None;
};

enum AStarSearchError {
    /**
     * @brief A path from start to dest was established or empty if start equals dest
     */
    None,

    /**
     * @brief No uninterrupted path from start to dest could be determined
     */
    PathNotFound,

    /**
     * @brief The start point is out of bounds
     */
    StartInvalid,

    /**
     * @brief The dest point is out of bounds
     */
    DestInvalid,

    /**
     * @brief The start point is blocked
     */
    StartBlocked,

    /**
     * @brief The dest point is blocked
     */
    DestBlocked,
};

/**
 * @brief Represents a completed AStar search
 */
struct AStarSearch {
    /**
     * @brief Start of the path finder
     */
    Vec2<i32> start;

    /**
     * @brief Destination of the path finder
     */
    Vec2<i32> dest;

    /**
     * @brief The computed path to follow to reach dest from start
     */
    std::vector<Vec2<i32>> path;

    /**
     * @brief Path finding result descriptor. Nonzero if erroneous.
     */
    AStarSearchError error;
};

/**
 * @brief Returns true if the AStarSearch results have the same start and destination
 */
[[nodiscard]] bool operator==(const AStarSearch& lhs, const AStarSearch& rhs);

/**
 * @brief Performs an A* search on the given grid using the euclidean distance heuristic.
 *
 * Returns a path of coordinates of the optimal path to reach the destination.
 */
[[nodiscard]] AStarSearch astar_search(Grid<AStarCell>& grid, const Vec2<i32>& start, const Vec2<i32>& dest);

#endif
