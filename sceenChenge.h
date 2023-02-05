#pragma once

#include "Obj3d.h"
#include "Sprite.h"

//”Ä—p“I‚ÉŽg‚¦‚é‚æ‚¤‚É‚Í‚µ‚Ä‚È‚¢
class SceenChenge
{
public:
	SceenChenge();
	~SceenChenge();

    void SetSprite(void);

	void Update();

	void Draw();

    bool GetIsStart(void) { return isStart; }
    bool GetIsEnd(void) { return isEnd; }
    bool GetIsResetOk(void) { return isResetOk_; }

	void SetIsStart(bool flag) { isStart = flag; };

    void Reset(void);

private:

	float easeOutBack(float start, float end, float time);

	DirectX::XMFLOAT2 Vec2EaseOutBack(DirectX::XMFLOAT2 start, DirectX::XMFLOAT2 end, float time);

	float lerp(const float& start, const float& end, const float t);

	float easeInBack(float start, float end, float time);

private:

	
	
    
	std::unique_ptr<Sprite> chenge;

	Camera* cameraPtr{ nullptr };

	float rotate = 0;
	DirectX::XMFLOAT2 scale = { 1,1 };

	float maxRopeMoveTime = 50;
	float maxPlayerMoveTime = 80;
	float maxChengeMoveTime = 50;

	float maxWait = 30;
	float waitTime = 0;

	float ropeMoveTime = 0;
	float playerMoveTime = 0;
	float ChengeMoveTime = 0;

	DirectX::XMFLOAT2 testPos = {1500,0};

	bool isEnd = false;

	bool isStart = false;

	bool checkDebug = false;

    bool isResetOk_{ false };
};

