#ifndef JARALYN_GRID_HXX
#define JARALYN_GRID_HXX

template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
concept Indexable = requires(T a) {
    { a.x } -> std::convertible_to<std::size_t>;
    { a.y } -> std::convertible_to<std::size_t>;
};


/**
 * @brief Generic implementation of a two-dimensional grid
 *
 * Most methods which access specific locations using a templatized `location_type`
 * accept either coordinates packed into a `Vec2<u32>` or an integral typed index
 *
 * Grid is not suitable for very large dimensions, i.e. dimensions which would
 * produce indices larger than u32 could represent. (TODO: assertions)
 */
template<typename cell>
class Grid {
public:
    using cell_type = cell;
    using container_type = std::vector<cell_type>;
protected:
    container_type cells_;
    Vec2<u32> dimensions_;

    /**
     * @brief Helper which guarantees an index location return type
     *
     * If the `location_type` is an index, it will be returned as-is
     * otherwise the location is converted from the assumed index location.
     */
    template<typename location_type>
    [[nodiscard]] constexpr size_t ensure_index(location_type location) const {
        if constexpr (std::is_unsigned_v<location_type>) {
            return static_cast<size_t>(location);
        }
        return to_index(location);
    }

    /**
     * @brief Helper which guarantees a coordinate location return type
     *
     * If the `location_type` is a coordinate, it will be returned as-is
     * otherwise the location is converted from the assumed index location.
     */
    template<typename location_type>
    [[nodiscard]] constexpr Vec2<u32> ensure_coordinates(location_type location) const {
        if constexpr (std::is_integral_v<location_type>) {
            return to_coordinates(location);
        }
        return location;
    }
public:
    /**
     * @brief Resets and resizes the grid to the given cell dimensions
     */
    constexpr void resize(const Vec2<u32>& dimensions) {
        cells_.resize(dimensions.x * dimensions.y);
        dimensions_ = dimensions;
        reset();
    }

    /**
     * @brief Fills all cells in the grid with a copy of the null cell
     */
    constexpr void reset() {
        std::fill(cells_.begin(), cells_.end(), cell{});
    }

    /**
     * @brief Returns a pointer to the first element of the internal container
     */
    [[nodiscard]] constexpr const cell* data() const {
        return cells_.data();
    }

    /**
     * @brief Returns the amount of cells in the grid
     */
    [[nodiscard]] constexpr size_t cell_count() const {
        return cells_.size();
    }

    /**
     * @brief Returns the total grid size in bytes
     */
    [[nodiscard]] constexpr size_t byte_size() const {
        return cells_.size() * sizeof(cell);
    }

    /**
     * @brief Returns the width and height in of the grid
     */
    [[nodiscard]] constexpr Vec2<u32> dimensions() const {
        return dimensions_;
    }

    /**
     * @brief Returns the width and height in of the grid
     */
    [[nodiscard]] constexpr container_type& cells() {
        return cells_;
    }

    /**
     * @brief Returns a pointer to the cell at the given location
     *
     * Returns null if the location does not point to an existing cell
     */
    template<typename location_type>
    [[nodiscard]] constexpr cell* at(location_type location) {
        if (!in_bounds(location)) {
            return nullptr;
        }
        return &cells_[ensure_index(location)];
    }

    /**
     * @brief Returns a pointer to the immutable cell at the given location
     *
     * Returns null if the location does not point to an existing cell
     */
    template<typename location_type>
    [[nodiscard]] constexpr const cell* at(location_type location) const {
        if (!in_bounds(location)) {
            return nullptr;
        }
        return &cells_[ensure_index(location)];
    }

    template<typename fn>
    void each(fn callback) {
        for (auto& cell : cells_) {
            callback(cell);
        }
    }


    /**
     * @brief Assigns the given cell to the given location
     */
    template<typename cel, typename location_type>
    constexpr void put(cel&& c, location_type location) {
        if (!in_bounds(location)) {
            const auto coord { ensure_coordinates(location) };

            Log::error(
                "Placement at {}, {}, not within bounds ({}, {})",
                coord.x, coord.y, dimensions_.x, dimensions_.y
            );
            return;
        }
        cells_[ensure_index(location)] = std::forward<cel>(c);
    }

    /**
     * @brief Returns whether the given location is contained by the grid
     *
     * You may either pass a Vec2<u32> to indicate a cartesian coordinate
     * or an index, in which case `location_type` must be integral.
     */
    [[nodiscard]] constexpr bool in_bounds(Indexable auto location) const {
        if constexpr (std::is_signed_v<decltype(location.x)>) {
            if (location.x < 0 || location.y < 0) {
                return false;
            }
        }
        return static_cast<std::size_t>(location.x) + 1 <= dimensions_.x &&
               static_cast<std::size_t>(location.y) + 1 <= dimensions_.y;
    }

    /**
     * @brief Returns whether the given location is contained by the grid
     */
    [[nodiscard]] constexpr bool in_bounds(Integral auto location) const {
        return location < cells_.size();
    }

    /**
     * @brief Clamps (mutates) the given coordinates to the grid dimensions
     *
     * This ensures that the coordinates are contained by the grid
     */
    template<typename coord_type>
    constexpr void clamp(Vec2<coord_type>& coord) const {
        if (coord.x > dimensions_.x) {
            coord.x = static_cast<coord_type>(dimensions_.x);
        }
        if (coord.y > dimensions_.y) {
            coord.y = static_cast<coord_type>(dimensions_.y);
        }
    }

    /**
     * @brief Converts the given index to grid coordinates
     *
     * This does *not* verify the resulting coordinates.
     */
    [[nodiscard]] constexpr Vec2<u32> to_coordinates(size_t index) const {
        return Vec2<u32> {
            static_cast<u32> (index) % dimensions_.x,
            static_cast<u32> (index) / dimensions_.x
        };
    }

    /**
     * @brief Converts the given coordinates to an index
     *
     * This method does *not* verify the resulting index.
     */
    template<typename coord_type>
    [[nodiscard]] constexpr size_t to_index(Vec2<coord_type> coord) const {
        return static_cast<u32> (coord.y) * dimensions_.x + static_cast<u32> (coord.x);
    }
};

#endif
