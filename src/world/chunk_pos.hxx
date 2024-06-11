#ifndef NOEDEN_CHUNK_POS_HXX
#define NOEDEN_CHUNK_POS_HXX

struct ChunkPos {
    i32 x;
    i32 z;

    ChunkPos();
    ChunkPos(i32 x, i32 z);
    ChunkPos(const ChunkPos& other) = default;
    ChunkPos(ChunkPos&& other) = default;

    bool operator==(const ChunkPos& other) const;
    bool operator!=(const ChunkPos& other) const;
};

#endif
