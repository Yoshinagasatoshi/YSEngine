#include "stdafx.h"
#include "Player.h"
#include "Player_target.h"
#include <iostream>
#include "gameObject/ysGameObjectManager.h"
#include "Enemy.h"
#include "Enemy_asigaru.h"
#include "Wepon_ghost.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
#include "SoundDirector.h"
#include "GameOver.h"
#include "Game.h"
#include "Fade.h"
#include "graphics\shadow\ShadowMap.h"

const float posClearRange = 600.0f * 600.0f;	//�N���A������s���͈́B�Q�[���N���A�ł͂Ȃ�
const float PLAYER_COLLIDER_HEIGHT = 100.0f;	//�v���C���[�̃J�v�Z���R���C�_�[�̍����B
const float PLAYER_COLLIDER_RADIUS = 60.0f;		//�v���C���[�̃J�v�Z���R���C�_�[�̔��a�B

const float SpeedAmount = 1500.0f;				//���ʂ̈ړ���
const float gravity = 200.0f;					//�d��
const float JumpPower = 800.0f;				//�v���C���[�̔�ԗ�
const float JumpATKPower = 350.0f;				//�W�����v�`���̋���
const float standardPower = 200.0f;				//�v���C���[�̓G������΂���
const float limit = 2.0f;						//�d�͌W���̏��
const int Timer_ZERO = 0;						//0�ɂȂ�B���̂܂�
const float InterpolationTime = 0.2f;			//�A�j���[�V�����̕�Ԏ���
const int	 onebrock = 24;						//�萔�_���[�W�B�{�X����̍U���Ƃ��З͂������z�ɂ�*2�Ƃ�����Ǝv��
//�A�j���[�V�����̕�Ԏ��ԁE��
const float InterpolationTimeS = 0.1f;		
//�A�j���[�V�����̕�Ԏ��ԁE��
const float InterpolationTimeM = 0.2f;		
//�A�j���[�V�����̕�Ԏ��ԁE��
const float InterpolationTimeL = 0.5f;			

Player::Player()
{
	testID = rand();
	//��SE�f��
	m_sd = &SoundDirector::GetInstans();
	m_sd->SoundInit();
	m_bgm.Init(L"Assets/sound/Chanbara.wav");
	m_bgm.Play(true);
	//se�`�F�b�N�̂��߂ɁA�啪������
	m_bgm.SetVolume(1.0f);
	CharaconInit();
	m_pl_target = g_goMgr.NewGameObject<Player_target>("PLT");
	m_pl_target->SetPlayerInfo(this);
	//cmo�t�@�C���̓ǂݍ��݁B
	m_playerModel.Init(L"Assets/modelData/busyo.cmo");
	//�ړ���Ԃ̃��[�h
	m_busyoAnimeClip[animClip_idle].Load(L"Assets/animData/busyo_idle.tka");
	m_busyoAnimeClip[animClip_idle].SetLoopFlag(true);
	m_busyoAnimeClip[animClip_Walk].Load(L"Assets/animData/busyo_walk.tka");
	m_busyoAnimeClip[animClip_Walk].SetLoopFlag(true);
	m_busyoAnimeClip[animClip_jump].Load(L"Assets/animData/busyo_jump.tka");
	m_busyoAnimeClip[animClip_jump].SetLoopFlag(false);
	//�U���A�j�����[�h
	m_busyoAnimeClip[animClip_ATK1].Load(L"Assets/animData/busyo_kougeki.tka");
	m_busyoAnimeClip[animClip_ATK2].Load(L"Assets/animData/busyo_kougeki2.tka");
	m_busyoAnimeClip[animClip_ATK3].Load(L"Assets/animData/busyo_kougeki3.tka");
	m_busyoAnimeClip[animClip_ATK4].Load(L"Assets/animData/busyo_kougeki4.tka");
	m_busyoAnimeClip[animClip_ATK5].Load(L"Assets/animData/busyo_kougeki5.tka");
	m_busyoAnimeClip[animClip_XATK].Load(L"Assets/animData/busyo_kaitengiri.tka");
	m_busyoAnimeClip[animClip_JUMP_ATK].Load(L"Assets/animData/busyo_jump_kougeki.tka");
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

	m_skelton = &m_playerModel.GetSkeleton();
	//���̃R�[�h�̓{�[���̔z����m�F���邽�߂ɏ����Ă���R�[�h�B���ڃQ�[���ɂ͊ւ���ĂȂ��B
	//�Ƃ肠����30������z����쐬
	//30�Ȃ̂̓v���C���[�̃{�[���̐���30������ł��B
	const wchar_t* bonename[30];

	for (int i = 0; i < 29; i++) {
		bonename[i] = m_skelton->GetBone(i)->GetName();

		if (i == 28)
		{
			bonename[i + 1] = L"end";
		}
	}

	m_busyoAnime.AddAnimationEventListener(	[&](const wchar_t* clipName, const wchar_t* eventName)
	{
		auto m_bone = m_skelton->GetBone(20);
		CVector3 bonepos;
		//y�������������{�[�����Z�b�g
		bonepos.Set(
			m_bone->GetWorldMatrix().m[3][0],
			0,
			m_bone->GetWorldMatrix().m[3][2]
		);
		//OnAnimationEvent(clipName,eventName);

		//�{�[����pos�ƃv���C���[��pos�𑫂����ꏊ
		m_calcPos = m_position + bonepos;
		//ghost���������܂��Ă����̂ŏ�����ɍ��킹��B
		m_calcPos.y += 70.0f;

		m_pl_Wepon = g_goMgr.NewGameObject<Wepon_ghost>("PL_Wepon");
		m_pl_Wepon->SetPosition(m_calcPos);
		m_pl_Wepon->SetPlayerInfo(this);
		m_pl_Wepon->GhostInit();

		if (!m_se.IsPlaying()) {
			m_se.Init(L"Assets/sound/swing.wav");
			m_se.Play(false);
			m_se.SetVolume(3.0f);//�����ɂł������Ă���B��Œ���
		}
		else {
			if (!m_se2.IsPlaying()) {
				m_se2.Init(L"Assets/sound/swing.wav");
				m_se2.Play(false);
				m_se2.SetVolume(3.0f);//�����ɂł������Ă���B��Œ���
			}
		}
	}
	);

}


