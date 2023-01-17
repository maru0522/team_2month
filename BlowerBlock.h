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

	//�d�C������Ă��邩�ǂ�����flag��ύX����
	void SetIsflow(bool flag) { isFlow = flag; };

	//���̕�����ύX����
	void SetWindVec(DirectX::XMFLOAT3 vec) { windVec = vec; };

	//���̋�����ύX����
	void SetWindSpeed(float speed) { windSpeed = speed; };

	//���̓͂�������ύX����
	void SetWindLength(float length) { windLength = length; };

	//�ڑ�����
	void SetConnectionBlock(NormalBlock* setBlock) { connectionBlock = setBlock; }

	//�O��
	void RemoveConnectionBlock();

private:

public:

private:

	//�{��
	std::unique_ptr<Obj3d> object_{};
	//���̔���pobj
	std::unique_ptr<Obj3d> windObject_{};

	//�ڑ���
	NormalBlock* connectionBlock = nullptr;

	//�d�C������Ă��邩
	bool isFlow = false;

	//���̕���(�΂߂ɂł��邪���肪�������̂ł��ׂ��ł͂Ȃ�)
	DirectX::XMFLOAT3 windVec = { 0,1,0 };

	//���̋���
	float windSpeed = 1;

	//���̓͂�����
	float windLength = 2;

};

