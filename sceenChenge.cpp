#include "sceenChenge.h"

SceenChenge::SceenChenge()
{

	/*ropeObj = std::make_unique<Obj3d>("Resources/3dModels/ropetest/ropetest.obj");
	playerObj = std::make_unique<Obj3d>("Resources/3dModels/playertest2/playertest2.obj");
	ropeObj->worldCoordinate_.scale_ = { 5,5,5 };
	ropeObj->worldCoordinate_.position_ = { -0.0f, -10.0f, -92.0f };
	ropeObj->worldCoordinate_.rotation_ = {0.0f,3.141592653589f/2,0.0f };

	playerObj->worldCoordinate_.position_ = { -0.0f, -10.0f, -92.0f };
	playerObj->worldCoordinate_.rotation_ = { 0.0f,3.141592653589f / 5.0f,0.0f };*/
	Texture::Load("Resources/Image/testcheng.png");
	Texture::Load("Resources/Image/cheng.png");
	//Texture::Load("Resources/Image/testplayercheng.png");
	Texture::Load("Resources/Image/testplayercheng2_R.png");
	Texture::Load("Resources/Image/testplayercheng2_L.png");
	Texture::Load("Resources/Image/testropecheng.png");

	chenge = std::make_unique<Sprite>("Resources/Image/cheng.png", CMode::PATH);

}

SceenChenge::~SceenChenge()
{
}

void SceenChenge::SetSprite(void)
{
	chenge = std::make_unique<Sprite>("Resources/Image/cheng.png", CMode::PATH);
}

void SceenChenge::Update()
{
	//ropeObj->Update();
	//playerObj->Update();

	chenge->SetPosition({ 630,360 });
	chenge->SetAnchorPoint({ 0.5f,0.5f });

#ifdef _DEBUG

	ImGui::Begin("pos");

	

	ImGui::InputFloat("maxRopeMoveTime", &maxRopeMoveTime, 1.0f, 5.0f, "%.0f");
	ImGui::InputFloat("maxPlayerMoveTime", &maxPlayerMoveTime, 1.0f, 5.0f, "%.0f");

	ImGui::InputFloat("maxChengeMoveTime", &maxChengeMoveTime, 1.0f, 5.0f, "%.0f");
	ImGui::InputFloat("maxWait", &maxWait, 1.0f, 5.0f, "%.0f");

	ImGui::Checkbox("debugDraw", &checkDebug);

	ImGui::End();

#endif // DEBUG
	if (isStart)
	{

		if (isEnd == false)
		{
			rotate = lerp(0, 1000, playerMoveTime / maxPlayerMoveTime);
			scale.x = lerp(1, 3000, playerMoveTime / maxPlayerMoveTime);
			scale.y = lerp(1, 3000, playerMoveTime / maxPlayerMoveTime);
		}
		else
		{
			rotate = lerp(1000, 0, ChengeMoveTime / maxChengeMoveTime);
			scale.x = lerp(3000, 0, ChengeMoveTime / maxChengeMoveTime);
			scale.y = lerp(3000, 0, ChengeMoveTime / maxChengeMoveTime);
		}

		if (maxPlayerMoveTime <= playerMoveTime and isEnd == false)
		{
			if (waitTime >= maxWait)
			{
				isEnd = true;
				ropeMoveTime = 0;
				playerMoveTime = 0;

				
			}

			if (maxWait > waitTime)
			{
				waitTime++;
			}

		}

		if (maxChengeMoveTime <= ChengeMoveTime)
		{

            isResetOk_ = true;

		}

		chenge->SetRotation(rotate);
		chenge->SetSize(scale);

		if (maxRopeMoveTime > ropeMoveTime and isEnd == false)
		{
			ropeMoveTime++;
		}

		if (maxRopeMoveTime <= ropeMoveTime and maxPlayerMoveTime > playerMoveTime and isEnd == false)
		{
			playerMoveTime++;
		}

		if (isEnd and maxChengeMoveTime > ChengeMoveTime)
		{

			ChengeMoveTime++;

		}

		chenge->Update();

	}

#ifdef _DEBUG


#endif // _DEBUG


	

}

void SceenChenge::Draw()
{
	//ropeObj->Draw();
	//playerObj->Draw();

	if (isStart)
	{
		chenge->Draw();
		
	}
#ifdef _DEBUG

	if (checkDebug)
	{
		
	}

#endif // _DEBUG


}

void SceenChenge::Reset(void)
{
    isEnd = false;
    ChengeMoveTime = 0;
    waitTime = 0;
    isStart = false;
    isResetOk_ = false;
}

float SceenChenge::easeOutBack(float start, float end, float time)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return start + (1 + c3 * powf(time - 1, 3) + c1 * powf(time - 1, 2)) * (end - start);
}

DirectX::XMFLOAT2 SceenChenge::Vec2EaseOutBack(DirectX::XMFLOAT2 start, DirectX::XMFLOAT2 end, float time)
{

	DirectX::XMFLOAT2 ans;

	ans.x = easeOutBack(start.x, end.x, time);
	ans.y = easeOutBack(start.y, end.y, time);
	

	return ans;

}

float SceenChenge::lerp(const float& start, const float& end, const float t)
{
	return start * (1.0f - t) + end * t;
}

float SceenChenge::easeInBack(float start, float end, float time)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return start + (c3 * time * time * time - c1 * time * time) * (end - start);
}
