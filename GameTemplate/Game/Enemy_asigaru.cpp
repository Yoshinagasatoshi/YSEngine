#include "stdafx.h"
#include "Enemy_asigaru.h"
#include "gameObject/ysGameObjectManager.h"
#include "GameData.h"
#include "Wepon_ghost.h"
#include "GameCamera.h"

const float BattleRange = 180.0f * 180.0f;
const float VililanceRange = 600.0f * 600.0f;
const float grabity = -9.8f;
const float tikadukisugi = 2.0f;
const float Timer_ZERO = 0.0f;
/// <summary>
/// boid
/// </summary>
Enemy_asigaru::IdlePos Enemy_asigaru::m_idlePos[5];
//�R���X�g���N�^
Enemy_asigaru::Enemy_asigaru()
{
	gamedata = &GameData::GetInstans();
	m_scale = CVector3::One();

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

	m_asigaruAnime.Init(
		m_model,
		m_asigaruAnimeClip,
		Asigaru_anim_num
	);

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
		}
	);
}
//�f�X�g���N�^
Enemy_asigaru::~Enemy_asigaru()
{
	g_goMgr.DeleteGOObject(this);
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
	//Y�����̈ړ����x���o�b�N�A�b�v���Ă����B
	float ySpeed = m_moveSpeed.y;

	//�L�����R��������Ă��Ȃ�������init����
	if (!m_characonContain) {
		m_characonContain = true;
		CharaconInit();
	}

	//�ł��������Ē��n�����Ƃ��ɓ����Ă��܂��̂�
	//������蓮�����Œ肷��
	if (m_isDeadfrag
		&&m_characon.IsOnGround()){
		m_moveSpeed = CVector3::Zero();
		m_Deathtimer_f++;
		//���񂾌�ɏ��������ă��f����������
		if (m_Deathtimer_f > 10.0f) {
			//m_characon.RemoveRigidBoby();
			DeleteGO(this);
		}
	}

	//����ł��邩�ǂ����ōs���������ς��
	if (m_isDeadfrag
		&&!m_deadMoveStopper) {
		m_deadMoveStopper = true;
		DeadMove();
		ySpeed = m_moveSpeed.y;
	}
	else if(!m_isDeadfrag){
		StateJudge();

		//��]�����B�v���C���[�̕��Ɍ�������
		Turn();
	}


	//����̃S�[�X�g�����������ɓ���������A���񂾂Ƃ����M���𗧂Ă�
	QueryGOs<Wepon_ghost>("PL_Wepon", [&](Wepon_ghost* wepon) {
		PhysicsGhostObject* ghostobject = wepon->GetGhostObject();
		g_physics.ContactTest(m_characon, [&](const btCollisionObject& contactObject) {
		if (ghostobject->IsSelf(contactObject) == true) {
			//�ʂ��Ă���̂͊m�F����
			m_isDeadfrag = true;
		}
		});
		return true;
	});

	//���[���h���W�̍X�V
	m_moveSpeed.y = ySpeed + grabity;

	//m_position += m_moveSpeed;
	m_position = m_characon.Execute(1.0f / 30.0f, m_moveSpeed);
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_model_Row.UpdateWorldMatrix(m_position, m_rotation, m_scale);

	//m_ghostObject.SetPosition(m_position);
	m_asigaruAnime.Update(1.0f / 30.0f);
}
void Enemy_asigaru::Draw()
{
	CVector3 cameraPos = m_gameCamera->GetCameraPos();
	CVector3 Lenght = cameraPos - m_position;
	Lenght.y = 0.0f;
	if (Lenght.LengthSq() < 3000.0f * 3000.0f) {
		//���f���̕`��
		m_model.Draw(
			g_camera3D.GetViewMatrix(),
			g_camera3D.GetProjectionMatrix()
		);
	}
	else {
		m_model_Row.Draw(
			g_camera3D.GetViewMatrix(),
			g_camera3D.GetProjectionMatrix()
		);
	}
}

