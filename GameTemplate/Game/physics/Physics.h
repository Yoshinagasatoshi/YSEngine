#pragma once
//#include "physics/RigidBody.h"
class RigidBody;
class PhysicsWorld
{
	//btDefaultCollisionConfiguration*		collisionConfig = nullptr;
	//btCollisionDispatcher*					collisionDispatcher = nullptr;	//!<衝突解決処理。
	//btBroadphaseInterface*					overlappingPairCache = nullptr;	//!<ブロードフェーズ。衝突判定の枝切り。
	//btSequentialImpulseConstraintSolver*	constraintSolver = nullptr;		//!<コンストレイントソルバー。拘束条件の解決処理。
	//btDiscreteDynamicsWorld*				dynamicWorld = nullptr;			//!<ワールド。

	std::unique_ptr<btDefaultCollisionConfiguration>	m_collisionConfig;
	std::unique_ptr<btCollisionDispatcher>				m_collisionDispatcher;//!<衝突解決処理。
	std::unique_ptr<btBroadphaseInterface>				m_overlappingPairCache;//!<ブロードフェーズ。衝突判定の枝切り。
	std::unique_ptr<btSequentialImpulseConstraintSolver>m_constraintSolver;//!<コンストレイントソルバー。拘束条件の解決処理。
	std::unique_ptr<btDiscreteDynamicsWorld>			m_dynamicWorld;//!<ワールド。

	
public:
	~PhysicsWorld();
	void Init();
	void Update();
	void Release();
	/*!
	* @brief	ダイナミックワールドを取得。
	*/
	btDiscreteDynamicsWorld* GetDynamicWorld()
	{
		return m_dynamicWorld.get();
	}
	/*!
	* @brief	剛体を登録。
	*/
	void AddRigidBody(RigidBody& rb);
	/*!
	* @brief	剛体を破棄。
	*/
	void RemoveRigidBody(RigidBody& rb);
	/// <summary>
	/// コリジョンオブジェクトをワールドに登録
	/// </summary>
	/// <param name="colliObj">コリジョンオブジェクト</param>
	void AddCollisionObject(btCollisionObject& colliObj)
	{
		m_dynamicWorld->addCollisionObject(&colliObj);
	}
	/// <summary>
	/// コリジョンオブジェクトをワールドから削除する。
	/// </summary>
	/// <param name="colliObj"></param>
	void RemoveCollisionObject(btCollisionObject& colliObj)
	{
		m_dynamicWorld->removeCollisionObject(&colliObj);
	}

	void ConvexSweepTest(
		const btConvexShape* castShape,
		const btTransform& convexFromWorld,
		const btTransform& convexToWorld,
		btCollisionWorld::ConvexResultCallback& resultCallback,
		btScalar allowedCcdPenetration = 0.0f
	)
	{
		m_dynamicWorld->convexSweepTest(castShape, convexFromWorld, convexToWorld, resultCallback, allowedCcdPenetration);
	}
	void ContactText(
		btCollisionObject* colObj, 
		btCollisionWorld::ContactResultCallback& resultCallback
	)
	{
		m_dynamicWorld->contactTest(colObj, resultCallback);
	}
	void ContactTest(
		btCollisionObject* colObj,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	);
	void DebugWire();
};
extern PhysicsWorld g_physics;