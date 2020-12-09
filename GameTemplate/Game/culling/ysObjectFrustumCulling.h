/*!
 *@brief	フラスタムカリング処理。
 */

#pragma once

#include "ysObjectCulling.h"


	/*!
	 *@brief	フラスタムカリング処理。
	 *@details
	 * オブジェクトを内包するAABBを構成する8頂点がカメラに映らない場合にカリングされます。
	 */
	class CObjectFrustumCulling : public IObjectCulling{
	public:
		/*!
		 *@brief	コンストラクタ。
		 */
		CObjectFrustumCulling();
		/*!
		 *@brief	デストラクタ。
		 */
		~CObjectFrustumCulling();
		/*
		 *@brief	カリングアルゴリズムの実行。
		 *@param[in]	box	カリングを行いたいオブジェクトをバウンディングボックス。
		 */
		void Execute(const CBox& box) override;
	private:
	};