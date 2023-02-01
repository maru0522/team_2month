#pragma once

#include "Obj3d.h"

//�ėp�I�Ɏg����悤�ɂ͂��ĂȂ�
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

