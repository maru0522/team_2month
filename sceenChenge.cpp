#include "sceenChenge.h"

SceenChenge::SceenChenge()
{

	ropeObj = std::make_unique<Obj3d>("Resources/3dModels/ropetest/ropetest.obj");
	playerObj = std::make_unique<Obj3d>("Resources/3dModels/playertest2/playertest2.obj");
	ropeObj->worldCoordinate_.scale_ = { 5,5,5 };
	ropeObj->worldCoordinate_.position_ = { -0.0f, -10.0f, -92.0f };
	ropeObj->worldCoordinate_.rotation_ = {0.0f,3.141592653589f/2,0.0f };
	
}

SceenChenge::~SceenChenge()
{
}

void SceenChenge::Update()
{
	ropeObj->Update();
	playerObj->Update();

	ImGui::Begin("pos");

	ImGui::SliderFloat("x", &ropeObj->worldCoordinate_.position_.x, -500.0f, 500.0f, "%.3f");
	ImGui::SliderFloat("y", &ropeObj->worldCoordinate_.position_.y, -500.0f, 500.0f, "%.3f");
	ImGui::SliderFloat("z", &ropeObj->worldCoordinate_.position_.z, -500.0f, 500.0f, "%.3f");

	ImGui::SliderFloat("playerx", &playerObj->worldCoordinate_.position_.x, -500.0f, 500.0f, "%.3f");
	ImGui::SliderFloat("playery", &playerObj->worldCoordinate_.position_.y, -500.0f, 500.0f, "%.3f");
	ImGui::SliderFloat("playerz", &playerObj->worldCoordinate_.position_.z, -500.0f, 500.0f, "%.3f");

	ImGui::End();
}

void SceenChenge::Draw()
{
	ropeObj->Draw();
	playerObj->Draw();
}