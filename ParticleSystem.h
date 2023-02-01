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

	//起動しているかどうかを変える
	void SetisActive(bool flag) { isActive = flag; };

	//パーティクルの発生間隔を変える
	void SetCoolTime(float time) { coolTime = time; };

	//重力に従いパーティクル(中でパラメータをセットしてupdateを呼んでくれる)
	void FountainParticle();

	//上に上がって消える
	void UpParticle();

	//渡したベクトルの方向に出す
	void testParticle(DirectX::XMFLOAT3 vec,DirectX::XMFLOAT2 xRandlen = { 1.0f,3.0f }, DirectX::XMFLOAT2 yRandlen = { 1.0f,3.0f }, DirectX::XMFLOAT2 zRandlen = { 1.0f,3.0f });

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

	//パーティクルの発生間隔
	float coolTime = 1;

	//cooltime用の時間計測
	float timeCount = 0;

	//パーティクルの移動スピード
	float moveSpeed = 0.5f;

	//パーティクルの生存時間
	float aliveTime = 10;

	bool isGravity = false;

	bool isActive = true;

};

