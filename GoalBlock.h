#include "Obj3d.h"
#include "Camera.h"
#include <memory>
#include "IBlock.h"
#include <DirectXMath.h>

class GoalBlock : public IBlock
{
public: // �֐�
    GoalBlock(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, Camera* pCamera);

    void Update(void) override;
    void Draw(void) override;
};