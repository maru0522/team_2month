#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Init(Camera* setCamera)
{
	camera = setCamera;
}

void ParticleSystem::Update()
{

	worldCoordinate.Update();

	particles.remove_if([](std::unique_ptr<Particle>& particle)
	{
			return !particle->GetIsAlive();
	});

	for (std::unique_ptr<Particle>& particle : particles)
	{
		particle->Update();
	}

}

void ParticleSystem::Draw()
{

	for (std::unique_ptr<Particle>& particle : particles)
	{
		particle->Draw();
	}

}

void ParticleSystem::popParticle()
{
	//�������E����ݒ�
	if (particles.size() < particleNum)
	{

		std::unique_ptr<Particle> newParticle = std::make_unique<Particle>(camera, worldCoordinate.position_, startScale);

		newParticle->SetMaxAliveTime(aliveTime);
		newParticle->SetMoveVec(moveVec);
		newParticle->SetMoveSpeed(moveSpeed);
		newParticle->SetIsGravity(isGravity);

		//�G�L������o�^
		particles.push_back(std::move(newParticle));
	}

}

void ParticleSystem::FountainParticle()
{

	if (isActive)
	{
		//���ʂȌv�Z�����Ȃ����߂�
		if (particles.size() < particleNum)
		{
			moveVec = { Util::Random<float>(-5,5),0,Util::Random<float>(-5,5) };

			// �m�����擾
			float length{ std::sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y + moveVec.z * moveVec.z) };
			// ���K��
			if (length != 0) {
				moveVec.x /= length;
				moveVec.y /= length;
				moveVec.z /= length;
			}

			isGravity = true;


		}

		popParticle();
	}

	Update();

}