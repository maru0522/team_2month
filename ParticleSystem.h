#pragma once
#include "Particle.h"
#include "WorldCoordinate.h"

//本体を持っているパーティクルの動きを持つクラス
class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void Init(Camera* setCamera);

	

	void Draw();

	/// <summary>
	/// パーティクル生成するときの初期サイズを設定
	/// </summary>
	/// <param name="Vec">サイズ</param>
	void SetParticleScale(DirectX::XMFLOAT3 scale) { startScale = scale; };

	//パーティクル限界生成数
	void SetParticleNum(int setNum) { particleNum = setNum; };

	/// <summary>
	/// 生存時間を入れる
	/// </summary>
	void SetAliveTime(float setTime) { aliveTime = setTime; };

	//噴水のように飛び出すパーティクル(パラメータをセットしてupdateを呼ぶ)
	void FountainParticle();

private:

	void Update();

	//パーティクルを生成
	void popParticle();

public:

	//出す位置
	WorldCoordinate worldCoordinate;

private:

	Camera* camera;

	

	//たくさん持つための物
	std::list<std::unique_ptr<Particle>> particles;

	//パーティクル生成数
	int particleNum = 10;

	//パーティクルの初期サイズ
	DirectX::XMFLOAT3 startScale = { 1,1,1 };

	//パーティクルの移動方向
	DirectX::XMFLOAT3 moveVec = { 0,1,0 };

	//パーティクルの移動スピード
	float moveSpeed = 2;

	//パーティクルの生存時間
	float aliveTime = 10;

	bool isGravity = false;

};

