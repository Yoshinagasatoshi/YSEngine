#pragma once



class ICollider;

//���̏��B
struct RigidBodyInfo {
	RigidBodyInfo() :
		collider(NULL),
		mass(0.0f)
	{
		pos = CVector3::Zero();
		rot = CQuaternion::Identity();
		localInteria = CVector3::Zero();
	}
	CVector3 pos;			//���W�B
	CQuaternion rot;	//��]�B
	CVector3 localInteria;						//�����e���\��
	ICollider* collider;				//�R���C�_�[�B
	float mass = 0.0f;							//���ʁB

};
//���̃N���X�B
class RigidBody
{
	std::unique_ptr<btRigidBody>		 m_rigidBody;		//���́B
	std::unique_ptr<btDefaultMotionState>m_myMotionState;	//���[�V�����X�e�[�g�B
	bool m_isAddPhysicsWorld = false;			//�������[���h�ɒǉ�����Ă�H
public:
	~RigidBody();
	void Release();
	void Create(RigidBodyInfo& rbInfo);
	btRigidBody* GetBody()
	{
		return m_rigidBody.get();
	}
	//�������[���h�ɓo�^���̈������B
	void SetMarkAddPhysicsWorld()
	{
		m_isAddPhysicsWorld = true;
	}
	//�������[���h�ɓo�^���̈���O���B
	void SetUnmarkAddPhysicsWorld()
	{
		m_isAddPhysicsWorld = false;
	}
	//�������[���h�ɒǉ�����Ă���H
	bool IsAddPhysicsWorld() const
	{
		return m_isAddPhysicsWorld;
	}
	/// <summary>
	///�����I�u�W�F�N�g�̍��W�Ɖ�]��ݒ肷��
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	void SetPositionAndRotation(const CVector3& pos, const CQuaternion& rot);
	void GetPositionAndRotation(CVector3& pos, CQuaternion& rot)const;
};