//�������@��芪�������������Ă���ꏊ
void Enemy_asigaru::Move()
{
	const int NomalMove = -1;
	int kakoi_num = m_player->RequestEnemyData(m_position, this);
	int a = kakoi_num;
	//���ӂ���芪�����Ȃ̂��H
	if (kakoi_num != NomalMove) {
		//�������牺�͎�芪����̏���
		CVector3 kaiten = m_playerPos - m_position;
		//�������������Ɖ�]�̏�����ς���
		idlePosInit();
		m_moveSpeed = m_idlePos[kakoi_num].idlePos - m_position;
		m_moveSpeed.Normalize();
		m_moveSpeed *= 30.0f;//30�{

	float angle = atan2(kaiten.x,kaiten.z);
	m_rotation.SetRotation(CVector3::AxisY(), angle);
	return;
	}
	else {
		CVector3 kaiten = m_playerPos - m_position;
		float angle = atan2(kaiten.x, kaiten.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);

		if (kaiten.LengthSq() < BattleRange) {
			int i = 0;
			i = rand() % 5;
			CVector3 panko = CVector3::Zero();
			panko = m_idlePos[i].idlePos - m_position;
			panko.Normalize();
			m_moveSpeed = panko * 10.0f;
		}

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

	m_idlePos[0].idlePos = m_playerPos + CVector3{150.0f,0.0f,0.0f};
	m_idlePos[1].idlePos = m_playerPos + CVector3{ -150.0f,0.0f,0.0f };
	m_idlePos[2].idlePos = m_playerPos + CVector3{ 100.0f,0.0f,100.0f };
	m_idlePos[3].idlePos = m_playerPos + CVector3{ -100.0f,0.0f,100.0f };
	m_idlePos[4].idlePos = m_playerPos + CVector3{ 0.0f,0.0f,-150.0f };
}

//�����ɂ�锻�菈��
void Enemy_asigaru::StateJudge()
{
	//�`�`Range
	m_playerPos = m_player->GetPosition();
	CVector3 kyori = m_playerPos - m_position;
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
			m_kougekiframenum = AttackframeNum();
		}
		//sentou�̏���
		m_moveSpeed = CVector3::Zero();
		m_frameTimer += 1.0f;

		if (m_frameTimer >= m_kougekiframenum)
		{
			m_frameTimer = Timer_ZERO;
			m_kougekiframenum = AttackframeNum();
			m_player->PlayerDamage();
			m_asigaruState = Asigaru_attack;
		}
		Move();
		break;
	case Asigaru_tikazuki:
		if (kyori.LengthSq() < BattleRange) {
			kyori.Normalize();
			float angle = acosf(kyori.Dot(m_forward));

			//����p�����Ă�H
			if (fabsf(angle) < PI * 0.25f) {
				m_asigaruState = Asigaru_sentou;
			}
		}
		else if (kyori.LengthSq() > VililanceRange)
		{
			m_asigaruState = Asigaru_totugeki;
		}
		//tikazuki�̏���
		kyori.Normalize();
		m_moveSpeed = kyori * 50.0f;
		//Move();
		break;


	case Asigaru_totugeki:
		if (kyori.LengthSq() < VililanceRange) {
			//����p�����Ă�H
			kyori.Normalize();
			float angle = acosf(kyori.Dot(m_forward));
			if (fabsf(angle) < PI * 0.25f) {
				m_asigaruState = Asigaru_tikazuki;
			}
		}
		//totugeki�̏���
		m_moveSpeed = CVector3::Zero();
		CQuaternion ADDrot = CQuaternion::Identity();
		ADDrot.SetRotationDeg(CVector3::AxisY(), 2.0f);
		m_rotation.Multiply(ADDrot);
		break;
	}
		m_asigaruAnime.Play(m_asigaruState, 0.2f);
}

//�G�l�~�[�̃S�[�X�g�B���͎g���Ă��Ȃ��̂ŃR�����g�A�E�g���Ă���
//void Enemy_asigaru::ghostInit()
//{
//	m_ghostObject.CreateBox(
//		m_position,
//		m_rotation,
//		{
//		m_scale.x * 50.0f,
//		m_scale.y * 200.0f,
//		m_scale.z * 50.0f 
//		}
//	);
//}

/// <summary>
/// ���񂾂Ƃ��ɌĂ΂�閼�O�̒ʂ�̊֐�
/// </summary>
void Enemy_asigaru::DeadMove()
{
	//�����蔻��Ȃǂ̊J��
	//m_ghostObject.Release();
	//�v���C���[����H������Z�̈З͂ɉ�����
	//������΂��ʂ����߂�
	float power;
	power = m_player->GetBlowOffPower();
	m_moveSpeed.y += power;
	CVector3 a;
	a = m_forward * power / 4;
	m_moveSpeed -= a;
	m_asigaruAnime.Play(Asigaru_dead,0.1f);
	m_asigaruAnime.Update(1/10);
}