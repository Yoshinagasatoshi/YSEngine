#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include "gameObject/ysGameObjectManager.h"
#include "Enemy.h"
#include "Enemy_asigaru.h"

const float posClearRange = 600.0f * 600.0f;	//�N���A������s���͈́B
const float PLAYER_COLLIDER_HEIGHT = 20.0f;		//�v���C���[�̃J�v�Z���R���C�_�[�̍����B
const float PLAYER_COLLIDER_RADIUS = 20.0f;		//�v���C���[�̃J�v�Z���R���C�_�[�̔��a�B

const float SpeedAmount = 1000.0f;						//���ʂ̈ړ���
const float gravity = 600.0f;								//�d��
const float JumpPower = 1200.0f;							//�v���C���[�̔�ԗ�

Player::Player()
{
	CharaconInit();
	//cmo�t�@�C���̓ǂݍ��݁B
	m_playerModel.Init(L"Assets/modelData/busyo.cmo");
	m_busyoAnimeClip[animClip_idle].Load(L"Assets/animData/busyo_idle.tka");
	m_busyoAnimeClip[animClip_idle].SetLoopFlag(true);
	//�U���A�j�����[�h
	m_busyoAnimeClip[animClip_ATK1].Load(L"Assets/animData/busyo_kougeki.tka");
	m_busyoAnimeClip[animClip_ATK2].Load(L"Assets/animData/busyo_kougeki2.tka");
	m_busyoAnimeClip[animClip_ATK3].Load(L"Assets/animData/busyo_kougeki3.tka");
	m_busyoAnimeClip[animClip_ATK4].Load(L"Assets/animData/busyo_kougeki4.tka");
	m_busyoAnimeClip[animClip_ATK5].Load(L"Assets/animData/busyo_kougeki5.tka");
	//�_���[�W���[�h
	m_busyoAnimeClip[animClip_SmallDamage].Load(L"Assets/animData/busyo_smalldamage.tka");
	m_busyoAnimeClip[animClip_busyo_dead].Load(L"Assets/animData/busyo_dead.tka");
	//�S�����̍Đ��ł����̂�false�ɂ��Ƃ�
	for (int i = animClip_ATK1; i < animClip_num; i++) {
		m_busyoAnimeClip[i].SetLoopFlag(false);
	}
	//�A�j���[�V�����惂�f���ƃA�j���[�V�����̌���ݒ�B
	m_busyoAnime.Init(
		m_playerModel,
		m_busyoAnimeClip,
		animClip_num
	);
	
	m_busyoAnime.AddAnimationEventListener(	[&](const wchar_t* clipName, const wchar_t* eventName)
	{
		OnAnimationEvent(clipName,eventName);
		//(void)clipName;
		//MessageBox(NULL, "Attack", "attack", MB_OK);
	}
	);
	ghostInit();
}


Player::~Player()
{
}

