#include "NormalBlock.h"

NormalBlock::NormalBlock(Camera* pCamera)
{
	object_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", pCamera);
}

NormalBlock::~NormalBlock()
{
}

void NormalBlock::Update(void)
{

	object_->Update();

}

void NormalBlock::Draw(void)
{

	object_->Draw();

}

void NormalBlock::SetIsFlow(bool flag)
{ 

	if (isConductor)
	{

		isFlow = flag;

	}

}