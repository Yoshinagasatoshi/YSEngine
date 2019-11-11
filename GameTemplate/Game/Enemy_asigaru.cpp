#include "stdafx.h"
#include "Enemy_asigaru.h"
#include "gameObject/ysGameObjectManager.h"
#include "GameData.h"

Enemy_asigaru::IdlePos Enemy_asigaru::m_idlePos[5];
//�R���X�g���N�^
Enemy_asigaru::Enemy_asigaru()
{
	//asigaru�̃��f�������[�h����B
	m_model.Init(L"Assets/modelData/asigaru.cmo");
	m_rotation = CQuaternion::Identity();
	m_scale = CVector3::One();
	//�A�j���[�V�������i�[��
	m_asigaruAnimeClip[Asigaru_totugeki].Load(L"Assets/animData/asigaru_totugeki.tka");
	m_asigaruAnimeClip[Asigaru_tikazuki].Load(L"Assets/animData/asigaru_tikazuki.tka");
	m_asigaruAnimeClip[Asigaru_sentou].Load(L"Assets/animData/asigaru_Normal_Attack.tka");

	m_asigaruAnimeClip[Asigaru_totugeki].SetLoopFlag(true);
	m_asigaruAnimeClip[Asigaru_tikazuki].SetLoopFlag(true);
	m_asigaruAnimeClip[Asigaru_sentou].SetLoopFlag(true);

	m_asigaruAnime.Init(
		m_model,
		m_asigaruAnimeClip,
		3
	);

	
}
//�f�X�g���N�^
Enemy_asigaru::~Enemy_asigaru()
{
}

void Enemy_asigaru::CharaconInit()
{
	//�d�Ă��I�I�I�I�I�I�I�I�I�I�I�I
	//�L�����R���̏�����
	m_characon.Init(
		3.0f,
		3.0f,
		m_position
	);
}
void Enemy_asigaru::Update()
{
	const float grabity = -30.0f;
	if (!m_characonState) {
		m_characonState = true;
		CharaconInit();
	}
	idlePosInit();
	
	Move();
	StateJudge();
	m_moveSpeed.y = grabity;
	//���[���h���W�̍X�V
	m_position = m_characon.Execute(1.0f / 30.0f, m_moveSpeed);
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_asigaruAnime.Update(1.0f / 30.0f);
}
void Enemy_asigaru::Draw()
{
	//���f���̕`��
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

// 11/11 �͂���true��false���ŏ����𕪂������������H
void Enemy_asigaru::Move()
{
	float range = 50.0f * 50.0f;
	CVector3 kyori = m_player->GetPosition() - m_position;
	kyori.y = 0.0f;
	for (int i = 0; i < m_gameData->GetKakoiNum(); i++) {

		//�X�|�b�g���m��
		bool SpotUse = m_gameData->GetSpotUse(i);

		//�����͋߂��H
		if (kyori.LengthSq() < range) {
			kyori.Normalize();
			float angle = acosf(kyori.Dot(m_forward));
			//����p�����Ă�H
			if (fabsf(angle) > PI * 0.25f) {
				m_asigaruState = Asigaru_sentou;
				//�l���v�Z�p�̕ϐ��Ɏ󂯓n��
				m_moveSpeed = kyori;
			}
		}
		else if (kyori.LengthSq() < 300.0f * 300.0f) {
			//�g���Ă�����X���[�@�g���ĂȂ��Ȃ炻���Ɉړ�����
			//gamedata�Ɏg���Ă���Ƃ������𑗂�[
			if (SpotUse) {
				CVector3 kakoi_kyori = m_gameData->GetSpotPos(i) - m_position;
				kakoi_kyori.y = 0.0f;
				kakoi_kyori.Normalize();
				float angle = acosf(kakoi_kyori.Dot(m_forward));
				//����p�����Ă�H
				if (fabsf(angle) > PI * 0.25f) {
					m_asigaruState = Asigaru_tikazuki;
					//�l���v�Z�p�̕ϐ��Ɏ󂯓n��
					m_moveSpeed = kakoi_kyori * 50.0f;
				}
			}
			else {
				m_gameData->SetSpotUse(i);
			}
			//kyori.Normalize();
			//float angle = acosf(kyori.Dot(m_forward));
			////����p�����Ă�H
			//if (fabsf(angle) > PI * 0.25f) {
			//	m_asigaruState = Asigaru_tikazuki;
			//	//�l���v�Z�p�̕ϐ��Ɏ󂯓n��
			//	m_moveSpeed = kyori * 10.0f;
			//}
		}
		else {
			m_asigaruState = Asigaru_totugeki;
			m_moveSpeed = CVector3::Zero();
		}
	}
		CQuaternion ADDrot = CQuaternion::Identity();
		ADDrot.SetRotationDeg(CVector3::AxisY(), 2.0f);
		m_rotation.Multiply(ADDrot);
	
	Turn();
}

void Enemy_asigaru::Turn()
{
	if (fabsf(m_moveSpeed.x) <= 0.001f    //fabsf�͐�Βl�Bm_movespeed.x&m_movespeedz��
		&& fabsf(m_moveSpeed.z) <= 0.001f) {//0.001�ȉ��̎��ɂ͉������Ȃ��B
		return;
	}
	else {
		float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
	}
}
//������͂ރv���C���[�̎�����͂ޏꏊ�̏��
void Enemy_asigaru::idlePosInit()
{
	//�S�_������Ă����ɃG�l�~�[���Q���点��
	//���x���ł�������������Ƃ����A�h���@�C�X����������̂�
	//��Ń��x���ɕς���B
	m_playerPos = m_player->GetPosition();
	m_idlePos[0].idlePos = m_playerPos + CVector3{ 0.0f,0.0f,100.0f };
	m_idlePos[1].idlePos = m_playerPos + CVector3{ 0.0f,0.0f,-100.0f };
	m_idlePos[2].idlePos = m_playerPos + CVector3{ -100.0f,0.0f,0.0f };
	m_idlePos[3].idlePos = m_playerPos + CVector3{ 100.0f,0.0f,0.0f };
	m_idlePos[4].idlePos = m_playerPos + CVector3{ 0.0f,0.0f,0.0f };
}
//�����ɂ�锻�菈��
void Enemy_asigaru::StateJudge()
{
	if (m_oldState != m_asigaruState)
	{
		m_oldState = m_asigaruState;
		m_asigaruAnime.Play(m_asigaruState, 0.2f);
	}
}