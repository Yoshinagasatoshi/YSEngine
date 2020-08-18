#include "stdafx.h"
#include "gameObject/ysGameObjectManager.h"
#include "Wepon_ghost.h"
#include "Enemy_Busyo.h"
#include "GameClear.h"
#include "Game.h"
#include "InGameSoundDirector.h"
#include "Fade.h"
#include "InGameSoundDirector.h"
const float power = 250.0f;
const float InitHP = 1;
Enemy_Busyo::Enemy_Busyo()
{
	m_HP = InitHP;
	m_model.Init(L"Assets/modelData/enemy_busyo.cmo");
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(),
		L"Assets/modelData/_Users_GC1831_Desktop_YSEngine_YSEngine_GameTemplate_Game_Assets_modelData_BUSYO__HUKU.png.dds",		//���[�h����e�N�X�`���̃p�X
		0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
		false,
		nullptr,
		&m_normalMapSRV	//�쐬���ꂽSRV�̃A�h���X�̊i�[��
	);
	m_rotation = CQuaternion::Identity();
	m_scale = CVector3::One();
	m_animClip[ATK].Load(L"Assets/animData/enemy_Busyo_Attack.tka");
	m_animClip[IDL].Load(L"Assets/animData/enemy_Busyo_Leftstep.tka");
	m_animClip[MOVE].Load(L"Assets/animData/enemy_Busyo_Inflate.tka");
	m_animClip[DAMAGE].Load(L"Assets/animData/enemy_Busyo_Damage.tka");
	m_animClip[DEATH].Load(L"Assets/animData/enemy_Busyo_Death.tka");
	m_animClip[FIGHTING].Load(L"Assets/animData/enemy_Busyo_Fighting_Pose.tka");
	m_animClip[FIGHTING_KICK].Load(L"Assets/animData/enemyBusyo_newKick.tka");
	m_animClip[FIGHTING_LONG].Load(L"Assets/animData/enemy_Busyo_LongAttack.tka");

	m_enemy_BusyoAnime.Init(
		m_model,
		m_animClip,
		AnimationClip_Num
	);

	m_animClip[ATK].SetLoopFlag(false);
	m_animClip[MOVE].SetLoopFlag(false);
	m_animClip[IDL].SetLoopFlag(true);
	m_animClip[DAMAGE].SetLoopFlag(false);
	m_animClip[DEATH].SetLoopFlag(false);
	m_animClip[FIGHTING].SetLoopFlag(true);
	m_animClip[FIGHTING_KICK].SetLoopFlag(false);
	m_animClip[FIGHTING_LONG].SetLoopFlag(false);

	m_enemy_BusyoAnime.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName) {
		(void)clipName;
		m_en_Wepon = g_goMgr.NewGameObject<Wepon_ghost>("EN_Wepon");
		m_en_Wepon->SetPosition(m_position);
		m_en_Wepon->SetPlayerInfo(m_player);
		m_en_Wepon->GhostInit();
		}
	);
}

Enemy_Busyo::~Enemy_Busyo()
{

}

void Enemy_Busyo::Update()
{
	//�L�����R���������Ă��Ȃ�����������B
	if (!m_charaConUse) {
		CharaconInit();
		m_charaConUse = true;
	}
	//m_state�W���b�W�B���̒���m_state������U����B
	StateJudge();

	switch (m_state)
	{
	case ATK:
		m_player->RequestEnemyData(m_position, this);
		AttackMove();
		break;
	case MOVE:
		NormalMove();
		break;
	case IDL:
		IdleMove();
		break;
	case DAMAGE:
		ThisDelete();
	case DAMAGE_AFTER:
		DamageAfter();
	}
	//���߁[�������
	ThisDamage();

	//���[���h���W�̍X�V�@��������skeletonUpdate��������
	auto footStep = m_enemy_BusyoAnime.Update(1.0f / 30.0f);//���[�J�����W�̍X�V�@�������͂�����Ȃ�
	//if (m_busyoState == BusyoAttack) {
	//�U�����̓t�b�g�X�e�b�v�̈ړ��ʂ����Z����B
	CMatrix mBias = CMatrix::Identity();
	mBias.MakeRotationX(CMath::PI * -0.5f);
	CMatrix rotMatrix;
	//��]�s����쐬����B
	rotMatrix.MakeRotationFromQuaternion(m_rotation);
	rotMatrix.Mul(mBias, rotMatrix);
	rotMatrix.Mul(footStep);
	footStep *= 600.0f;
	m_moveSpeed += footStep;

	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	//���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

}