Player::~Player()
{
	//g_goMgr.DeleteGOObject(this);
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
	if (m_deadFrag) {
		m_busyoState = BusyoDead;
	}
	if (m_busyoState != BusyoDead) {
		//�n�ʂ��Ă�H
		if (m_characon.IsOnGround()) {
			//�U������Ƃ��̃��[�V����
			AttackMove();
			//�d�͂̏d�݂��y������
			m_gravity_keisuu = 0.1f;
			//�W�����v���Ă��H
			if (m_Jumpfrag) {
				m_Jumpfrag = false;
				m_jumpAttackfrag = false;
			}
		}
		//�X�e�[�g���Ƃɂ̏����Ɍ�ł���B
		if (m_busyoState != BusyoAttack) {
			if (g_pad->IsTrigger(enButtonA)) {
				if (!m_Jumpfrag) {
					m_moveSpeed.y += JumpPower;
					m_Jumpfrag = true;
					m_animStep = 0;
					//�U�����[�V�������ɃW�����v����ƃv���C���[�������Ȃ��Ȃ鎞���������̂Œǉ�
					m_underAttack = false;
				}
			}
			m_gravity_keisuu += 0.1f;
			if (m_gravity_keisuu > limit) {
				m_gravity_keisuu = limit;
			}
			//�W�����v��Ԃ���Ȃ���Έړ����x�ɂ���ăA�j���[�V������ς���B
			if (!m_Jumpfrag) {
				if (m_moveSpeed.Length() > 300.0f) {
					m_busyoAnime.Play(animClip_Walk, InterpolationTimeS);
				}
				else {
					m_busyoAnime.Play(animClip_idle, InterpolationTimeS);
				}
			}
			else {
				JumpAttackMove();
			}
		}
		//������ӂ̏����ł͂ق��Ɋ֐����g����������������
		if (m_damagefrag)
		{
			m_damagefrag = false;
			if (m_PL_HP != onebrock) {
				m_PL_HP -= onebrock;
			}
			else {
				m_deadFrag = true;
			}
			m_animState = animClip_SmallDamage;
			m_busyoAnime.Play(animClip_SmallDamage,InterpolationTimeL);
		}

		//�_���[�W�A�j���[�V�������I������痧���p��
		if (!m_busyoAnime.IsPlaying())
		{
			m_animState = animClip_idle;
			m_busyoState = BusyoNormal;
			//if (m_underAttack) {
			//	m_underAttack = false;
			//}
			if (!m_Jumpfrag) {
				m_busyoAnime.Play(animClip_idle,InterpolationTimeM);
			}
		}
		//�ړ�����
		Move();
		//haha
		//��]����
		Turn();
		
		//�G����̃S�[�X�g������������_���[�W���󂯂�B
		QueryGOs<Wepon_ghost>("EN_Wepon", [&](Wepon_ghost* wepon) {
			PhysicsGhostObject* ghostobject = wepon->GetGhostObject();
			g_physics.ContactTest(m_characon, [&](const btCollisionObject& contactObject) {
				if (ghostobject->IsSelf(contactObject) == true) {

					PlayerDamage();

					g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(g_Effect.m_sampleEffect, m_position.x, m_position.y + 100.0f, m_position.z);
				}
				});
			return true;
			});
	}
	else {
		//�v���C���[������ł��鎞�̏���
		m_moveSpeed = CVector3::Zero();
		m_busyoAnime.Play(animClip_busyo_dead);
		if (!m_busyoAnime.IsPlaying()
			&& !m_isDestroyed) {
			Fade::Getinstance().StartFadeIn();
		}
		if (!Fade::Getinstance().IsFade()) {
			m_isDestroyed = true;
			g_goMgr.NewGameObject<GameOver>("GameOver");
			m_game->GameDelete();
		}
	}
	Execute();
	/// <summary>
	/// �f�o�b�N�p�R�}���h�B��ŏ����B
	/// </summary>
	if (g_pad->IsTrigger(enButtonLeft)) {
		m_busyoState = BusyoDead;
	}

	g_graphicsEngine->GetShadowMap()->UpdateFromLightTarget(m_position + CVector3::One() * 300.0f,m_position);
}

