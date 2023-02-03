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
	
	player->SetPosition(playerpos);
	playerR->SetPosition(playerpos);
	playerL->SetPosition(playerpos);
	rope->SetPosition(ropepos);
	chenge->SetPosition(chengepos);

}

SceenChenge::~SceenChenge()
{
}

void SceenChenge::SetSprite(void)
{
    player = std::make_unique<Sprite>("Resources/Image/testplayercheng.png", CMode::PATH);
    playerR = std::make_unique<Sprite>("Resources/Image/testplayercheng2_R.png", CMode::PATH);
    playerL = std::make_unique<Sprite>("Resources/Image/testplayercheng2_L.png", CMode::PATH);
    rope = std::make_unique<Sprite>("Resources/Image/testropecheng.png", CMode::PATH);
    chenge = std::make_unique<Sprite>("Resources/Image/testcheng.png", CMode::PATH);
}

void SceenChenge::Update()
{
	//ropeObj->Update();
	//playerObj->Update();

	player->Update();
	playerR->Update();
	playerL->Update();
	rope->Update();
	chenge->Update();

#ifdef _DEBUG

	ImGui::Begin("pos");

	ImGui::SliderFloat("x", &ropepos.x, 0.0f, 1280.0f, "%.3f");
	ImGui::SliderFloat("y", &ropepos.y, -200.0f, 720.0f, "%.3f");
	//ImGui::SliderFloat("z", &ropeObj->worldCoordinate_.position_.z, -500.0f, 500.0f, "%.3f");

	ImGui::SliderFloat("playerx", &playerpos.x, 0.0f, 1280.0f, "%.3f");
	ImGui::SliderFloat("playery", &playerpos.y, -500.0f, 720.0f, "%.3f");
	//ImGui::SliderFloat("playerz", &playerObj->worldCoordinate_.position_.z, -500.0f, 500.0f, "%.3f");

	ImGui::SliderFloat("chengeX", &chengepos.x, 0.0f, 1280.0f, "%.3f");
	ImGui::SliderFloat("chengeY", &chengepos.y, -720.0f, 720.0f, "%.3f");

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
			ropepos = Vec2EaseOutBack({ 510,720 }, { 510,-190 }, ropeMoveTime / maxRopeMoveTime);
			playerpos.y = lerp(720, -400, playerMoveTime / maxPlayerMoveTime);
			chengepos.y = lerp(1120, 0, playerMoveTime / maxPlayerMoveTime);
		}
		else
		{
			chengepos.y = lerp(0, -720, ChengeMoveTime / maxChengeMoveTime);
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

		player->SetPosition(playerpos);
		playerR->SetPosition(playerpos);
		playerL->SetPosition(playerpos);
		rope->SetPosition(ropepos);
		chenge->SetPosition(chengepos);

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



	}

#ifdef _DEBUG

	player->SetPosition(playerpos);
	playerR->SetPosition(playerpos);
	playerL->SetPosition(playerpos);
	rope->SetPosition(ropepos);
	chenge->SetPosition(chengepos);
#endif // _DEBUG


}

void SceenChenge::Draw()
{
	//ropeObj->Draw();
	//playerObj->Draw();

	if (isStart)
	{
		if (isEnd == false)
		{
			//player->Draw();
			playerR->Draw();
			rope->Draw();
			playerL->Draw();
		}
		chenge->Draw();
	}
#ifdef _DEBUG

	if (checkDebug)
	{
		playerR->Draw();
		rope->Draw();
		playerL->Draw();
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