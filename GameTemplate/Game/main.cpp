#include "stdafx.h"
#include "system/system.h"
#include "level/Level.h"
#include "Title.h"
#include "gameObject/ysGameObjectManager.h"
#include "InGameSoundDirector.h"
#include "Fade.h"

const float NEARVALUE	 = 10.0f;		//�J�����̋ߕ���
const float FARVALUE	 = 100000.0f;	//�J�����̉�����
 
const CVector3 CAMERA_POSITION = { 0.0f, 100.0f, 300.0f };
const CVector3 CAMERA_TARGET = { 0.0f, 200.0f, 0.0f };

const CVector3 CAMERA_POSITION_2D = { 0.0f, 0.0f, -10.0f };
const CVector3 CAMERA_TARGET_2D = CVector3::Zero();

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");
	
	//�T�E���h�̓Q�[������c
	//�T�E���h�G���W����������
	g_goMgr.InitEffekseer();
	
	//�J�������������B
	g_camera3D.InitCamera();
	g_camera3D.SetPosition(CAMERA_POSITION);
	g_camera3D.SetTarget(CAMERA_TARGET);

	g_camera2D.SetWidth(FRAME_BUFFER_W);
	g_camera2D.SetHeight(FRAME_BUFFER_H);
	g_camera2D.SetPosition(CAMERA_POSITION_2D);
	g_camera2D.SetTarget(CAMERA_TARGET_2D);
	g_camera3D.SetNear(NEARVALUE);
	g_camera3D.SetFar(FARVALUE);

	//g_camera2D.InitCamera();
	//�^�C�g��
	Fade::Getinstance();

	g_goMgr.NewGameObject<Title>("Title");

	InGameSoundDirector::GetInstans();
	InGameSoundDirector::GetInstans().UpdateOn();

	CStopwatch sw;
	//�Q�[�����[�v�B
	while (DispatchWindowMessage() == true)
	{
		//1�t���[���̎��Ԍv�����J�n�B
		sw.Start();


		//�Q�[���p�b�h�̍X�V�B	
		for (auto& pad : g_pad) {
			pad.Update();
		}
		
		//�����G���W���̍X�V�B
		g_physics.Update();

		g_goMgr.Update();

		//g_graphicsEngine->RenderToShadowMap();
		g_graphicsEngine->Render();

		bool UpdateMode = InGameSoundDirector::GetInstans().GetisUpdateMode();
		if (UpdateMode) {
			InGameSoundDirector::GetInstans().Update();
		}

		//g_goMgr.Render();
		//g_goMgr.Draw();
		//g_goMgr.DrawNoAlphaBlend();
		
		//�J�����̍X�V�B
		g_camera3D.Update();
		g_camera2D.Update();
		Fade::Getinstance().Update();
		Fade::Getinstance().PostDraw();
	

		g_goMgr.Delete();


		//�`��I���B
		g_graphicsEngine->EndRender();
		//1�t���[���̎��Ԍv���I���B
		sw.Stop();
	
		//1�t���[���̎��Ԃ��L�����Ă����B
		GameTime().PushFrameDeltaTime(sw.GetElapsed());
	}
}