void Player::Move()
{
	//�v���C���[������ł��Ȃ����̏����B
	//���ʂ̈ړ��ʂ̓A�v�f���ƂɃ��Z�b�g����B
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	if (m_busyoState != BusyoAttack) {
		//���͗ʂ��󂯎��
		WideMoveL = g_pad->GetLStickXF();
		heightMoveL = g_pad->GetLStickYF();

		//�J�����̑O�����ƉE�������擾
		m_CameraForward = g_camera3D.GetForword();
		m_CameraRight = g_camera3D.GetRight();
		//Y�̏��͂���Ȃ��̂�0�ɂ��A�O�ƉE�����̒P�ʂƂ���B
		m_CameraForward.y = 0.0f;
		m_CameraForward.Normalize();
		m_CameraRight.y = 0.0f;
		m_CameraRight.Normalize();
		//�U�����͎��R�ɓ����Ȃ����ɂ���B
		//m_busyoState = BusyoAttack;
		if (!m_underAttack) {
			m_moveSpeed += m_CameraForward * heightMoveL * SpeedAmount;
			m_moveSpeed += m_CameraRight * WideMoveL * SpeedAmount;
		}
	}
	m_moveSpeed.y -= gravity * m_gravity_keisuu;
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
	if(m_busyoState != BusyoAttack) {

		if (fabsf(m_moveSpeed.x) <= 0.001f    //fabsf�͐�Βl�Bm_movespeed.x&m_movespeedz��
			&& fabsf(m_moveSpeed.z) <= 0.001f) {//0.001�ȉ��̎��ɂ͉������Ȃ��B
			return;
		}
		else {
			float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
			m_rotation.SetRotation(CVector3::AxisY(), angle);
		}
	}
	else {
		if (m_pl_target->GetHosei()) {
			CVector3 kaiten = m_pl_target->GetDistans();
			float angle = atan2(-kaiten.x, -kaiten.z);
			m_rotation.SetRotation(CVector3::AxisY(), angle);
		}
	}
}

void Player::AttackMove()
{
	if (g_pad->IsTrigger(enButtonX)&&m_playTimer>3.0f) {
		
	//m_busyoState = BusyoAttack;
		if (!m_underAttack)
		{
			m_underAttack = true;
		}
		m_busyoState = BusyoAttack;
		//�␳��������ׂ��Ȃ̂��𒲂ׂ܂��B
		bool hosei = m_pl_target->GetHosei();
		
		//���肵�܂��B
		//�X�g���e�W�[�p�^�[���\���R
		switch (m_animStep)
		{
		case animClip_idle:
			m_busyoAnime.Play(animClip_ATK1, InterpolationTime);
			m_busyoAnimeClip->GetKeyFramePtrListArray();
			//enmu�̗��ꂽ�ʒu�ɃA�^�b�N�����邽�߁A�ŏ�����+= animClip_ATK1�𑫂�
			m_animStep += animClip_ATK1;
			m_blowOffPower = standardPower * 1.7f;
			break;
		case animClip_ATK1:
			m_busyoAnime.Play(animClip_ATK2, InterpolationTime);
			m_animStep++;
			break;
		case animClip_ATK2:
			m_busyoAnime.Play(animClip_ATK3, InterpolationTime);
			m_animStep++;
			break;
		case animClip_ATK3:
			m_busyoAnime.Play(animClip_ATK4, InterpolationTime);
			m_animStep++;
			m_blowOffPower = standardPower * 2.0f;
			break;
		case animClip_ATK4:
			m_busyoAnime.Play(animClip_ATK5, InterpolationTime);
			m_animStep++;
			m_blowOffPower = standardPower * 2.5f;
			break;
		}
	}
	XAttackMove();

	m_playTimer++;

	if (m_animStep != 0) {
		if (m_animStep != m_oldAnimStep) {
			m_playTimer = Timer_ZERO;
			m_oldAnimStep = m_animStep;
		}
		//�Ō�܂ōs���ƌ������炷���Ԃ𑝂₷
		if (m_animStep == animClip_ATK5) {
			//���₷����
			m_TimerRelease = 15;
		}
		if (m_playTimer >= m_TimerRelease) {
			//���̎��Ԃ��߂�����A�j���X�e�[�g�֌W��������
			m_busyoState = BusyoNormal;
			m_TimerRelease = 15;
			m_animStep = animClip_idle;
			m_oldAnimStep = animClip_idle;
			m_playTimer = Timer_ZERO;
			m_busyoAnime.Play(animClip_idle, InterpolationTime*2.0f);
			m_underAttack = false;
		}
	}
}

