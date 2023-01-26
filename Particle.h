#pragma once
#include "Obj3d.h"

class Particle
{
public:
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="StartPos">発生位置を置く</param>
	/// <param name="StartScale">初期サイズを設定</param>
	Particle(Camera* camera, DirectX::XMFLOAT3 StartPos, DirectX::XMFLOAT3 StartScale);
	~Particle();

	void Update();

	void Draw();

	/// <summary>
	/// 生存時間を入れる
	/// </summary>
	void SetMaxAliveTime(float setTime) { maxAliveTime = setTime; };

	/// <summary>
	/// ここに入れるベクトルは正規化したものを入れる
	/// </summary>
	/// <param name="Vec">移動方向</param>
	void SetMoveVec(DirectX::XMFLOAT3 Vec) { moveVec = Vec; };

	/// <summary>
	/// 移動速度を入れる
	/// </summary>
	void SetMoveSpeed(float setSpeed) { moveSpeed = setSpeed; };

private:

	void Init(Camera* camera);

public:

	//本体
	std::unique_ptr<Obj3d> obj;


private:

	//生存時間
	float maxAliveTime = 100;
	float aliveTime = 0;

	//生存フラグ
	bool isAlive = true;

	//移動方向
	DirectX::XMFLOAT3 moveVec = { 0,1,0 };

	//移動スピード
	float moveSpeed = 2;

	//パーティクルの初期サイズ
	DirectX::XMFLOAT3 startScale = { 1,1,1 };

	//lerp用に用意した
	DirectX::XMVECTOR start;


};

