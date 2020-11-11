#pragma once
#include "gameObject/ysGameObject.h"
#include "physics/PhysicsStaticObject.h"
class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	void Update();
	void Draw();
private:
	SkinModel m_model;										//�X�L�����f���B
	CVector3 m_position = CVector3{ 0.0f,-100.0f,0.0f };	//�w�i�̈ʒu
	CQuaternion m_rotation = CQuaternion::Identity();		//�w�i�̉�]�p�x
	CVector3 m_scale = CVector3::One();						//�w�i�̑傫��
	PhysicsStaticObject m_physicsStaticObject;				//�ÓI�I�u�W�F�N�g�B���b�V����ݒ肷��
	ID3D11ShaderResourceView* m_earthNormalMapSRV;			//!<�y�@���}�b�v��SRV�B
	ID3D11ShaderResourceView* m_eatthSpecularMapSRV;		//!<�y�X�y�L�����}�b�v��SRV�B
	ID3D11ShaderResourceView* m_grassNormalMapSRV;			//!<���@���}�b�v��SRV�B
	ID3D11ShaderResourceView* m_grassSpecularMapSRV;		//!<���X�y�L�����}�b�v��SRV�B
};