void Enemy_Busyo::CharaconInit()
{
	//�L�����R���̏�����
	m_characon.Init(
		60.0f, //���a
		100.0f,//����
		m_position//�ʒu
	);
}

void Enemy_Busyo::Draw()
{
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}


void Enemy_Busyo::StateJudge()
{
	/// <summary>
	/// �v���C���[�Ƃ̋����ŌĂԊ֐���ς��Ă��鏈��
	/// </summary>
	distance = m_player->GetPosition() - m_position;

	//�_���[�W���󂯂���łȂ����
	if (m_state != DAMAGE_AFTER) {
		if (distance.LengthSq() > BattleRange
			&& distance.LengthSq() < VigilanceRange) {
			m_state = MOVE;
		}
		if (distance.LengthSq() > VigilanceRange) {
			m_state = IDL;
		}
		if (distance.LengthSq() < BattleRange) {
			m_state = ATK;
		}
		//�_���[�W���󂯂Ă��鎞�͑��̂̃X�e�[�g�����D��
		if (m_isDeadfrag) {
			m_state = DAMAGE;
		}
	}
}



//�U���͈͂ɗ����炱���ɓ���B
void Enemy_Busyo::AttackMove()
{
	m_frameTimer += 1.0f;

	//�U�����̓���
	m_moveSpeed = CVector3::Zero();
	//���[���h���W�̍X�V
	m_moveSpeed.y += grabity * 10.0f;
	if (m_characon.IsOnGround()) {
		m_moveSpeed.y = 0.0f;
	}
	//�U���̊��o�����߂�
	AttackframeNum();
	if (m_frameTimer > m_attackFrameNum)
	{
		if (!m_isFight) {
			//�U�����@�̒��I
			m_gacha = 1;//rand() % 3;
			m_isFight = true;
		}
		//���������Ȃ邩��֐��ɂ�����
		switch (m_gacha)
		{
		case 0:
			m_enemy_BusyoAnime.Play(ATK, 0.2f);
			if (!m_enemy_BusyoAnime.IsPlaying())
			{
				InGameSoundDirector::GetInstans().RingSE_Slash();
				m_frameTimer = 0;
				m_enemy_BusyoAnime.Play(MOVE, 0.2f);
				m_isFight = false;
			}
		break;
		case 1:
			//�A�j���[�V�����C�x���g������
			//�A�j���[�V���������ꂽ��_���[�W��H�炤�͂�
			m_enemy_BusyoAnime.Play(FIGHTING_KICK, 0.2f);
			if (!m_enemy_BusyoAnime.IsPlaying())
			{
				InGameSoundDirector::GetInstans().RingSE_Slash();
				m_frameTimer = 0;
				m_enemy_BusyoAnime.Play(MOVE, 0.2f);
				m_isFight = false;
			}
			break;
		case 2:
			InGameSoundDirector::GetInstans().RingSE_Slash();
			m_enemy_BusyoAnime.Play(FIGHTING_LONG, 0.2f);
			if (!m_enemy_BusyoAnime.IsPlaying())
			{
				m_frameTimer = 0;
				m_enemy_BusyoAnime.Play(MOVE, 0.2f);
				m_isFight = false;
			}
			break;
		}

	}
	if (!m_isFight) {
		m_enemy_BusyoAnime.Play(FIGHTING, 0.1f);
	}

	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
}

