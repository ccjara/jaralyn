#ifndef JARALYN_BLACKBOARD_HXX
#define JARALYN_BLACKBOARD_HXX

class Blackboard {
public:
    /**
     * @brief Sets a value in the blackboard, overwriting any existing value
     */
    void set(const std::string& key, std::any value);

    /**
     * @brief Removes an entry from the blackboard by the given key. Does nothing if the key is not found
     */
    void unset(const std::string& key);

    /**
     * @brief Removes all entries from the blackboard
     */
    void clear();

    /**
     * @brief Gets a value from the blackboard, returning nullptr if not found
     */
    template<typename T>
    const T* get(const std::string& key) const {
        auto it = data.find(key);
        if (it != data.end()) {
            return std::any_cast<const T>(&it->second);
        }
        return nullptr;
    }
private:
    std::unordered_map<std::string, std::any> data;
};


#endif