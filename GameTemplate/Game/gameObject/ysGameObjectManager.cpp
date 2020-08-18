#include "stdafx.h"
#include "ysGameObjectManager.h"
//ysGameObjectManager�N���X�̃C���X�^���X�B
ysGameObjectManager g_goMgr;
ysGameObjectManager::EffectTool g_Effect;
	void ysGameObjectManager::Start()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->StartWrapper();
			}
		}
	}

	//void ysGameObjectManager::Update()
	//{
	//	for (GameObjectList objList : m_gameObjectListArray) {
	//		for (IGameObject* obj : objList) {
	//			obj->UpdateWrapper();
	//		}
	//	}
	//}

	void ysGameObjectManager::PostUpdate()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PostUpdateWrapper();
			}
		}
	}

	void ysGameObjectManager::PreUpdate()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PreUpdateWrapper();
			}
		}
	}

	//void ysGameObjectManager::Draw()
	//{
	//	for (GameObjectList objList : m_gameObjectListArray) {
	//		for (IGameObject* obj : objList) {
	//			obj->DrawWrapper();
	//		}
	//	}
	//}

	void ysGameObjectManager::Update()
	{
		//�q�b�g�X�g�b�v���Ă��Ȃ���Ζ���A�b�v�f�[�g
		if (!m_isHitStopFrag) {
			for (auto& go : IGameObjectList) {
				go->Update();
			}
		}
		//�q�b�g�X�g�b�v�����Ă�����Update�͉񂳂Ȃ�
		if (m_isHitStopFrag) {
			HitStopTimer++;
			//���Ȃ̂Ő��l�⑫���͓̂K���B���܂������΂����Ƃ������̂ɂ���
			if (HitStopTimer > m_HitStopCount) {
				//Update�ł���悤��
				m_isHitStopFrag = false;
				HitStopTimer = 0;
			}
		}
		
		Effekseer::Matrix44 efCameraMat;
		g_camera3D.GetViewMatrix().CopyTo(efCameraMat);
		Effekseer::Matrix44 efProjMat;
		g_camera3D.GetProjectionMatrix().CopyTo(efProjMat);
		//�J�����s��ƃv���W�F�N�V�����s���ݒ�B
		g_Effect.m_effekseerRenderer->SetCameraMatrix(efCameraMat);
		g_Effect.m_effekseerRenderer->SetProjectionMatrix(efProjMat);
		g_Effect.m_effekseerManager->Update();
	}

	void ysGameObjectManager::Draw()
	{
		for (auto& go : IGameObjectList) {
			go->Draw();
		}
		
	}

	//void ysGameObjectManager::DrawNoAlphaBlend()
	//{
	//	for (auto go : IGameObjectList) {
	//		go->DrawNoAlphaBlend();
	//	}
	//}

	void ysGameObjectManager::PostRender()
	{
		for (auto go : IGameObjectList) {
			go->PostRender();
		}
	}
	void ysGameObjectManager::Render()
	{
		for (auto go : IGameObjectList) {
			go->Render();
		}
	}

	void ysGameObjectManager::Delete()
	{
		//�S�ẴQ�[���I�u�W�F�N�g��1�t���[�����̏������I����Ă���A�폜����B
		for (auto it = IGameObjectList.begin(); it != IGameObjectList.end();) {
			if ((*it)->isRequestDelete()) {
				//�폜���N�G�X�g���󂯂Ă���̂ō폜
				delete* it;
				it = IGameObjectList.erase(it);
			}
			else {
				//���N�G�X�g���󂯂Ă��Ȃ�����
				it++;
			}
		}
	}

	void ysGameObjectManager::Execute()
	{
	//	//�o�^����Ă���Q�[���I�u�W�F�N�g�̍X�V�������ĂԁB
	//	for (auto go : IGameObjectList) {
	//		go->Update();
	//		go->Draw();
	//	}
	}

	void ysGameObjectManager::Init(int gameObjectPrioMax)
	{
	//	IGameObjectList.reserve(gameObjectPrioMax);
		m_gameObjectListArray.resize(gameObjectPrioMax);
		m_deleteObjectArray[0].resize(gameObjectPrioMax);
		m_deleteObjectArray[1].resize(gameObjectPrioMax);
	}

	void ysGameObjectManager::InitEffekseer()
	{
		//�����_���[��������
		g_Effect.m_effekseerRenderer = EffekseerRendererDX11::Renderer::Create(
			g_graphicsEngine->GetD3DDevice(),
			g_graphicsEngine->GetD3DDeviceContext(),
			20000									//�|���̍ő吔�E
		);
		//�G�t�F�N�g�}�l�[�W����������
		g_Effect.m_effekseerManager = Effekseer::Manager::Create(10000);

		//�`��p�C���X�^���X����`��@�\��ݒ�
		g_Effect.m_effekseerManager->SetSpriteRenderer(g_Effect.m_effekseerRenderer->CreateSpriteRenderer());
		g_Effect.m_effekseerManager->SetRibbonRenderer(g_Effect.m_effekseerRenderer->CreateRibbonRenderer());
		g_Effect.m_effekseerManager->SetRingRenderer(g_Effect.m_effekseerRenderer->CreateRingRenderer());
		g_Effect.m_effekseerManager->SetTrackRenderer(g_Effect.m_effekseerRenderer->CreateTrackRenderer());
		g_Effect.m_effekseerManager->SetModelRenderer(g_Effect.m_effekseerRenderer->CreateModelRenderer());

		//�C���X�^���X����e�N�X�`���̓ǂݍ��݋@�\��ݒ�
		// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
		g_Effect.m_effekseerManager->SetTextureLoader(g_Effect.m_effekseerRenderer->CreateTextureLoader());
		g_Effect.m_effekseerManager->SetModelLoader(g_Effect.m_effekseerRenderer->CreateModelLoader());
	}