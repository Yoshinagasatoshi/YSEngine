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
		//�f���[�g
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
		//�z��
		template <class T>
		T* FindGOObject(IGameObject* go)
		{
			//�z��J�E���g
			int hairetunumber = 0;
			//���X�g���猟�����āA����������Ԃ�
			for (auto it = IGameObjectList.begin();
				it != IGameObjectList.end();
				it++) {
				i++;
				if ((*it) == go) {
					T* p = dynamic_cast<T*>(go);
					if (p == nullptr && enableErrorMessage == true) {
						//�^�ϊ��Ɏ��s
						MessageBox("�^�ϊ��Ɏ��s���܂���");
					}
					return p;
				}
			}
		}
		//findGO?�搶�̂���܂˂�����
		template<class T>
		T* FindGameObject(const char* objactName, bool enableErrorMessage)
		{
			unsigned int nameKey = CUtil::MakeHash(ObjectName);
			for (auto go : goList) {
				if (go->m_nameKey == nameKey) {
					//����
					T* p = dynamic_cast<T*>(go);
					if (p == nullptr && enableErrorMessage == true) {
						//�^�ϊ��Ɏ��s
						MessageBox("�^�ϊ��Ɏ��s���܂���");
					}
					return p;
				}
			}
		}
		//���s�BExecute����Draw��Update�ɕ���
		void Execute();
		void Updater();
		void Draw();
	private:
		//�������B
		void Init(int gameObjectPropMax);
		
	private:
		void Start();
		//void Draw();
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