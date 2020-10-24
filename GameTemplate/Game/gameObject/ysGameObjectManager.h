/*
  ysGameObject�̃}�l�[�W���[
*/
#pragma once
#include "gameObject/ysGameObject.h"
#include "util/Util.h"
	class ysGameObjectManager
	{
	public:
		ysGameObjectManager() {}
		~ysGameObjectManager() {}
		
		struct EffectTool
		{
			//Effekseer�}�l�[�W���Ǘ��B
			//�C���X�^���X��1��
			Effekseer::Manager* m_effekseerManager = nullptr;
			EffekseerRenderer::Renderer* m_effekseerRenderer = nullptr;

			//�G�t�F�N�g
			Effekseer::Effect* m_sampleEffect = nullptr;
			Effekseer::Handle m_playEffectHandle = -1;
		};

		/// <summary>
		/// �C�t�F�N�T�[������B
		/// </summary>
		void InitEffekseer();
		/// <summary>
		/// �A�b�v�f�[�g
		/// </summary>
		void Update();
		void XorY(int X);
		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̖��O�L�[���쐬
		/// </summary>
		static unsigned int MakeGameObjectNameKey(const char* objectName)
		{
			static const unsigned int defaultNameKey = Util::MakeHash("Undefined");
			unsigned int hash;
			if (objectName == nullptr) {
				hash = defaultNameKey;
			}
			else {
				hash = Util::MakeHash(objectName);
			}
			return hash;
		}

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�����N���X
		/// NewGO()���Ă�ł����ʓI�ɂ͂��̊֐���ʂ�
		/// </summary>
		template <class T>
		T* NewGameObject(const char* objectName)
		{
			T* newObj = new T;
			IGameObjectList.push_back(newObj);
			unsigned int hash = MakeGameObjectNameKey(objectName);
			newObj->m_nameKey = hash;
			return newObj;
		}

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�������֐�
		/// DeleteGO()���Ă�ł����̊֐���ʂ邱�ƂɂȂ�
		/// IGameObject���p�����Ă���N���X��1���������Ƃ��͂�������g��
		/// �������������Ƃ���DeleteGOObject"s"�̕����g���B
		/// </summary>
		/// <param name="go"></param>
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
		/// <summary>
		/// DeleteGOOnject�̕����`
		/// ���O���������āA���̖��O�ɊY������C���X�^���X��S��
		/// �f���[�g����B
		/// </summary>
		/// <param name="go"></param>
		/// <param name="objname"></param>
		void DeleteGOObjects(IGameObject* go, const char * objname)
		{
			//���X�g���猟������
			for (auto it = IGameObjectList.begin();
				it != IGameObjectList.end();
				it++) {
				if ((*it) == go) {
					//�폜���N�G�X�g�𑗂�B

				}
			}
		}

		/// <summary>
		/// �Q�[���I�u�W�F�N�g��(�ȉ�GO��)�̌����B�d��
		/// GO���̌�����1�񂾂��s�������Ȃ炱����Ă�
		/// �������O��GO���𕡐��Ăт����ꍇ��
		/// FindGameObject"s"���ĂԁB
		/// </summary>
		template<class T>
		T* FindGameObject(const char* objectName, bool enableErrorMessage = true)
		{
			unsigned int nameKey = Util::MakeHash(objectName);
			for (auto goList : IGameObjectList) {
				if (goList->m_nameKey == nameKey) {
					//����
					T* p = dynamic_cast<T*>(goList);
					if (p == nullptr && enableErrorMessage == true) {
						//�^�ϊ��Ɏ��s
						//MessageBox("�^�ϊ��Ɏ��s���܂���");
						abort();
					}
					return p;
				}
			}
		}

		/// <summary>
		/// �Q�[���I�u�W�F�N�g(�ȉ�GO)�𕡐������������Ƃ��Ɏg���֐�
		/// </summary>
		template<class T>
		void FindGameObjects(const char* objectName, std::function<bool(T* go)> func)
		{
			unsigned int nameKey = Util::MakeHash(objectName);
			for (auto goList : IGameObjectList) {
				if (goList->m_nameKey == nameKey) {
					//�������B
					T* p = dynamic_cast<T*>(goList);
					if (func(p) == false) {
						//�N�G�����f�B
						return;
					}
				}
			}
		}

		/// <summary>
		/// �������̃J�E���g�B�Ă΂ꂽ��1�������
		/// </summary>
		void Counting()
		{
			Count++;
		}
		/// <summary>
		/// ���̓������Ă��邩���m�F����֐�
		/// </summary>
		/// <returns></returns>
		int GetCount() const
		{
			return Count;
		}
		/// <summary>
		/// �������̃J�E���g��0�ɂ���N���X�B
		/// �Q�[����2���ڈȍ~�ɌĂ΂�邱�Ƃ�z�肵�Ă��܂��B
		/// </summary>
		void ResetCount()
		{
			Count = 0;
		}

		/// <summary>
		/// �G�l�~�[�̐������J�E���g����
		/// </summary>
		void EnemyCounting()
		{
			m_enemyCount++;
		}
		/// <summary>
		/// �G�l�~�[�����񂾂�J�E���g�����炷
		/// </summary>
		void EnemyNumSubtract()
		{
			m_enemyCount--;
		}
		///���݂̃G�l�~�[�̐���Ԃ�
		int GetEnemyNum() const
		{
			return  m_enemyCount;
		}
		///�G�l�~�[�̃J�E���g�����Z�b�g����
		void EnemyNumResetCount()
		{
			//?
			m_enemyCount = 0;
		}
		/// <summary>
		/// �Ă΂ꂽ��q�b�g�X�g�b�v���I����
		/// </summary>
		void HitStopOn(int i)
		{
			m_isHitStopFrag = true;
			m_HitStopCount = i;
		}

		///�Ă΂ꂽ��q�b�g�X�g�b�v���I�t��
	/*	void HitStopOff()
		{
			m_isHitStopFrag = false;
		}*/
		//
		/// <summary>
		/// ���݂̃q�b�g�X�g�b�v�̏�Ԃ�Ԃ�
		/// </summary>
		/// <returns></returns>
		bool IsHitStop() const 
		{
			return m_isHitStopFrag;
		}

		/// <summary>
		/// ���o�Q�[�W���ǂꂾ�����邩��Ԃ��֐��B
		/// </summary>
		const float&GetMusouGaugeValue() const
		{
			return m_musouGauge;
		}
		/// <summary>
		/// ���o�Q�[�W���ǂꂾ���������������󂯎��֐�
		/// ������-�����ꂽ�猸�点��B
		/// </summary>
		void AddMusouGauge(float Value)
		{
			m_musouGauge += Value;
			//�����A���o�Q�[�W�̍ő�l�𒴂������Ȃ�
			if (m_musouGauge > 450.0f)
			{
				m_musouGauge = 450.0f;
			}
			//�����A���o�Q�[�W�̍Œ�l�𒴂������Ȃ�
			if (m_musouGauge < 0.0f)
			{
				m_musouGauge = 0.0f;
			}
		}

		/// <summary>
		/// ���o�Z���łĂ�󋵂��ǂ�����Ԃ�
		/// </summary>
		bool isMusouSpecial()const
		{
			return m_isMusouSpecial;
		}

		/// <summary>
		/// ���o�Z���łĂ鍇�}
		/// </summary>
		void SetMusou_Uteru()
		{
			m_isMusouSpecial = true;
		}

		/// <summary>
		/// ���o�Z���łĂȂ����}
		/// </summary>
		void SetMusou_Utenai()
		{
			m_isMusouSpecial = false;
		}

		/// <summary>
		/// �Ă΂ꂽ�疳�o�Q�[�W�ʂ����Z�b�g����B
		/// �Q�[����2���ڈȍ~�ɂ�΂��\��B
		/// addMusouGauge�ł���p�͂ł���
		/// </summary>
		void ResetMusouGauge()
		{
			m_musouGauge = 0.0f;
		}

		//���s�BExecute����Draw��Update�ɕ���
		void Execute();
		//void Update();
		void Draw();

		//void DrawNoAlphaBlend();
		//2d���ǉ�
		void Render();
		void Delete();
		void PostDraw()
		{
			for (auto go : IGameObjectList) {
				go->PostDraw();
			}
		}
		//�z�񐔂���肽���̂Ń��X�g��Ԃ��悤�ɂ���
		std::list<IGameObject*> GetIGameObjectList()
		{
			return IGameObjectList;
		}
	private:
		//�������B
		void Init(int gameObjectPropMax);

		void Start();
		//void Draw();
		void PostUpdate();
		void PostRender();

		void PreUpdate();

	private:
		//�z��̐����ϓ�������J�E���g����ϐ�����肽�������̂Œǉ�
		int Count = 0;
		//COunt�Ƃقړ����A�J�E���g�Ƃ͕ʘg�Ő����������̂�����Ύg���B���O�ς��邩��
		int m_enemyCount = 0;
		//�N���A�ʒm�A�͂��悤�ɂ��悤��
		bool isCrear = false;
		//�Q�[���I�[�o�[�ʒm
		bool isGameOver = false;
		//�q�b�g�X�g�b�v�������邩�ǂ����A���ꂪtrue�ɂȂ��Update()���~�܂�悤�ɁB
		bool m_isHitStopFrag = false;
		//X�ɂ����������̃q�b�g�X�g�b�v������̂�
		bool m_isHitXStopFrag = false;
		//�q�b�g�X�g�b�v�^�C�}�[;
		int HitStopTimer = 0;
		//�q�b�g�X�g�b�v�̃J�E���g
		int m_HitStopCount = 0;
		//���o�Q�[�W�̗ʂ�~�ς��Ă���ϐ�
		float m_musouGauge;
		//���o�Z���łĂ邩�ǂ����𔻒f����ϐ�
		bool m_isMusouSpecial = false;
	private:
		//���ƔŊi�[��
		std::list<IGameObject*> IGameObjectList; //�Q�[���I�u�W�F�N�g�̃��X�g
		//�z��̊i�[��
		typedef std::list<IGameObject*> GameObjectList;
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
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�����̃w���p�[�֐�
	/// </summary>
	template<class T>
	static inline T* NewGO(const char* objectName)
	{
		return ysGameObjectManager().NewGameObject<T>(objectName);
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�폜�̃w���p�[�֐�
	/// NewGO���g�p���č쐬�����I�u�W�F�N�g�͕K��DeleteGO�����s���邱��
	/// </summary>
	/// <param name="go">�폜����Q�[���I�u�W�F�N�g</param>
	static inline void DeleteGO(IGameObject* go)
	{
		g_goMgr.DeleteGOObject(go);
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�����̃w���p�[�֐�
	/// �����̃Q�[���I�u�W�F�N�g�S�Ăɑ΂���Query���s�������ꍇ�Ɏg�p����B
	/// objectName	�Q�[���I�u�W�F�N�g�̖��O
	/// func		�Q�[���I�u�W�F�N�g�������������ɌĂ΂��R�[���o�b�N�֐��B
	/// <param name="objectName"></param>
	/// <param name="func"></param>
	/// </summary>
	template<class T>
	static inline void QueryGOs(const char* objectName, std::function<bool(T* go)>func)
	{
		return g_goMgr.FindGameObjects<T>(objectName,func);
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�𖼑O�w��ō폜
	/// </summary>
	static inline void DeleteGOs(const char* objectName)
	{
		QueryGOs<IGameObject>(objectName, [](auto go) {
			DeleteGO(go);
			return true;
		});
	}
	extern ysGameObjectManager::EffectTool g_Effect;