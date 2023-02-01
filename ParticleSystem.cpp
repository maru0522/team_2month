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
	if (timeCount >= coolTime)
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
			timeCount = 0;
		}
	}

	Update();

	timeCount++;

}

void ParticleSystem::UpParticle()
{

	if (timeCount >= coolTime)
	{

		if (isActive)
		{
			//���ʂȌv�Z�����Ȃ����߂�
			if (particles.size() < particleNum)
			{
				moveVec = { Util::Random<float>(-3,3),5,Util::Random<float>(-3,3) };

				// �m�����擾
				float length{ std::sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y + moveVec.z * moveVec.z) };
				// ���K��
				if (length != 0) {
					moveVec.x /= length;
					moveVec.y /= length;
					moveVec.z /= length;
				}

				isGravity = false;

				startScale = { 0.8f,0.8f,0.8f };

				aliveTime = Util::Random<float>(5, 20);
				moveSpeed = 1.0f;


			}

			popParticle();
			timeCount = 0;
		}

	}

	Update();
	timeCount++;

}

void ParticleSystem::testParticle(DirectX::XMFLOAT3 vec, DirectX::XMFLOAT2 xRandlen, DirectX::XMFLOAT2 yRandlen, DirectX::XMFLOAT2 zRandlen)
{

	if (timeCount >= coolTime)
	{

		if (isActive)
		{
			//���ʂȌv�Z�����Ȃ����߂�
			if (particles.size() < particleNum)
			{
				DirectX::XMFLOAT3 norVec = vec;
				// �m�����擾
				float vecLength{ std::sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z) };
				// ���K��
				if (vecLength != 0) {
					norVec.x /= vecLength;
					norVec.y /= vecLength;
					norVec.z /= vecLength;
				}

				moveVec = { norVec.x * Util::Random<float>(xRandlen.x,xRandlen.y),norVec.y * Util::Random<float>(yRandlen.x,yRandlen.y),norVec.z * Util::Random<float>(zRandlen.x,zRandlen.y) };

				// �m�����擾
				float length{ std::sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y + moveVec.z * moveVec.z) };
				// ���K��
				if (length != 0) {
					moveVec.x /= length;
					moveVec.y /= length;
					moveVec.z /= length;
				}

				isGravity = false;

				startScale = { 0.5f,0.5f,0.5f };


			}

			popParticle();
			timeCount = 0;
		}

	}

	Update();
	if (coolTime >= timeCount)
	{
		timeCount++;
	}

	ImGui::Begin("num");

	ImGui::Text("%d", particles.size());

	ImGui::End();

	ImGui::Begin("Particle");

	ImGui::InputInt("maxNum", &particleNum);
	ImGui::SliderFloat("cooltime", &coolTime, 0.0f, 50.0f, "%.3f");
	ImGui::SliderFloat("aliveTime", &aliveTime, 0.0f, 50.0f, "%.3f");

	ImGui::End();


}