void Player::CharaconInit()
{
	//�L�����R���̏�����
	m_characon.Init(
		PLAYER_COLLIDER_RADIUS,//�R���C�_�[�̔��a�̒���
		PLAYER_COLLIDER_HEIGHT,//�R���C�_�[�̍���
		m_position
	);
}
void Player::Update()
{
	if (!m_deadFrag) {
		//�v���C���[������ł��Ȃ����̏����B
		//���ʂ̈ړ��ʂ̓A�v�f���ƂɃ��Z�b�g����
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;
		//���͗ʂ��󂯎��
		float WideMove = g_pad->GetLStickXF();
		float heightMove = g_pad->GetLStickYF();


		//�J�����̑O�����ƉE�������擾
		CVector3 CameraForward = g_camera3D.GetForword();
		CVector3 CameraRight = g_camera3D.GetRight();
		//Y�̏��͂���Ȃ��̂�0�ɂ��A�O�ƉE�����̒P�ʂƂ���B
		CameraForward.y = 0.0f;
		CameraForward.Normalize();
		CameraRight.y = 0.0f;
		CameraRight.Normalize();
		m_moveSpeed += CameraForward * heightMove * SpeedAmount;
		m_moveSpeed += CameraRight * WideMove * SpeedAmount;
		m_moveSpeed.y -= gravity * gravity_keisuu;

		//�n�ʂ��Ă�H
		if (m_characon.IsOnGround()) {
			//�d�͂͂���Ȃ�
			AttackMove();
			gravity_keisuu = 0.1f;
			//�W�����v���Ă��H
			if (m_Jumpfrag) {
				m_Jumpfrag = false;
			}
		}
		//�X�e�[�g���Ƃɂ̏����Ɍ�ł���B
		if (g_pad->IsTrigger(enButtonA)) {
			if (!m_Jumpfrag) {
				m_moveSpeed.y += JumpPower;
				m_Jumpfrag = true;
				m_animStep = 0;
			}
			gravity_keisuu += 0.1f;
			m_busyoAnime.Play(animClip_idle, 0.5f);
		}
		//������ӂ̏����ł͂ق��Ɋ֐����g����������������
		if (m_damagefrag)
		{
			m_damagefrag = false;
			if (PL_HP != 0) {
				PL_HP--;
			}
			else {
				m_deadFrag = true;
			}
			m_playerState = animClip_SmallDamage;
			m_busyoAnime.Play(animClip_SmallDamage, 0.5f);
		}
		if (m_playerState == animClip_SmallDamage
			&& !m_busyoAnime.IsPlaying())
		{
			m_playerState == animClip_idle;
			m_busyoAnime.Play(animClip_idle, 0.5f);
		}
		Turn();
	}
	else {
		//�v���C���[������ł��鎞�̏���
		m_moveSpeed = CVector3::Zero();
		m_busyoAnime.Play(animClip_busyo_dead);
	}
	//���[���h�s��̍X�V�B
	m_ghostObject.Release();
	m_position = m_characon.Execute(1.0f / 60.0f, m_moveSpeed);
	m_ghostObject.SetPosition(m_position);
	//m_position.Set(CVector3::Zero());
	m_playerModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	
	m_busyoAnime.Update(1.0f / 30.0f);
}
void Player::Draw()
{
	//�v���C���[��`��
	m_playerModel.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}
void Player::Turn()
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
void Player::AttackMove()
{
	//��Ԏ���
	float InterpolationTime = 0.5;
	if (g_pad->IsTrigger(enButtonB)&&m_playTimer>10.0f) {
		int i = 0;
		//���肵�܂��B
		//�X�g���e�W�[�p�^�[���\���R
		switch (m_animStep)
		{
		case 0:
			m_busyoAnime.Play(animClip_ATK1, InterpolationTime);
			m_animStep++;
			break;
		case 1:
			m_busyoAnime.Play(animClip_ATK2, InterpolationTime);
			m_animStep++;
			break;
		case 2:
			m_busyoAnime.Play(animClip_ATK3, InterpolationTime);
			m_animStep++;
			break;
		case 3:
			m_busyoAnime.Play(animClip_ATK4, InterpolationTime);
			m_animStep++;
			break;
		case 4:
			m_busyoAnime.Play(animClip_ATK5, InterpolationTime);
			m_animStep++;
			break;
		}
	}
	m_playTimer++;
	if (m_animStep != 0) {
		if (m_animStep != m_oldAnimStep) {
			m_playTimer = 0;
			m_oldAnimStep = m_animStep;
		}
		if (m_playTimer >= 40) {
			//���̎��Ԃ��߂�����A�j���X�e�[�g�֌W��������
			m_animStep = 0;
			m_oldAnimStep = 0;
			m_playTimer = 0;
			m_busyoAnime.Play(animClip_idle, InterpolationTime*2.0f);
		}
	}
}



int Player::RequestEnemyData(CVector3 pos,Enemy* enemy)
{
	for (int i = 0; i < kakoi_max; i++)
	{
		
		//��ԍŏ��ɃG�l�~�[�̋󂢂Ă��鏊�ɏ�������
		if (m_enemydata[i].position.y == NULL) {
			m_enemydata[i].position = pos;
			m_enemydata[i].enemy = enemy;
			return i;
		}
		else if (m_enemydata[i].enemy == enemy) {
			return i;
		}

		if (m_enemydata[i].position.y != NULL) {
			CVector3 kyori = m_enemydata[i].position - pos;
			if (kyori.LengthSq() > posClearRange) {
				m_enemydata[i].enemy = NULL;
				m_enemydata[i].position = CVector3{NULL,NULL,NULL};
				return -1;
			}
		}
	}
	//null�łȂ�����
	//����������Ă�����
	//pos���k���ɂ���
	//-1��
	//�������ɍs���ƒʏ�ʂ�
	return -1;
}

void Player::ghostInit()
{
	CVector3 BIG = { 100.0f,100.0f,100.0f };
	m_ghostObject.CreateBox(
		m_position,
		m_rotation,
		BIG
	);
}