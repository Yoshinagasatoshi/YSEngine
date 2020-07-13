#include "stdafx.h"
#include "system/system.h"
#include "level/Level.h"
#include "Title.h"
#include "gameObject/ysGameObjectManager.h"
#include "SoundDirector.h"
#include "Fade.h"

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
	g_camera3D.SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 200.0f, 0.0f });

	g_camera2D.SetWidth(FRAME_BUFFER_W);
	g_camera2D.SetHeight(FRAME_BUFFER_H);
	g_camera2D.SetPosition({ 0.0f, 0.0f, -10.0f });
	g_camera2D.SetTarget(CVector3::Zero());
	g_camera3D.SetNear(10.0f);
	g_camera3D.SetFar(100000.0f);

	//g_camera2D.InitCamera();
	//�^�C�g��
	Fade::Getinstance();

	Title* title = g_goMgr.NewGameObject<Title>("Title");

	SoundDirector* Dsound = g_goMgr.NewGameObject<SoundDirector>("SoundDirector");

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

		//g_goMgr.Render();
		//g_goMgr.Draw();
		//g_goMgr.DrawNoAlphaBlend();
		g_graphicsEngine->Render();
		Fade::Getinstance().Update();
		Fade::Getinstance().PostDraw();
		
		Dsound->Update();

		//�J�����̍X�V�B
		g_camera3D.Update();
		g_camera2D.Update();
		//�`��I���B
		g_graphicsEngine->EndRender();
		//1�t���[���̎��Ԍv���I���B
		sw.Stop();
		//1�t���[���̎��Ԃ��L�����Ă����B
		GameTime().PushFrameDeltaTime(sw.GetElapsed());
	}
}
