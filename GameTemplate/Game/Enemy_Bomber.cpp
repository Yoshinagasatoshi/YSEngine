#include "stdafx.h"
#include "gameObject/ysGameObjectManager.h"
#include "Enemy_Bomber.h"
#include "bom.h"
const float InterpolationTime = 0.2f;			//�A�j���[�V�����̕�Ԏ���
const float bomberSpeed = 5.0f;
const float harfrenge = 1400.0f;

Enemy_Bomber::Enemy_Bomber()
{
	//�{���G�̃��f����asigaru�̃��f���ɑ���������������̂Ȃ̂�
	//�|��鎞�Ƃ��̃A�j����asigaru�̃A�j���𗬗p���܂��B
	m_model.Init(L"Assets/modelData/asigaru_taicho.cmo");
	m_rotation = CQuaternion::Identity();
	m_scale = CVector3::One();
	m_bomAnimeClip[Asigaru_attack].Load(L"Assets/animData/asigaru_nage.tka");
	m_bomAnimeClip[Asigaru_tikazuki].Load(L"Assets/animData/asigaru_tikazuki.tka");
	m_bomAnimeClip[Asigaru_damage].Load(L"Assets/animData/asigaru_damage.tka");
	m_bomAnimeClip[Asigaru_dead].Load(L"Assets/animData/asigaru_dead.tka");
	m_bomAnimeClip[Asigaru_zyosou].Load(L"Assets/animData/asigaru_zyosou.tka");
	m_bomAnimeClip[Asigaru_attack].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_tikazuki].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_damage].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_dead].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_zyosou].SetLoopFlag(false);
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
	//m_moveSpeed = m_player->GetPosition() - m_position;
	//m_moveSpeed.Normalize();
	//m_moveSpeed *= bomberSpeed;
	if (!m_bomberAnime.IsPlaying()) {
		Statekanri();
		Animekanri();
	}
	//m_position += m_moveSpeed;
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_bomberAnime.Update(GameTime().GetFrameDeltaTime());
}

void Enemy_Bomber::Statekanri()
{

	// �{�}�[�̃A�j���[�V�������I���������bomber�̏�Ԃɂ���āA
	// �X�e�[�g��ς��鏈��������
	// �Ȃ��A�E���ꂽ���͌Ă΂�Ȃ��\��ł��B

	CVector3 distans = m_position - m_player->GetPosition();
		if (distans.LengthSq() < harfrenge * harfrenge) {
			if (m_state != Asigaru_attack) {
				m_state = Asigaru_attack;
			}
			else if(!m_bomberAnime.IsPlaying()){
				m_state = Asigaru_tikazuki;
			}
		}
		else {
			m_state = Asigaru_zyosou;
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