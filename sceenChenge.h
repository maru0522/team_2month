#pragma once

#include "Obj3d.h"

//”Ä—p“I‚ÉŽg‚¦‚é‚æ‚¤‚É‚Í‚µ‚Ä‚È‚¢
class SceenChenge
{
public:
	SceenChenge();
	~SceenChenge();

	void Update();

	void Draw();

private:

	std::unique_ptr<Obj3d> ropeObj;
	std::unique_ptr<Obj3d> playerObj;

	Camera* cameraPtr{ nullptr };

	

};