void Player::XAttackMove()
{
	float interTime = 0.1f;
	//�U�����ĂȂ�����Y�������ꂽ��
	if (g_pad->IsTrigger(enButtonY)&&!m_underAttack) {
		m_busyoState = BusyoAttack;
		m_busyoAnime.Play(animClip_XATK, interTime);
		m_blowOffPower = standardPower * 1.7f;
		m_underAttack = true;
		m_XTrigger = true;
	}
	if (m_XTrigger && !m_busyoAnime.IsPlaying()) {
		m_XTrigger = false;
		m_underAttack = false;
	}
}

void Player::Execute()
{
	//���[���h���W�̍X�V�@��������skeletonUpdate��������
	auto footStep = m_busyoAnime.Update(GameTime().GetFrameDeltaTime());//���[�J�����W�̍X�V�@�������͂�����Ȃ�
	//if (m_busyoState == BusyoAttack) {
	//�U�����̓t�b�g�X�e�b�v�̈ړ��ʂ����Z����B
	CMatrix mBias = CMatrix::Identity();
	mBias.MakeRotationX(CMath::PI * -0.5f);
	CMatrix rotMatrix;
	//��]�s����쐬����B
	rotMatrix.MakeRotationFromQuaternion(m_rotation);
	rotMatrix.Mul(mBias, rotMatrix);
	rotMatrix.Mul(footStep);

	footStep *= 32.0f;
	m_moveSpeed += footStep;

	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	//���[���h�s��̍X�V�B
	m_playerModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

	
	m_pl_target->HoseiJujge();
}

//�G�l�~�[�ɌĂ΂��z
int Player::RequestEnemyData(CVector3 pos,Enemy* enemy)
{
	for (int i = 0; i < 5; i++)
	{
		if (!enemy->GetenemyDeath()) {
			//��ԍŏ��ɃG�l�~�[�̋󂢂Ă��鏊�ɏ�������
			if (m_enemydata[i].position.y == 0.0f) {
				m_enemydata[i].position = pos;
				m_enemydata[i].enemy = enemy;
				//���̃^�C�~���O�œG�Ƃ̋����v��(�������߂�������Ă΂�鏈���Ȃ̂Ɂc�H)
				if (g_goMgr.GetEnemyNum() != 0) {
					m_pl_target->SetEnemyInfo(enemy);
				}
				return i;
			}
			else if (m_enemydata[i].enemy == enemy) {
				return i;
			}

			if (m_enemydata[i].position.y != 0.0f) {
				CVector3 kyori = m_enemydata[i].position - pos;
				if (kyori.LengthSq() > posClearRange) {
					m_enemydata[i].enemy = NULL;
					m_enemydata[i].position = CVector3{ 0.0f,0.0f,0.0f };
					m_pl_target->Hoseioff();
					return -1;
				}
			}
		}
		else {
			enemy = nullptr;
			m_pl_target->SetEnemyInfo(enemy);
		}
	}
	//null�łȂ�����
	//����������Ă�����
	//-1��߂�l�ɂ���
	return -1;
}

void Player::JumpAttackMove() {
	if (m_Jumpfrag && !m_jumpAttackfrag) {
		if (g_pad->IsTrigger(enButtonX)) {
			m_jumpAttackfrag = true;
			m_busyoAnime.Play(animClip_JUMP_ATK, InterpolationTimeS);
			m_blowOffPower = JumpATKPower;
		}
	}
	if (!m_jumpAttackfrag) {
		m_busyoAnime.Play(animClip_jump, InterpolationTimeM);
	}
}