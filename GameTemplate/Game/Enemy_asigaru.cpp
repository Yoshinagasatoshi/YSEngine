#include "stdafx.h"
#include "Enemy_asigaru.h"
#include "gameObject/ysGameObjectManager.h"
#include "GameData.h"
#include "InGameSoundDirector.h"
#include "Wepon_ghost.h"
#include "GameCamera.h"
#include "math/Matrix.h"

const float tikadukisugi = 2.0f;
const float Timer_ZERO = 0.0f;
const float DeleteTime = 10.0f;
const float drawNearSpeed = 110.0f;
const float ViewLenght = 0.25f;						//����p�͈�
const float fastTime = 1.0f;						//���̐��l���傫���Ȃ�Ή����鎞�Ԃ������Ȃ�
const int MAX_RING_SE = 10;

/// <summary>
/// boid
/// </summary>
Enemy_asigaru::IdlePos Enemy_asigaru::m_idlePos[5];
//�R���X�g���N�^
Enemy_asigaru::Enemy_asigaru()
{
	gamedata = &GameData::GetInstans();
	m_scale = CVector3::One();
	m_sd = &InGameSoundDirector::GetInstans();
	//m_sd = g_goMgr.NewGameObject<SoundDirector>("SoundDirector");

	//asigaru�̃��f�������[�h����B
	m_model.Init(L"Assets/modelData/asigaru.cmo");
	m_model_Row.Init(L"Assets/modelData/asigaru_Rowpori.cmo");
	m_rotation = CQuaternion::Identity();
	//�A�j���[�V�������i�[��
	m_asigaruAnimeClip[Asigaru_totugeki].Load(L"Assets/animData/asigaru_totugeki.tka");
	m_asigaruAnimeClip[Asigaru_tikazuki].Load(L"Assets/animData/asigaru_tikazuki.tka");
	m_asigaruAnimeClip[Asigaru_sentou].Load(L"Assets/animData/asigaru_tikazuki.tka");
	m_asigaruAnimeClip[Asigaru_attack].Load(L"Assets/animData/asigaru_Normal_Attack.tka");
	m_asigaruAnimeClip[Asigaru_damage].Load(L"Assets/animData/asigaru_damage.tka");
	m_asigaruAnimeClip[Asigaru_dead].Load(L"Assets/animData/asigaru_dead.tka");

	m_asigaruAnimeClip[Asigaru_totugeki].SetLoopFlag(true);
	m_asigaruAnimeClip[Asigaru_tikazuki].SetLoopFlag(true);
	m_asigaruAnimeClip[Asigaru_sentou].SetLoopFlag(false);
	m_asigaruAnimeClip[Asigaru_damage].SetLoopFlag(false);
	m_asigaruAnimeClip[Asigaru_dead].SetLoopFlag(false);

	//���f���ƃA�j���f�[�^��������
	m_asigaruAnime.Init(
		m_model,
		m_asigaruAnimeClip,
		Asigaru_anim_num
	);
	//���̃R�[�h��skeleton�̖��O���������Ă���B
	m_skeleton = &m_model.GetSkeleton();
	const wchar_t* bonename[30];

	for (int i = 0; i < 20; i++) {
		bonename[i] = m_skeleton->GetBone(i)->GetName();

		if (i == 19)
		{
			bonename[i + 1] = L"end";
		}
	}

	//ghostInit();


	m_asigaruAnime.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName) {
		(void)clipName;
		m_en_Wepon = g_goMgr.NewGameObject<Wepon_ghost>("EN_Wepon");
		m_en_Wepon->SetPosition(m_position);
		m_en_Wepon->SetPlayerInfo(m_player);
		m_en_Wepon->GhostInit();
		}
	);
	//g_goMgr.InitEffekseer();
	g_Effect.m_sampleEffect = Effekseer::Effect::Create(g_Effect.m_effekseerManager, (const EFK_CHAR*)L"Assets/effect/sword.efk");
	//�G�t�F�N�g���Đ�����B
	g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(g_Effect.m_sampleEffect, 0.0f, 0.0f, 0.0f);
}
//�f�X�g���N�^
Enemy_asigaru::~Enemy_asigaru()
{
	//�G��|���������v��
	g_goMgr.Counting();
	g_goMgr.DeleteGOObject(this);
	g_goMgr.EnemyNumSubtract();
}

void Enemy_asigaru::CharaconInit()
{
	//�L�����R���̏�����
	m_characon.Init(
		60.0f, //���a
		100.0f,//����
		m_position//�ʒu
	);
}

