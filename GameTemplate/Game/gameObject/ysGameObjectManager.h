/*
  ysGameObject�̃}�l�[�W���[
*/
#pragma once
#include "gameObject/ysGameObject.h"
namespace ysEngine{
	class ysGameObjectManager
	{
	public:
		ysGameObjectManager();
		~ysGameObjectManager();
	private:
		//���s�B
		void Execute();
		//�������B
		void Init(int gameObjectPropMax);
		
	private:
		void Start();
		void Update();
		void PostUpdate();
		void PreUpdate();
	private:
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
};