//���G�͈͂ɗ����炱���ɓ���
void Enemy_Busyo::NormalMove()
{
	if (distance.LengthSq() > 200.0f * 200.0f) {
		CVector3 direction = distance;
		direction.y = 0.0f;
		direction.Normalize();
		m_moveSpeed = direction * power;
		//���[���h���W�̍X�V
		m_moveSpeed.y += grabity;
		if (m_characon.IsOnGround()) {
			m_moveSpeed.y = 0.0f;
		}
		float angle = atan2(distance.x, distance.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
		m_enemy_BusyoAnime.Play(MOVE, 0.2f);
		//�A�j���[�V�����C�x���g�̂��߂ɕ����A�j�������[�v�Đ����烏���V���b�g�Đ��ɕύX�B
		//��������܂������߂ɁA�A�j���[�V�����Đ����I�������1�t���[�������ʂ̃A�j��������B
		if (!m_enemy_BusyoAnime.IsPlaying()) {
			m_enemy_BusyoAnime.Play(IDL, 0.1f);
		}
	}
	else {
		m_moveSpeed = CVector3::Zero();
		m_enemy_BusyoAnime.Play(IDL, 0.1f);
	}
	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
}

//���������������炱���ɓ���B
void Enemy_Busyo::IdleMove()
{
	m_moveSpeed = CVector3::Zero();
	//���[���h���W�̍X�V
	m_moveSpeed.y += grabity;
	if (m_characon.IsOnGround()) {
		m_moveSpeed.y = 0.0f;
	}

	m_enemy_BusyoAnime.Play(IDL, 0.2f);
	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
}

void Enemy_Busyo::ThisDamage()
{
	//����̃S�[�X�g�����������ɓ���������A�_���[�W��H������ƒm�点��悤�ɂ���
	QueryGOs<Wepon_ghost>("PL_Wepon", [&](Wepon_ghost* wepon) {
		PhysicsGhostObject* ghostobject = wepon->GetGhostObject();
		g_physics.ContactTest(m_characon, [&](const btCollisionObject& contactObject) {
			if (ghostobject->IsSelf(contactObject) == true) {
				//�ʂ��Ă���̂͊m�F����
				m_isDeadfrag = true;
				//�G�t�F�N�g���o���B
				g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(
					g_Effect.m_sampleEffect,
					m_position.x, m_position.y + 100.0f,
					m_position.z
				);

			}
			});
		return true;
		});
}

void Enemy_Busyo::ThisDelete()
{
	//�̗͂͂܂��c���Ă��邩�ǂ���
	if (m_HP != 0) {
		InGameSoundDirector::GetInstans().RingSE_Slash();
		m_HP--;
		m_enemy_BusyoAnime.Play(DAMAGE, 0.2f);
		m_state = DAMAGE_AFTER;
	}
	//�Ȃ��Ȃ��Ă����炱�����ʂ�
	else {
		m_state = DAMAGE;
		m_moveSpeed = CVector3::Zero();
		//enemy�p�ɂ�
		ThisDeath();
		//�v���C���[�ɖ��G������B
		//�v���C���[���{�X��|������Ɏ��ʂ�
		//�Q�[���N���A�ƃQ�[���I�[�o�[���ꏏ�ɏo�Ă��܂����߁B
		m_player->SetPlayerMuTeki();

		m_enemy_BusyoAnime.Play(DEATH, 0.1f);
		if (!m_isDestroyed && !m_enemy_BusyoAnime.IsPlaying()) {
			{
				Fade::Getinstance().StartFadeIn();
			}
			if (!Fade::Getinstance().IsFade()) {
				g_goMgr.ResetCount();
				InGameSoundDirector::GetInstans().UpdateOff();
				//�|����ok�Ƃ�����Ԃ�
				g_goMgr.NewGameObject<GameClear>("GameClear");
				//�����Ă˂��H
				m_game->GameDelete();
				m_isDestroyed = true;
			}
		}

	}
}

void Enemy_Busyo::DamageAfter()
{
	//�A�j���[�V�����������I�������IDL�ɐ؂�ւ���B
	if (!m_enemy_BusyoAnime.IsPlaying()) {
		//�؂�ւ�
		m_state = IDL;
		//m_enemy_BusyoAnime.Play(IDL, 0.2f);
		//�̗͂��[������Ȃ���΃_���[�W������悤�ɂ�����A
		if (m_HP != 0) {
			m_isDeadfrag = false;
		}
	}
}