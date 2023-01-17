#pragma once

#include "Obj3d.h"
#include <memory>
#include "Camera.h"
#include "NormalBlock.h"

class BlowerBlock
{
public:

	BlowerBlock(Camera* pCamera);

	void Update(void);

	void Draw(void);

	//電気が流れているかどうかのflagを変更する
	void SetIsflow(bool flag) { isFlow = flag; };

	//風の方向を変更する
	void SetWindVec(DirectX::XMFLOAT3 vec) { windVec = vec; };

	//風の強さを変更する
	void SetWindSpeed(float speed) { windSpeed = speed; };

	//風の届く長さを変更する
	void SetWindLength(float length) { windLength = length; };

	//接続する
	void SetConnectionBlock(NormalBlock* setBlock) { connectionBlock = setBlock; }

	//外す
	void RemoveConnectionBlock();

private:

public:

private:

	//本体
	std::unique_ptr<Obj3d> object_{};
	//風の判定用obj
	std::unique_ptr<Obj3d> windObject_{};

	//接続先
	NormalBlock* connectionBlock = nullptr;

	//電気が流れているか
	bool isFlow = false;

	//風の方向(斜めにできるが判定が厳しいのでやるべきではない)
	DirectX::XMFLOAT3 windVec = { 0,1,0 };

	//風の強さ
	float windSpeed = 1;

	//風の届く長さ
	float windLength = 2;

};

