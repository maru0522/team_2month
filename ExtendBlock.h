#pragma once
#include "Obj3d.h"
#include <memory>
#include "Camera.h"
#include "NormalBlock.h"

class ExtendBlock
{
public:
	ExtendBlock(Camera* pCamera);
	~ExtendBlock();

	//�J�n�ʒu���Z�b�g����
	void Init(DirectX::XMFLOAT3 StartScale);

	void Update(void);

	void Draw(void);

	//�L�т�
	void Extend();

	//�d�C������Ă��邩�ǂ�����flag��ύX����
	void SetIsflow(bool flag) { isFlow = flag; };

	//���̕�����ύX����
	void SetExtendVec(DirectX::XMFLOAT3 vec) { extendVec = vec; };

	//���̋�����ύX����
	void SetExtendSpeed(float speed) { extendSpeed = speed; };

	//���̓͂�������ύX����
	void SetExtendLength(float length) { extendLength = length; };

	//�ڑ�����
	void SetConnectionBlock(NormalBlock* setBlock) { connectionBlock = setBlock; }

	//�O��
	void RemoveConnectionBlock();

private:

public:

private:

	//�{��
	std::unique_ptr<Obj3d> object_{};

	//�ڑ���
	NormalBlock* connectionBlock = nullptr;

	//�d�C������Ă��邩
	bool isFlow = false;

	//�L�т����
	DirectX::XMFLOAT3 extendVec = { 0,1,0 };

	//�J�n�ʒu(���`��ԗp)
	DirectX::XMFLOAT3 startScale = {1,1,1};

	//�߂�p�̈ʒu(���`��ԗp)
	DirectX::XMFLOAT3 nowScale;

	//�߂�Ƃ��̃X�^�[�g�ʒu���擾���邽�߂̃t���O
	bool nowScaleCount = false;

	//�L�т鑬��
	float extendSpeed = 0.1f;

	//�L�т钷��
	float extendLength = 5;

	float timeCount = 0;

	float maxTime = 20.0f;


};

const DirectX::XMFLOAT3 lerp(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const float t);

