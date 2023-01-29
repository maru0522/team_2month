#pragma once
#include "Particle.h"
#include "WorldCoordinate.h"

//�{�̂������Ă���p�[�e�B�N���̓��������N���X
class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void Init(Camera* setCamera);

	

	void Draw();

	/// <summary>
	/// �p�[�e�B�N����������Ƃ��̏����T�C�Y��ݒ�
	/// </summary>
	/// <param name="Vec">�T�C�Y</param>
	void SetParticleScale(DirectX::XMFLOAT3 scale) { startScale = scale; };

	//�p�[�e�B�N�����E������
	void SetParticleNum(int setNum) { particleNum = setNum; };

	/// <summary>
	/// �������Ԃ�����
	/// </summary>
	void SetAliveTime(float setTime) { aliveTime = setTime; };

	//�����̂悤�ɔ�яo���p�[�e�B�N��(�p�����[�^���Z�b�g����update���Ă�)
	void FountainParticle();

private:

	void Update();

	//�p�[�e�B�N���𐶐�
	void popParticle();

public:

	//�o���ʒu
	WorldCoordinate worldCoordinate;

private:

	Camera* camera;

	

	//�������񎝂��߂̕�
	std::list<std::unique_ptr<Particle>> particles;

	//�p�[�e�B�N��������
	int particleNum = 10;

	//�p�[�e�B�N���̏����T�C�Y
	DirectX::XMFLOAT3 startScale = { 1,1,1 };

	//�p�[�e�B�N���̈ړ�����
	DirectX::XMFLOAT3 moveVec = { 0,1,0 };

	//�p�[�e�B�N���̈ړ��X�s�[�h
	float moveSpeed = 2;

	//�p�[�e�B�N���̐�������
	float aliveTime = 10;

	bool isGravity = false;

};

