#include "sceenChenge.h"

SceenChenge::SceenChenge()
{
	Texture::Load("Resources/Image/cheng.png");

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
			//çLÇ™ÇÈìÆçÏ
			rotate = lerp(0, 1000, ropeMoveTime / maxRopeMoveTime);
			scale.x = lerp(1, 2000, ropeMoveTime / maxRopeMoveTime);
			scale.y = lerp(1, 2000, ropeMoveTime / maxRopeMoveTime);
		}
		else
		{
			//èkÇﬁìÆçÏ
			rotate = lerp(1000, 0, ChengeMoveTime / maxChengeMoveTime);
			scale.x = lerp(2000, 0, ChengeMoveTime / maxChengeMoveTime);
			scale.y = lerp(2000, 0, ChengeMoveTime / maxChengeMoveTime);
		}

		if (maxRopeMoveTime <= ropeMoveTime and isEnd == false)
		{
			//âÊñ ëSëÃÇ…è≠ÇµÇ¢ÇÈÇΩÇﬂÇ…ïKóv
			if (waitTime >= maxWait)
			{
				isEnd = true;
				ropeMoveTime = 0;
				
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

		if (isEnd and maxChengeMoveTime > ChengeMoveTime)
		{

			ChengeMoveTime++;

		}

		chenge->Update();

	}

	chenge->Update();

}

void SceenChenge::Draw()
{

	if (isStart)
	{
		chenge->Draw();
		
	}

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
