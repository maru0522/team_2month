#include "ExtendBlock.h"

ExtendBlock::ExtendBlock(Camera* pCamera)
{

	object_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", pCamera);

}

ExtendBlock::~ExtendBlock()
{
}

void ExtendBlock::Init(DirectX::XMFLOAT3 StartScale)
{
	startScale = StartScale;
}

void ExtendBlock::Update(void)
{

	if (connectionBlock != nullptr)
	{

		isFlow = connectionBlock->GetIsFlow();

	}

	Extend();

	object_->Update();
	

}

void ExtendBlock::Draw(void)
{


	object_->Draw();


}

void ExtendBlock::Extend()
{

	if (isFlow)
	{

		if (nowScaleCount)
		{
			nowScaleCount = false;
			//timeCount = 0;
		}

		if (timeCount < maxTime)
		{
			timeCount+=extendSpeed;
		}

		if (timeCount < maxTime)
		{

			DirectX::XMFLOAT3 vec= lerp(startScale, { startScale.x + extendVec.x * extendLength, startScale.y + extendVec.y * extendLength,startScale.z + extendVec.z * extendLength }, timeCount / maxTime);

			//üŒ`•âŠÔ
			object_->worldCoordinate_.scale_ = vec;
			object_->worldCoordinate_.position_ = { object_->worldCoordinate_.position_.x + (extendVec.x * (extendSpeed / maxTime)), object_->worldCoordinate_.position_.y + (extendVec.y * (extendSpeed / maxTime)), object_->worldCoordinate_.position_.z + (extendVec.z * (extendSpeed / maxTime))};
			
		}
	}
	else
	{

		if (nowScaleCount==false)
		{
			nowScale = object_->worldCoordinate_.scale_;

			nowScaleCount = true;


		}

		

		if (timeCount > 0)
		{

			DirectX::XMFLOAT3 vec = lerp(startScale, { startScale.x + extendVec.x * extendLength, startScale.y + extendVec.y * extendLength,startScale.z + extendVec.z * extendLength }, timeCount / maxTime);

			//üŒ`•âŠÔ
			object_->worldCoordinate_.scale_ = vec;
			

		}
		if (timeCount > 0)
		{
			timeCount -= extendSpeed;
		}
		

		

	}

}

void ExtendBlock::RemoveConnectionBlock()
{

	connectionBlock = nullptr;
	isFlow = false;

}

const DirectX::XMFLOAT3 lerp(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const float t)
{

	DirectX::XMFLOAT3 ans = { start.x + t * (end.x - start.x),start.y + t * (end.y - start.y),start.z + t * (end.z - start.z) };

	return ans;

}