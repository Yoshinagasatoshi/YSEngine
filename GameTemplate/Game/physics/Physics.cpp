#include "stdafx.h"
#include "Physics/RigidBody.h"
#include "physics/Physics.h"
#include "DebugWireframe.h"
using namespace std;
PhysicsWorld g_physics;

namespace {
	struct MyContactResultCallback : public btCollisionWorld::ContactResultCallback {
		using ContactResultCallback = function<void(const btCollisionObject& contactCollisionObject)>;
		ContactResultCallback m_cb;
		btCollisionObject* m_me = nullptr;
		virtual btScalar	addSingleResult(
			btManifoldPoint& cp,
			const btCollisionObjectWrapper* colObj0Wrap,
			int partId0,
			int index0,
			const btCollisionObjectWrapper* colObj1Wrap,
			int partId1,
			int index1
		) override
		{
			if (m_me == colObj0Wrap->getCollisionObject()) {
				m_cb(*colObj1Wrap->getCollisionObject());
			}
			return 0.0f;
		}
	};
}

PhysicsWorld::~PhysicsWorld()
{
	Release();
}
void PhysicsWorld::Release()
{
	m_dynamicWorld.reset();
	m_constraintSolver.reset();
	m_overlappingPairCache.reset();
	m_collisionDispatcher.reset();
	m_collisionConfig.reset();

	//dynamicWorld = nullptr;
	//constraintSolver = nullptr;
	//overlappingPairCache = nullptr;
	//collisionDispatcher = nullptr;
	//collisionConfig = nullptr;
}
void PhysicsWorld::Init()
{
	Release();
	//�����G���W�����������B
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	m_collisionConfig = make_unique<btDefaultCollisionConfiguration>();
	//new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_collisionDispatcher = make_unique<btCollisionDispatcher>(m_collisionConfig.get());
	//new	btCollisionDispatcher(collisionConfig);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	m_overlappingPairCache = make_unique<btDbvtBroadphase>();
	//new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	m_constraintSolver = make_unique<btSequentialImpulseConstraintSolver>();
	//new btSequentialImpulseConstraintSolver;

	m_dynamicWorld = make_unique<btDiscreteDynamicsWorld>(
		m_collisionDispatcher.get(),
		m_overlappingPairCache.get(),
		m_constraintSolver.get(),
		m_collisionConfig.get()
		);

	m_dynamicWorld->setGravity(btVector3(0, -10, 0));
}
void PhysicsWorld::Update()
{
	m_dynamicWorld->stepSimulation(1.0f/60.0f);
}
void PhysicsWorld::AddRigidBody(RigidBody& rb)
{
	if (rb.IsAddPhysicsWorld() == false) {
		m_dynamicWorld->addRigidBody(rb.GetBody());
		rb.SetMarkAddPhysicsWorld();
	}
}
void PhysicsWorld::RemoveRigidBody(RigidBody& rb)
{
	if (rb.IsAddPhysicsWorld() == true) {
		m_dynamicWorld->removeRigidBody(rb.GetBody());
		rb.SetUnmarkAddPhysicsWorld();
	}
}
void PhysicsWorld::DebugWire()
{
	DebugWireframe m_dwf;
}
void PhysicsWorld::ContactTest(
	btCollisionObject* colObj,
	std::function<void(const btCollisionObject& contactCollisionObject)> cb
) 
{
	MyContactResultCallback myContactResultCallback;
	myContactResultCallback.m_cb = cb;
	myContactResultCallback.m_me = colObj;
	m_dynamicWorld->contactTest(colObj, myContactResultCallback);
}