#include "Stage.h"
#include <fstream>
#include <sstream>
#include <string>
#include "BlockManager.h"
#include "NormalBlock.h"
#include "StartBlock.h"
#include "Hook.h"
#include "PowerSupplyBlock.h"
#include "PowerReceiveBlock.h"
#include "SwitchBlock.h"

void Stage::LoadCsv(Camera* pCamera, const fsPath& path)
{
    std::ifstream ifs{ path };

    std::string line, tmp;

    std::array<float, 3> coordinate{};
    std::array<float, 3> scale{};
    IBlock::Type blockType{ IBlock::Type::INIT };
    uint32_t idXHook{};
    uint32_t idxConnect{};

    while (std::getline(ifs, line)) {
        size_t loopX{};

        std::istringstream line_stream{ line };

        while (std::getline(line_stream, tmp, ',')) {
            if (loopX <= 2) {
                coordinate[loopX] = std::stof(tmp);
            }
            else if (3 <= loopX && loopX <= 5) {
                scale[loopX - 3] = std::stof(tmp);
            }
            else if (loopX == 6) {
                blockType = static_cast<IBlock::Type>(std::stoi(tmp));
                if (blockType == IBlock::Type::HOOK) {
                    idXHook++;
                }
            }
            else if (loopX == 7) { // POWERRECEIVE�ȍ~
                if (static_cast<int>(blockType) >= static_cast<int>(IBlock::Type::POWERRECEIVE)) {
                    idxConnect = std::stoi(tmp);
                }
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
            BlockManager::Register(new NormalBlock{ {coordinate.at(0),coordinate.at(1),coordinate.at(2)}, {scale.at(0),scale.at(1),scale.at(2)}, pCamera});
            break;
        case IBlock::Type::START:
            BlockManager::Register(new StartBlock{ {coordinate.at(0),coordinate.at(1),coordinate.at(2)}, {scale.at(0),scale.at(1),scale.at(2)}, pCamera });
            break;
        case IBlock::Type::HOOK:
            BlockManager::Register(new Hook{ {coordinate.at(0),coordinate.at(1),coordinate.at(2)}, {scale.at(0),scale.at(1),scale.at(2)}, idXHook, pCamera });
            break;
        case IBlock::Type::POWERSUPPLY:
            BlockManager::Register(new PowerSupplyBlock{ {coordinate.at(0),coordinate.at(1),coordinate.at(2)}, {scale.at(0),scale.at(1),scale.at(2)}, pCamera });
            break;
        case IBlock::Type::POWERRECEIVE:
            BlockManager::Register(new PowerReceiveBlock{ {coordinate.at(0),coordinate.at(1),coordinate.at(2)}, {scale.at(0),scale.at(1),scale.at(2)}, idxConnect, pCamera });
            break;
        case IBlock::Type::SWITCH:
            BlockManager::Register(new SwitchBlock{ {coordinate.at(0),coordinate.at(1),coordinate.at(2)}, {scale.at(0),scale.at(1),scale.at(2)}, idxConnect, pCamera });
            break;
        }

        coordinate.fill(0.f);
        blockType = IBlock::Type::INIT;
    }
}
