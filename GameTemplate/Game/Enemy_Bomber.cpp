#include "stdafx.h"
#include "gameObject/ysGameObjectManager.h"
#include "Enemy_Bomber.h"
#include "Wepon_ghost.h"
#include "bom.h"
const float InterpolationTime = 0.2f;			//�A�j���[�V�����̕�Ԏ���
const float bomberSpeed = 5.0f;
const float harfrenge = 1400.0f;
const float socialdistance = 700.0f;			//���ł��B�R�ł��B�v���C���[�Ƃ̊ԍ����ł�
const float escapeSpeed = 200.0f;				//�������X�s�[�h
const float gravity = -20.0f;					//�d��
Enemy_Bomber::Enemy_Bomber()
{
	//�{���G�̃��f����asigaru�̃��f���ɑ���������������̂Ȃ̂�
	//�|��鎞�Ƃ��̃A�j����asigaru�̃A�j���𗬗p���܂��B
	m_model.Init(L"Assets/modelData/asigaru_taicho.cmo");
	m_rotation = CQuaternion::Identity();
	m_scale = CVector3::One();
	m_bomAnimeClip[Asigaru_attack].Load(L"Assets/animData/asigaru_nage.tka");
	m_bomAnimeClip[Asigaru_escape].Load(L"Assets/animData/asigaru_tikazuki.tka");
	m_bomAnimeClip[Asigaru_damage].Load(L"Assets/animData/asigaru_damage.tka");
	m_bomAnimeClip[Asigaru_dead].Load(L"Assets/animData/asigaru_dead.tka");
	m_bomAnimeClip[Asigaru_zyosou].Load(L"Assets/animData/asigaru_zyosou.tka");
	m_bomAnimeClip[Asigaru_Idle].Load(L"Assets/animData/asigaru_bomIdle.tka");
	m_bomAnimeClip[Asigaru_attack].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_escape].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_damage].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_dead].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_zyosou].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_Idle].SetLoopFlag(false);
	//���f���ƃA�j���f�[�^��������
	m_bomberAnime.Init(
		m_model,
		m_bomAnimeClip,
		Asigaru_anim_num//�e�X�g
	);
	m_bomberAnime.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName) {
		(void)clipName;
		m_bom = g_goMgr.NewGameObject<bom>("bom");
		m_bom->SetPosition(m_position);
		m_bom->SetPlayerInfo(m_player);
		m_bom->SetInpactPosition(m_player->GetPosition());
		}
	);
}

Enemy_Bomber::~Enemy_Bomber()
{
	g_goMgr.DeleteGOObject(this);
}

void Enemy_Bomber::Draw()
{
	//���f���̕`��
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Enemy_Bomber::Update()
{
	//�Ȃ���������
	if (!m_isCharaconUse) {
		m_isCharaconUse = true;
		CharaconInit();
	}
	//m_moveSpeed = m_player->GetPosition() - m_position;
	//m_moveSpeed.Normalize();
	//m_moveSpeed *= bomberSpeed;
	if (!m_bomberAnime.IsPlaying()) {
		Statekanri();
		Animekanri();
		Move();
		Turn();
	}
	if (m_isDeadfrag) {
		DeathMove();
	}

	//����̃S�[�X�g�����������ɓ���������A���񂾂Ƃ����M���𗧂Ă�
	QueryGOs<Wepon_ghost>("PL_Wepon", [&](Wepon_ghost* wepon) {
		PhysicsGhostObject* ghostobject = wepon->GetGhostObject();
		g_physics.ContactTest(m_characon, [&](const btCollisionObject& contactObject) {
			if (ghostobject->IsSelf(contactObject) == true) {
				//�ʂ��Ă���̂͊m�F����
				m_isDeadfrag = true;
				CSoundSource* m_se2 = new CSoundSource;
				m_se2->Init(L"Assets/sound/slash1.wav");
				m_se2->Play(false);
				m_se2->SetVolume(0.55f);//
				//�G�t�F�N�g���o���B
				g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(
					g_Effect.m_sampleEffect,
					m_position.x,
					m_position.y + 100.0f,
					m_position.z
				);
			}
			});
		return true;
		});

	if (!m_characon.IsOnGround()) {
		m_moveSpeed.y += grabity;
	}
	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_bomberAnime.Update(GameTime().GetFrameDeltaTime());
}

void Enemy_Bomber::Statekanri()
{
	// �{�}�[�̃A�j���[�V�������I���������bomber�̏�Ԃɂ���āA
	// �X�e�[�g��ς��鏈��������
	CVector3 distans = m_position - m_player->GetPosition();
		if (distans.LengthSq() < harfrenge * harfrenge) {
			if (m_state != Asigaru_attack) {
				m_state = Asigaru_attack;
			}
			else if(!m_bomberAnime.IsPlaying()){
				m_state = Asigaru_escape;
			}
		}
		else {
			m_state = Asigaru_Idle;
		}
		if (m_isDeadfrag) {
			m_state = Asigaru_dead;
		}
}

void Enemy_Bomber::Animekanri()
{
	/// <summary>
	/// �n���ꂽ�X�e�[�g����ǂ�ȃA�j���[�V�������ĂԂ������̊֐����ł���
	/// ���̎����ł�Update��1�s�����������ŏI��邯�ǁA�ォ��ύX�����\����Ȃ̂�
	/// �֐��ɂ��Ƃ��܂����B
	/// </summary>
	m_bomberAnime.Play(m_state, InterpolationTime);
}

//��]
void Enemy_Bomber::Turn()
{
	auto Rot = CMatrix::Identity();
	Rot.MakeRotationFromQuaternion(m_rotation);

	if (fabsf(m_moveSpeed.x) <= 0.001f    //fabsf�͐�Βl�Bm_movespeed.x&m_movespeedz��
		&& fabsf(m_moveSpeed.z) <= 0.001f) {//0.001�ȉ��̎��ɂ͉������Ȃ��B
		return;
	}
	else {
		float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
	}
}

//����
void Enemy_Bomber::EscapeMove()
{
	CVector3 distance = m_player->GetPosition() - m_position;
	if (distance.Length() < socialdistance) {
		CVector3 hikigimi = m_position - m_player->GetPosition();
		hikigimi.y = 0.0f;
		hikigimi.Normalize();
		hikigimi *= escapeSpeed;
		m_moveSpeed += hikigimi;
	}
}
//������Ƃ����������v�����Ȃ������̂�Turn()��������
void Enemy_Bomber::attackMove()
{
	m_moveSpeed = m_player->GetPosition() - m_position;
	Turn();
	m_moveSpeed = CVector3::Zero();
}

void Enemy_Bomber::DeathMove()
{
	if (m_isDeadfrag) {
		m_state = Asigaru_dead;
		m_bomberAnime.Play(m_state, InterpolationTime);
		if (!m_bomberAnime.IsPlaying()) {
			DeleteGO(this);
		}
	}
}

void Enemy_Bomber::Move()
{
	if (m_state == Asigaru_attack) {
		attackMove();
	}
	if (m_state == Asigaru_escape) {
		EscapeMove();
	}
}

void Enemy_Bomber::CharaconInit()
{
	//�L�����R���̏�����
	m_characon.Init(
		60.0f, //���a
		100.0f,//����
		m_position//�ʒu
	);
}