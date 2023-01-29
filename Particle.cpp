#include "Particle.h"

Particle::Particle(Camera* camera, DirectX::XMFLOAT3 StartPos, DirectX::XMFLOAT3 StartScale)
{

	Init(camera);

	startScale = StartScale;

	obj->worldCoordinate_.position_ = StartPos;
	obj->worldCoordinate_.scale_ = StartScale;

	start = DirectX::XMVectorSet(StartScale.x, StartScale.y, StartScale.z,0);

	obj->Update();
}

Particle::~Particle()
{
}

void Particle::Init(Camera* camera)
{
	obj= std::make_unique<Obj3d>();
	obj->SetCamera(camera);
}

void Particle::Update()
{

	DirectX::XMVECTOR lerpScale;

	lerpScale = DirectX::XMVectorLerp(start, { 0,0,0,0 }, aliveTime / maxAliveTime);

	obj->worldCoordinate_.position_ = { obj->worldCoordinate_.position_.x +moveVec.x*moveSpeed, obj->worldCoordinate_.position_.y + moveVec.y * moveSpeed - (isGravity*(gravity * aliveTime)) ,obj->worldCoordinate_.position_.z + moveVec.z * moveSpeed};

	obj->worldCoordinate_.scale_ = { lerpScale.m128_f32[0],lerpScale.m128_f32[1] ,lerpScale.m128_f32[2] };

	obj->Update();

	aliveTime++;

	if (maxAliveTime < aliveTime)
	{
		isAlive = false;
	}

}

void Particle::Draw()
{

	obj->Draw();

}