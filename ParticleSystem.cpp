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
	//生成限界数を設定
	if (particles.size() < particleNum)
	{

		std::unique_ptr<Particle> newParticle = std::make_unique<Particle>(camera, worldCoordinate.position_, startScale);

		newParticle->SetMaxAliveTime(aliveTime);
		newParticle->SetMoveVec(moveVec);
		newParticle->SetMoveSpeed(moveSpeed);
		newParticle->SetIsGravity(isGravity);

		//敵キャラを登録
		particles.push_back(std::move(newParticle));
	}

}

void ParticleSystem::FountainParticle()
{
	if (timeCount >= coolTime)
	{
		if (isActive)
		{
			//無駄な計算をしないために
			if (particles.size() < particleNum)
			{
				moveVec = { Util::Random<float>(-5,5),0,Util::Random<float>(-5,5) };

				// ノルム取得
				float length{ std::sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y + moveVec.z * moveVec.z) };
				// 正規化
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
			//無駄な計算をしないために
			if (particles.size() < particleNum)
			{
				moveVec = { Util::Random<float>(-3,3),5,Util::Random<float>(-3,3) };

				// ノルム取得
				float length{ std::sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y + moveVec.z * moveVec.z) };
				// 正規化
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
			//無駄な計算をしないために
			if (particles.size() < particleNum)
			{
				DirectX::XMFLOAT3 norVec = vec;
				// ノルム取得
				float vecLength{ std::sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z) };
				// 正規化
				if (vecLength != 0) {
					norVec.x /= vecLength;
					norVec.y /= vecLength;
					norVec.z /= vecLength;
				}

				moveVec = { norVec.x * Util::Random<float>(xRandlen.x,xRandlen.y),norVec.y * Util::Random<float>(yRandlen.x,yRandlen.y),norVec.z * Util::Random<float>(zRandlen.x,zRandlen.y) };

				// ノルム取得
				float length{ std::sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y + moveVec.z * moveVec.z) };
				// 正規化
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


}