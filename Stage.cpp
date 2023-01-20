#include "Stage.h"
#include <fstream>
#include <sstream>
#include <string>
#include "BlockManager.h"
#include "NormalBlock.h"

void Stage::LoadCsv(Camera* pCamera, const fsPath& path)
{
    std::ifstream ifs{ path };

    std::string line, tmp;

    std::array<float, 3> coordinate{};
    IBlock::Type blockType{ IBlock::Type::INIT };

    while (std::getline(ifs, line)) {
        size_t loopX{};

        std::istringstream line_stream{ line };

        while (std::getline(line_stream, tmp, ',')) {
            if (loopX < 3) {
                coordinate[loopX] = std::stof(tmp);
            }
            else if (loopX == 3) {
                blockType = static_cast<IBlock::Type>(std::stoi(tmp));
            }
            else {
                OutputDebugStringA("csv file error. Block information can be up to 4 per line.");
            }
            loopX++;
        }

        switch (blockType) {
        case IBlock::Type::INIT:
            break;
        case IBlock::Type::NORMAL:
            BlockManager::Register(new NormalBlock{ {coordinate.at(0),coordinate.at(1),coordinate.at(2)},pCamera });
            break;
        }

        coordinate.fill(0.f);
        blockType = IBlock::Type::INIT;
    }
}
