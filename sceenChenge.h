#pragma once

#include "Obj3d.h"

//汎用的に使えるようにはしてない
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