void Enemy_asigaru::Update()
{
	if (!m_isFirstUpdate) {
		//�ŏ��̃A�b�v�f�[�g�ł͂Ȃ��B
		//�J�����Ƃ̋������v�Z����B
		CVector3 toCamera = m_position - g_camera3D.GetPosition();
		if (toCamera.LengthSq() > 4000.0f * 4000.0f) {
			return;
		}
	}
	m_isFirstUpdate = false;
	
	if (isRingSE) {
		timer -= 60.0f * GameTime().GetFrameDeltaTime();
		//�^�C�}�[��0�����ɂȂ�ƃ��Z�b�g�B�����o����悤�ɂȂ�
		if (timer < 0.0f) {
			if (m_sd->GetReturnRingNum() <= 0) {
				isRingSE = false;
				timer = 10.0f;
			}
			////�Ȃ��Ă�����ʉ���0����Ȃ������猸�炷
			//else if (!m_sd->GetReturnRingNum() == 0) {
			//	m_sd->mainasuRingnum();
			//}

		}
	}
	//Y�����̈ړ����x���o�b�N�A�b�v���Ă����B
	float ySpeed = m_moveSpeed.y;

	//�L�����R��������Ă��Ȃ�������init����
	if (!m_characonConUse) {
		m_characonConUse = true;
		CharaconInit();
	}

	if (!m_isDeadfrag) {
		PlayertoDistans();
		//����̃S�[�X�g�����������ɓ���������A���񂾂Ƃ����M���𗧂Ă�
		//�U��������Ȃ�������Ă΂��\�������邩��A�ŏ��ɌĂ΂ꂽ���΂�����Ă΂Ȃ�
		QueryGOs<Wepon_ghost>("PL_Wepon", [&](Wepon_ghost* wepon) {
			PhysicsGhostObject* ghostobject = wepon->GetGhostObject();
			g_physics.ContactTest(m_characon, [&](const btCollisionObject& contactObject) {
				if (ghostobject->IsSelf(contactObject) == true) {
					//if (m_player->IsXTrigger()) {
					//	//X��Y�œn��������ς���
					//	g_goMgr.HitStopOn(8);
					//}
					//else {
					//	//X�Ȃ̂ŏ��Ȃ߂�
					//	g_goMgr.HitStopOn(4);
					//}
					//�ʂ��Ă���̂͊m�F����
					m_isDeadfrag = true;
					//enemy�p�ɂ�
					ThisDeath();
					//�a������炷�֐�
					m_sd->RingSE_Slash();
					//�G�t�F�N�g���o���B
					g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(
						g_Effect.m_sampleEffect,
						m_position.x,
						m_position.y + 100.0f, //������ɏo��
						m_position.z
					);
				}
				});
			return true;
			});
	}

	//�ł��������Ē��n�����Ƃ��ɓ����Ă��܂��̂�
//������蓮�����Œ肷��
	if (m_isDeadfrag
		&&m_characon.IsOnGround()) {
		m_moveSpeed = CVector3::Zero();

		//�������[�h�B1�񂾂��炷�B
		if (m_Deathtimer_f <= fastTime) {
			//down�̉�
			m_sd->RingSE_Down();
		}
		m_Deathtimer_f++;

		//���񂾌�ɏ��������ă��f����������
		if (m_Deathtimer_f > DeleteTime) {
			//m_characon.RemoveRigidBoby();
			g_goMgr.EnemyNumSubtract();
			//g_goMgr.DeleteGOObject(this);
		}
	}

	//����ł��邩�ǂ����ōs���������ς��
	if (m_isDeadfrag
		&& !m_deadMoveStopper) {
		m_deadMoveStopper = true;
		DeadMove();
		ySpeed = m_moveSpeed.y;
	}
	else if (!m_isDeadfrag) {
		//��]�����B�v���C���[�̕��Ɍ�������
		Turn();
		StateJudge();
	}

	//���[���h���W�̍X�V
	m_moveSpeed.y = ySpeed + grabity;

	
	//m_position += m_moveSpeed;
	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	
	//Effekseer�J�����s���ݒ�B
	//�܂���Effeseer�̍s��^�̕ϐ��ɁA�J�����s��ƃv���W�F�N�V�����s����R�s�[�B
	m_asigaruAnime.Update(GameTime().GetFrameDeltaTime());
}
void Enemy_asigaru::Draw()
{
	//�ŏ��̃A�b�v�f�[�g�ł͂Ȃ��B
	//�J�����Ƃ̋������v�Z����B
	CVector3 toCamera = m_position - g_camera3D.GetPosition();
	if (toCamera.LengthSq() > 8000.0f * 8000.0f) {
		return;
	}

	CVector3 cameraPos = m_gameCamera->GetCameraPos();
	CVector3 Lenght = cameraPos - m_position;
	Lenght.y = 0.0f;
	//���f���̕`��
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

//�������@��芪�������������Ă���ꏊ
void Enemy_asigaru::Move()
{
	const int NomalMove = -1;
	int kakoi_num = m_player->RequestEnemyData(m_position, this);
	//���ӂ���芪�����Ȃ̂��H
	if (kakoi_num != NomalMove) {
		m_torimaki = true;
		//�������牺�͎�芪����̏���
		CVector3 kaiten = m_playerPos - m_position;
		//�������������Ɖ�]�̏�����ς���
		idlePosInit();
		m_moveSpeed = m_idlePos[kakoi_num].idlePos - m_position;
		//�ړI�n�Ƌ��������܂�ɂ��߂��Ƃ��͓����Ȃ�
		if (m_moveSpeed.Length() < 7.0f) {
			m_moveSpeed = CVector3::Zero();
		}
		else {
			m_moveSpeed.Normalize();
			m_moveSpeed *= 30.0f;//30�{
		}
	float angle = atan2(kaiten.x,kaiten.z);
	m_rotation.SetRotation(CVector3::AxisY(), angle);
	return;
	}
	else {
		//��芪���t���O�I�t
		m_torimaki = false;
	}
}

void Enemy_asigaru::Turn()
{
	auto Rot = CMatrix::Identity();
	Rot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = Rot.m[2][0];
	m_forward.y = Rot.m[2][1];
	m_forward.z = Rot.m[2][2];

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

	CVector3 aho = m_playerPos - m_position;
	if (aho.Length() < 300.0f) {
		aho = aho;
	}
	else {
		aho.y = 0.0f;
		aho.Normalize();
		aho *= 300.0f;
	}
	//����6�Ζ��`
	m_idlePos[0].idlePos = m_playerPos + CVector3(0.0f,0.0f,-150.0f);		//��ԉ��ɂ���Z���^�[
	m_idlePos[1].idlePos = m_playerPos + CVector3(100.0f, 0.0f, -100.0f);	//���ɂ����̗�
	m_idlePos[2].idlePos = m_playerPos + CVector3(150.0f, 0.0f, -30.0f);	//�[����1
	m_idlePos[3].idlePos = m_playerPos + CVector3(-150.0f, 0.0f, -30.0f);	//�[����2
	m_idlePos[4].idlePos = m_playerPos + CVector3(-100.0f, 0.0f, -100.0f);	//���ɂ����̗�2

}

//�����ɂ�锻�菈��
void Enemy_asigaru::StateJudge()
{
	//�v���C���[�����񂾂Ƃ��ɂ��}���Ă��܂�
	//�`�`Range 
	/// <summary>
	/// -282497216
	/// -87308032
	/// </summary>
	CVector3 kyori = m_player->GetPosition() - m_position;
	//y������0�ɂ���B
	kyori.y = 0.0f;

	m_player_isdead = m_player->GetPlayerDead();
	//�X�e�[�g���Ƃ̏���������
	switch (m_asigaruState)
	{
	case Asigaru_attack:
		Move();
		m_moveSpeed = CVector3::Zero();
		if (!m_asigaruAnime.IsPlaying()) {
			m_asigaruState = Asigaru_sentou;
		}
		break;
	case Asigaru_sentou:
		if (m_isAttack) {
			m_isAttack = false;
		}	
		if (kyori.LengthSq() > BattleRange) {
			m_asigaruState = Asigaru_tikazuki;
			m_frameTimer = Timer_ZERO;
			AttackframeNum();
		}
		//sentou�̏���
		m_moveSpeed = CVector3::Zero();
		m_frameTimer += 1.0f;

		if (m_frameTimer >= m_kougekiframenum)
		{
			m_frameTimer = Timer_ZERO;
			AttackframeNum();

			m_asigaruState = Asigaru_attack;
		}
		Move();
		break;
	case Asigaru_tikazuki:
		if (kyori.LengthSq() < BattleRange) {
			kyori.Normalize();
			float angle = acosf(kyori.Dot(m_forward));

			//����p�����Ă�H
			if (fabsf(angle) < PI * ViewLenght) {
				m_asigaruState = Asigaru_sentou;
			}
		}
		else if (kyori.LengthSq() > VigilanceRange)
		{
			m_asigaruState = Asigaru_totugeki;
		}
		//tikazuki�̏���
		kyori.Normalize();
		m_moveSpeed = kyori * drawNearSpeed * 2.0f;
		break;


	case Asigaru_totugeki:
		if (kyori.LengthSq() < VigilanceRange) {
			//����p�����Ă�H
			kyori.Normalize();
			float angle = acosf(kyori.Dot(m_forward));
			if (fabsf(angle) < PI * ViewLenght) {
				m_asigaruState = Asigaru_tikazuki;
			}
		}
		kyori.Normalize();
		m_moveSpeed = kyori * drawNearSpeed * 0.1f;
		//���̏���
		//m_moveSpeed = CVector3::Zero();
		CQuaternion ADDrot = CQuaternion::Identity();
		ADDrot.SetRotationDeg(CVector3::AxisY(), 0.1f);
		m_rotation.Multiply(ADDrot);
		break;
	}
	m_asigaruAnime.Play(m_asigaruState, 0.2f);
}

/// <summary>
/// ���񂾂Ƃ��ɌĂ΂�閼�O�̒ʂ�̊֐�
/// </summary>
void Enemy_asigaru::DeadMove()
{
	//�����蔻��Ȃǂ̊J��
	//m_ghostObject.Release();
	//�v���C���[����H������Z�̈З͂ɉ�����
	//������ԗʂ����߂�
	float power;
	power = m_player->GetBlowOffPower();
	m_moveSpeed.y += power;
	CVector3 pushPower;
	pushPower = m_forward * power;
	m_moveSpeed -= pushPower;
	m_asigaruAnime.Play(Asigaru_dead,0.1f);
	//���񂾃A�j���[�V�����͑����Đ�����
	m_asigaruAnime.Update(GameTime().GetFrameDeltaTime());
}
