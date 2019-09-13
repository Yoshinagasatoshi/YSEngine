/*
  ysGameObject�̃}�l�[�W���[
*/
#pragma once
#include "gameObject/ysGameObject.h"
	class ysGameObjectManager
	{
	public:
		ysGameObjectManager() {}
		~ysGameObjectManager() {}
		//�A�b�v�f�[�g
		void Update();
		//newGO�E���ی��
		template <class T>
		T* NewGameObject()
		{
			T* newObj = new T;
			IGameObjectList.push_back(newObj);
			return newObj;
		}
		//�f���[�g�E���ی��
		void DeleteGOObject(IGameObject* go)
		{
			//���X�g���猟�����āA����������폜����B
			for (auto it = IGameObjectList.begin();
				it != IGameObjectList.end();
				it++) {
				if ((*it) == go) {
					//�폜���N�G�X�g�𑗂�B
					go->RequestDelete();
				}
			}
		}
		//���s�B
		void Execute();
	private:
		
		//�������B
		void Init(int gameObjectPropMax);
		
	private:
		void Start();
		void Draw();
		void PostUpdate();
		void PreUpdate();
		
		
	private:
		//���ƔŊi�[��
		std::vector<IGameObject*> IGameObjectList; //�Q�[���I�u�W�F�N�g�̃��X�g
		//�z��̊i�[��
		typedef std::vector<IGameObject*> GameObjectList;
		//List�͂��Ȃ�
		/// <summary>
		/// NewGO�p�̃��X�g�BtkEngine�ƈႢ�D��x�͂Ȃ��B
		/// </summary>
		std::vector<GameObjectList> m_gameObjectListArray;
		/// <summary>
		/// DeleteGO�p�̃��X�g
		/// </summary>
		std::vector<GameObjectList> m_deleteObjectArray[2];
	};
	//�O���A�N�Z�X������̂ŁAextern�錾������B
	extern ysGameObjectManager g_goMgr;