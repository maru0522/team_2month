#pragma once

#include "Obj3d.h"
#include "Sprite.h"

//”Ä—p“I‚ÉŽg‚¦‚é‚æ‚¤‚É‚Í‚µ‚Ä‚È‚¢
class SceenChenge
{
public:
	SceenChenge();
	~SceenChenge();

	void Update();

	void Draw();

	void SetIsstart(bool flag) { isStart = flag; };

private:

	float easeOutBack(float start, float end, float time);

	DirectX::XMFLOAT2 Vec2EaseOutBack(DirectX::XMFLOAT2 start, DirectX::XMFLOAT2 end, float time);

	float lerp(const float& start, const float& end, const float t);

	float easeInBack(float start, float end, float time);

private:

	std::unique_ptr<Obj3d> ropeObj;
	std::unique_ptr<Obj3d> playerObj;

	std::unique_ptr<Sprite> player{ std::make_unique<Sprite>("Resources/Image/testplayercheng.png",CMode::PATH) };
	std::unique_ptr<Sprite> playerR{ std::make_unique<Sprite>("Resources/Image/testplayercheng2_R.png",CMode::PATH) };
	std::unique_ptr<Sprite> playerL{ std::make_unique<Sprite>("Resources/Image/testplayercheng2_L.png",CMode::PATH) };

	std::unique_ptr<Sprite> rope{ std::make_unique<Sprite>("Resources/Image/testropecheng.png",CMode::PATH) };
	

	std::unique_ptr<Sprite> chenge{ std::make_unique<Sprite>("Resources/Image/testcheng.png",CMode::PATH) };

	Camera* cameraPtr{ nullptr };

	DirectX::XMFLOAT2 playerpos = {500,720 };
	DirectX::XMFLOAT2 ropepos = { 500 ,720 };
	DirectX::XMFLOAT2 chengepos = {0,720};

	float maxRopeMoveTime = 50;
	float maxPlayerMoveTime = 80;
	float maxChengeMoveTime = 50;

	float maxWait = 30;
	float waitTime = 0;

	float ropeMoveTime = 0;
	float playerMoveTime = 0;
	float ChengeMoveTime = 0;

	bool isEnd = false;

	bool isStart = false;

	bool checkDebug = false;
	

};

