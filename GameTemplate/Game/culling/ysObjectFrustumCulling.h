/*!
 *@brief	�t���X�^���J�����O�����B
 */

#pragma once

#include "ysObjectCulling.h"


	/*!
	 *@brief	�t���X�^���J�����O�����B
	 *@details
	 * �I�u�W�F�N�g������AABB���\������8���_���J�����ɉf��Ȃ��ꍇ�ɃJ�����O����܂��B
	 */
	class CObjectFrustumCulling : public IObjectCulling{
	public:
		/*!
		 *@brief	�R���X�g���N�^�B
		 */
		CObjectFrustumCulling();
		/*!
		 *@brief	�f�X�g���N�^�B
		 */
		~CObjectFrustumCulling();
		/*!
		 *@brief	�������B
		 */
		//void Init(Camera& camera )
		//{
		//	m_camera = &camera;
		//}
		/*!
		 *@brief	�J�����O�A���S���Y���̎��s�B
		 *@param[in]	box	�J�����O���s�������I�u�W�F�N�g���o�E���f�B���O�{�b�N�X�B
		 */
		void Execute(const CBox& box) override;
	private:
		 //Camera*	m_camera = nullptr;
	};