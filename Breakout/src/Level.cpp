#include "Level.h"

#include <fstream>
#include <sstream>

Level::Level()
{

}

Level::~Level()
{

}

void Level::loadLevel(const char* filePath, uint32_t levelWidth, uint32_t levelHeight)
{
    //Clear old data
    this->m_bricks.clear();

    unsigned int tileCode;
    std::string line;
    std::ifstream fstream(filePath);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) //read each line from the level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;

            while (sstream >> tileCode) //read each
            {
                row.push_back(tileCode);
            }

            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            init(tileData, levelWidth, levelHeight);
    }
}

void Level::onRender()
{

    for (Bricks& brick : m_bricks)
    {
        
        if (!brick.isDestroyed())
            brick.onRender();

    }
}

void Level::init(std::vector<std::vector<uint32_t>> tileData, uint32_t levelWidth, uint32_t levelHeight)
{
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size();

    float unit_width = levelWidth / static_cast<float>(width), unit_height = levelHeight / static_cast<float>(height);

    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            if (tileData[y][x] == 1) //1 is a solid block
            {
                glm::vec3 position((unit_width / 2) + (unit_width * x), (unit_height / 2) + (unit_height * y), 0);
                glm::vec2 size(unit_width, unit_height);

                //Shader shader("Assets/Shaders/vertexShader.shader", "Assets/Shaders/fragmentShader.shader");

                Bricks brick("assets/Breakout/textures/UnbreakableBlock.png");
                brick.loadAssets();
                brick.setSolid(true);
                brick.setPosition(position);
                brick.setSize(size);
                brick.setColor({ 0.8f, 0.7f, 0.6f, 1.0f });
                this->m_bricks.push_back(brick);
            }
            else if (tileData[y][x] > 1)
            {
                glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                if (tileData[y][x] == 2)
                    color = glm::vec4(0.2f, 0.6f, 1.0f, 1.0f);
                else if (tileData[y][x] == 3)
                    color = glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
                else if (tileData[y][x] == 4)
                    color = glm::vec4(0.8f, 0.8f, 0.4f, 1.0f);
                else if (tileData[y][x] == 5)
                    color = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);

                //Shader shader("Assets/Shaders/vertexShader.shader", "Assets/Shaders/fragmentShader.shader");

                glm::vec3 position((unit_width / 2) + (unit_width * x), (unit_height / 2) + (unit_height * y), 0);
                glm::vec2 size(unit_width, unit_height);

                Bricks brick("assets/Breakout/textures/Squareblock.png");
                brick.loadAssets();
                brick.setPosition(position);
                brick.setSize(size);
                brick.setColor(color);
                this->m_bricks.push_back(brick);
            }
        }
    }
}
