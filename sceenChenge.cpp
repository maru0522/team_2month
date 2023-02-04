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
	//Texture::Load("Resources/Image/testplayercheng.png");
	Texture::Load("Resources/Image/testplayercheng2_R.png");
	Texture::Load("Resources/Image/testplayercheng2_L.png");
	Texture::Load("Resources/Image/testropecheng.png");

	playerR = std::make_unique<Sprite>("Resources/Image/testplayercheng2_R.png", CMode::PATH);
	playerL = std::make_unique<Sprite>("Resources/Image/testplayercheng2_L.png", CMode::PATH);
	rope = std::make_unique<Sprite>("Resources/Image/testropecheng.png", CMode::PATH);
	rope2 = std::make_unique<Sprite>("Resources/Image/testropecheng.png", CMode::PATH);
	rope3 = std::make_unique<Sprite>("Resources/Image/testropecheng.png", CMode::PATH);

	chenge = std::make_unique<Sprite>("Resources/Image/testcheng.png", CMode::PATH);
	
	//player->SetPosition(playerpos);
	playerR->SetPosition(playerpos);
	playerL->SetPosition(playerpos);
	rope->SetPosition(ropepos);
	rope2->SetPosition(ropepos2);
	rope3->SetPosition(ropepos3);
	chenge->SetPosition(chengepos);

}

SceenChenge::~SceenChenge()
{
}

void SceenChenge::SetSprite(void)
{
    //player = std::make_unique<Sprite>("Resources/Image/testplayercheng.png", CMode::PATH);
    playerR = std::make_unique<Sprite>("Resources/Image/testplayercheng2_R.png", CMode::PATH);
    playerL = std::make_unique<Sprite>("Resources/Image/testplayercheng2_L.png", CMode::PATH);
    rope = std::make_unique<Sprite>("Resources/Image/testropecheng.png", CMode::PATH);
    rope2 = std::make_unique<Sprite>("Resources/Image/testropecheng.png", CMode::PATH);
    rope3 = std::make_unique<Sprite>("Resources/Image/testropecheng.png", CMode::PATH);
    chenge = std::make_unique<Sprite>("Resources/Image/testcheng.png", CMode::PATH);
}

void SceenChenge::Update()
{
	//ropeObj->Update();
	//playerObj->Update();
	
	rope2->SetRotation(3.141592653589f / 3.0f);
	rope3->SetRotation(-(3.141592653589f / 3.0f));

#ifdef _DEBUG

	ImGui::Begin("pos");

	ImGui::SliderFloat("x", &ropepos.x, 0.0f, 1280.0f, "%.3f");
	ImGui::SliderFloat("y", &ropepos.y, -200.0f, 720.0f, "%.3f");

	ImGui::SliderFloat("2x", &ropepos2.x, 0.0f, 1280.0f, "%.3f");
	ImGui::SliderFloat("2y", &ropepos2.y, -200.0f, 720.0f, "%.3f");

	ImGui::SliderFloat("3x", &ropepos3.x, 0.0f, 1280.0f, "%.3f");
	ImGui::SliderFloat("3y", &ropepos3.y, -200.0f, 720.0f, "%.3f");
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
			ropepos2 = Vec2EaseOutBack({ 510,720 }, { 510,-190 }, ropeMoveTime / maxRopeMoveTime);
			ropepos3 = Vec2EaseOutBack({ 510,720 }, { 510,-190 }, ropeMoveTime / maxRopeMoveTime);
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

				ropepos = { 510,720 };
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

		//player->SetPosition(playerpos);
		playerR->SetPosition(playerpos);
		playerL->SetPosition(playerpos);
		rope->SetPosition(ropepos);
		rope2->SetPosition(ropepos2);
		rope3->SetPosition(ropepos3);
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

	//player->SetPosition(playerpos);
	playerR->SetPosition(playerpos);
	playerL->SetPosition(playerpos);
	rope->SetPosition(ropepos);
	chenge->SetPosition(chengepos);
#endif // _DEBUG


	//player->Update();
	playerR->Update();
	playerL->Update();
	rope->Update();
	rope2->Update();
	rope3->Update();
	chenge->Update();

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
			rope2->Draw();
			rope3->Draw();
			playerL->Draw();
		}
		chenge->Draw();
	}
#ifdef _DEBUG

	if (checkDebug)
	{
		playerR->Draw();
		rope->Draw();
		rope2->Draw();
		rope3->Draw();
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
