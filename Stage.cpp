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
#include "FanBlock.h"
#include "GoalBlock.h"

DirectX::XMFLOAT3 Stage::maxBlockPosValue_{};

void Stage::LoadCsv(Camera* pCamera, const fsPath& path)
{
    std::ifstream ifs{ path };

    std::string line, tmp;

    std::array<float, 3> coordinate{};
    std::array<float, 3> scale{};
    IBlock::Type blockType{ IBlock::Type::INIT };
    uint32_t idXHook{};
    uint32_t idXSupply{};
    uint32_t idXReceive{};
    uint32_t idxConnect{};

    maxBlockPosValue_ = {0,0,0};

    while (std::getline(ifs, line)) {
        size_t loopX{};

        std::istringstream line_stream{ line };

        while (std::getline(line_stream, tmp, ',')) {
            if (loopX <= 2) {
                coordinate[loopX] = std::stof(tmp);
                if (coordinate[0] > maxBlockPosValue_.x) {
                    maxBlockPosValue_.x = coordinate[0];
                }
                if (coordinate[2] > maxBlockPosValue_.z) {
                    maxBlockPosValue_.z = coordinate[2];
                }
            }
            else if (3 <= loopX && loopX <= 5) {
                scale[loopX - 3] = std::stof(tmp);
            }
            else if (loopX == 6) {
                blockType = static_cast<IBlock::Type>(std::stoi(tmp));
                if (blockType == IBlock::Type::HOOK) {
                    idXHook++;
                }
                if (blockType == IBlock::Type::POWERSUPPLY) {
                    idXSupply++;
                }
                if (blockType == IBlock::Type::POWERRECEIVE) {
                    idXReceive++;
                }
            }
            else if (loopX == 7) { // POWERRECEIVEà»ç~
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
        case IBlock::Type::GOAL:
            BlockManager::Register(new GoalBlock{ {coordinate.at(0),coordinate.at(1),coordinate.at(2)}, {scale.at(0),scale.at(1),scale.at(2)}, pCamera });
            break;
        case IBlock::Type::HOOK:
            BlockManager::Register(new Hook{ {coordinate.at(0),coordinate.at(1),coordinate.at(2)}, {scale.at(0),scale.at(1),scale.at(2)}, idXHook, pCamera });
            break;
        case IBlock::Type::POWERSUPPLY:
            BlockManager::Register(new PowerSupplyBlock{ {coordinate.at(0),coordinate.at(1),coordinate.at(2)}, {scale.at(0),scale.at(1),scale.at(2)}, idXSupply, pCamera });
            break;
        case IBlock::Type::POWERRECEIVE:
            BlockManager::Register(new PowerReceiveBlock{ {coordinate.at(0),coordinate.at(1),coordinate.at(2)}, {scale.at(0),scale.at(1),scale.at(2)}, idxConnect,idXReceive, pCamera });
            break;
        case IBlock::Type::SWITCH:
            BlockManager::Register(new SwitchBlock{ {coordinate.at(0),coordinate.at(1),coordinate.at(2)}, {scale.at(0),scale.at(1),scale.at(2)}, idxConnect, pCamera });
            break;
        case IBlock::Type::FAN:
            BlockManager::Register(new FanBlock{ {coordinate.at(0),coordinate.at(1),coordinate.at(2)}, {scale.at(0),scale.at(1),scale.at(2)}, idxConnect, pCamera });
            break;
        }

        coordinate.fill(0.f);
        blockType = IBlock::Type::INIT;
    }
}
