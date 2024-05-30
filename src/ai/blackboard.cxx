#include "ai/blackboard.hxx"

void Blackboard::set(const std::string& key, std::any value) {
    data[key] = std::move(value);
}

void Blackboard::unset(const std::string& key) {
    data.erase(key);
}

void Blackboard::clear() {
    data.clear();
}
