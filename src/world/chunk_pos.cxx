#include "chunk_pos.hxx"

ChunkPos::ChunkPos() : x(0), z(0) {

}

ChunkPos::ChunkPos(i32 x, i32 z) : x(x), z(z) {

}

bool ChunkPos::operator==(const ChunkPos& other) const {
    return x == other.x && z == other.z;
}

bool ChunkPos::operator!=(const ChunkPos& other) const {
    return !(*this == other);
}
