#pragma once
/// <summary>
/// ����N���X
/// �N�i�C�┚�e�Ȃǂ̔�ѓ���n�̕���̋����̐e�ɂȂ�N���X
/// �����⑫�y�������Ă��镐��͌p�����Ȃ����ƁB
/// </summary>
class Wepon
{
public:
	Wepon();
	~Wepon();
private:
	//��莞�Ԃ��Ə�����֐�
	virtual void DispTime();
	//���킲�Ƃ̍U��������o���֐��B
	virtual void WeponATKjuge();
	//�A�v�f
	virtual void Update();
protected:
	SkinModel m_kunaiModel;							//�X�L�����f���B
};

