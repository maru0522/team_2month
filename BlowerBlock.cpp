#include "BlowerBlock.h"


BlowerBlock::BlowerBlock(Camera* pCamera)
{
	object_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", pCamera);
	windObject_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", pCamera);
}


void BlowerBlock::Update(void)
{

	if (connectionBlock != nullptr)
	{

		isFlow = connectionBlock->GetIsFlow();

	}

	object_->Update();
	windObject_->Update();

}

void BlowerBlock::Draw(void)
{


	object_->Draw();


}

void BlowerBlock::RemoveConnectionBlock()
{ 

	connectionBlock = nullptr; 
	isFlow = false; 

}