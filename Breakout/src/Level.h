#pragma once

#include "Bricks.h"

class Level
{
public:
    Level();
    ~Level();

    void loadLevel(const char* filePath, uint32_t levelWidth, uint32_t levelHeight);
    void onRender();

    std::vector<Bricks>& getBricks() { return m_bricks; }
private:
    void init(std::vector<std::vector<uint32_t>> tileData, uint32_t levelWidth, uint32_t levelHeight);
private:
    std::vector<Bricks> m_bricks;
    bool m_isComplete;
};