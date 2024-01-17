#include "BulletDynamics/Dynamics/btRigidBody.h"
#include <OgreCommon.h>
#include <OgreRenderSystem.h>
#include <string>
#include <vector>
#include <map>
#include <map>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <iostream>

// Globals
constexpr int maxObjects = 10;
constexpr int timeInterval = 100;
constexpr int objectLifeTime = 300;
int tick = 0;


struct BodyParameters {
	btScalar mass;
	btVector3 inertia;
	btDefaultMotionState* motionState;
	btCollisionShape* collisionShape;
	btRigidBody* rigidBody;
	Ogre::Node* ogreNode;
	int* lifeTime;
};
typedef BodyParameters BodyParameters;

class Physics
{
public:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	std::vector<btCollisionShape*> collisionShapes;
	std::map<Ogre::Node*, BodyParameters> bodies;

	Physics()
	{
		collisionConfiguration = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfiguration);
		overlappingPairCache = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver();
		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
		dynamicsWorld->setGravity(btVector3(0.0f, -9.80665f, 0.0f)); // earth
	}

	~Physics()
	{
		// https://stackoverflow.com/questions/40080205/rigid-body-still-colliding-after-removing-from-world-in-bullet-physics			
		std::cout << "Freeing " << bodies.size() << " objects" << std::endl;
		for (auto b : bodies) {
			delete b.second.motionState;
			delete b.second.collisionShape;
			dynamicsWorld->removeRigidBody(b.second.rigidBody);
			delete b.second.rigidBody;
		}

		delete dynamicsWorld;
		delete solver;
		delete overlappingPairCache;
		delete dispatcher;
		delete collisionConfiguration;
	}

	void addBox(Ogre::SceneNode* ogreNode, btVector3 boxDimensions, btScalar mass, btVector3 inertia, btScalar damping)
	{
		btCollisionShape* rigidShape = new btBoxShape(boxDimensions);
		collisionShapes.push_back(rigidShape);
		// set the initial position and transform. For this demo, we set the tranform to be none
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setRotation(btQuaternion(1.0f, 1.0f, 1.0f, 0));
		// set the mass of the object. a mass of "0" means that it is an immovable object
		Ogre::Vector3 position = ogreNode->getPosition();
		startTransform.setOrigin(btVector3(position[0], position[1], position[2]));
		// startTransform.setOrigin(btVector3(-5, 10, 0));
		rigidShape->calculateLocalInertia(mass, inertia);
		// actually contruct the body and add it to the dynamics world
		btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, rigidShape, inertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		std::cout << "set mass to " << mass << std::endl;
		std::cout << "set inertia to " << inertia[0] << "," << inertia[1] << "," << inertia[2] << std::endl;
		body->setMassProps(mass, inertia);
		body->setRestitution(1);
		body->setUserPointer(ogreNode);
		body->setDamping(damping, 0);
		dynamicsWorld->addRigidBody(body);
		// keep reference to allocated body
		int* t = new int;
		*t = objectLifeTime;
		bodies.insert(std::pair<Ogre::Node*, BodyParameters>(ogreNode, { mass, inertia, motionState, rigidShape, body, ogreNode, t }));
	}

	void addGround(Ogre::SceneNode* ogreNode) {
		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, 0, 0));
		btScalar groundMass(0.); //the mass is 0, because the ground is immovable (static)
		btVector3 localGroundInertia(0, 0, 0);
		btCollisionShape* groundShape = new btBoxShape(btVector3(10, 1, 10));
		btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
		groundShape->calculateLocalInertia(groundMass, localGroundInertia);
		btRigidBody::btRigidBodyConstructionInfo groundRBInfo(groundMass, groundMotionState, groundShape, localGroundInertia);
		btRigidBody* groundBody = new btRigidBody(groundRBInfo);
		dynamicsWorld->addRigidBody(groundBody);
		int* lf = new int;
		*lf = objectLifeTime;
		bodies.insert(std::pair<Ogre::Node*, BodyParameters>(ogreNode, { groundMass, localGroundInertia, groundMotionState, nullptr, groundBody, ogreNode, lf }));
	}
};

class KeyHandler : public OgreBites::InputListener
{
	bool keyPressed(const OgreBites::KeyboardEvent& evt) override
	{
		if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
		{
			Ogre::Root::getSingleton().queueEndRendering();
		}
		return true;
	}
};


void addRandomCubeToTheScene(Physics &physics, Ogre::SceneManager& sceneManager, int currentTick)
{
	if (currentTick /*% timeInterval == 0*/ == 50 || currentTick == 100) {
		/*Ogre::Real maxHeight = (Ogre::Real)-100.0f;
		for (std::map<Ogre::Node*, BodyParameters>::iterator i = bodies.begin(); i != bodies.end(); i++) {
			if (i->second.rigidBody) {
				Ogre::Vector3 p = i->first->getPosition();
				Ogre::Real height = p[2];
				if (height > maxHeight) {
					maxHeight = height;
				}
			}
		}
		std::cout << "MaxHeight" << maxHeight << std::endl;*/


		Ogre::Entity* cubeEntity = sceneManager.createEntity("Cube.mesh");
		std::string nodeName = "cubeNode" + std::to_string(currentTick);
		Ogre::SceneNode* cubeNode = sceneManager.getRootSceneNode()->createChildSceneNode(nodeName);
		cubeNode->setPosition(0, 13, 0);
		cubeNode->scale(.5, .5, .5);
		cubeNode->attachObject(cubeEntity);
		physics.addBox(cubeNode, btVector3(0.5f, 0.5f, 0.5f), btScalar(0.1f), btVector3(0.0f, 1.0f, 0.0f), 0.2f);
	}
}



class PhysFrameListener : public Ogre::FrameListener
{
private:
	Physics* physics;
	Ogre::SceneManager* sceneManager;

public:
	PhysFrameListener(Physics& p, Ogre::SceneManager& sm) 
		:physics(&p), sceneManager(&sm) {};

	bool frameStarted(const Ogre::FrameEvent& evt)
	{
		physics->dynamicsWorld->stepSimulation(1.0f / 60.0f); //suppose you have 60 frames per second

		//std::cout << "physic objects:" << physics->dynamicsWorld->getCollisionObjectArray().size() << std::endl;
		for (int i = 0; i < physics->dynamicsWorld->getCollisionObjectArray().size(); i++) {
			btCollisionObject* obj = physics->dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);

			if (body && body->isStaticObject()) {
				// it should be the ground
				/* body->applyCentralForce(btVector3(0.f,-2.0f,0.f)); */
				/* body->setDamping(1.0f, 2*3.14f); */
			}

			else if (body && body->getMotionState()) {
				btTransform trans;
				body->getMotionState()->getWorldTransform(trans);

				// Ogre moving object node referenced by userPointer
				void* userPointer = body->getUserPointer();
				if (userPointer) {
					btQuaternion orientation = trans.getRotation();
					Ogre::SceneNode* sceneNode = static_cast<Ogre::SceneNode*>(userPointer);
					sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
					sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));

					int* t = physics->bodies.at(sceneNode).lifeTime;
					//std::cout << "lifeTime:" << *t << std::endl;
					if (t && *t) {
						(*t)--;
					}
					else if (t) {
						//physics->dynamicsWorld->removeRigidBody(body);
						//sceneManager->destroySceneNode(sceneNode);
					}
				}
			}
		}
		return true;
	}

	bool frameEnded(const Ogre::FrameEvent& evt)
	{
		addRandomCubeToTheScene(*physics, *sceneManager, tick);
		tick++;
		return true;
	}
};





int main()
{
	Physics physics;

	Ogre::LogManager* logManager = new Ogre::LogManager();
	logManager->createLog("ogre.log", true, false, false);

	OgreBites::ApplicationContext ctx("Ogre-Bullet");
	ctx.initApp();

	Ogre::Root* root = ctx.getRoot();
	Ogre::SceneManager* sceneManager = root->createSceneManager();

	PhysFrameListener* physFrameListener = new PhysFrameListener(physics, *sceneManager);
	root->addFrameListener(physFrameListener);

	// Shader init
	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(sceneManager);

	// create light
	sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Light* light = sceneManager->createLight("MainLight");
	Ogre::SceneNode* lightNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);
	lightNode->setPosition(20, 80, 50);

	// create the camera
	Ogre::SceneNode* camNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	Ogre::Camera* cam = sceneManager->createCamera("myCam");
	cam->setNearClipDistance(Ogre::Real(0.1));
	cam->setFarClipDistance(100);
	cam->setAutoAspectRatio(true);
	camNode->attachObject(cam);
	camNode->setPosition(0, 5, 20);
	// camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);

	// and tell it to render into the main window
	ctx.getRenderWindow()->addViewport(cam);


	// create a plane
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshPtr planePtr = Ogre::MeshManager::getSingleton().createPlane("ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
		1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* groundEntity = sceneManager->createEntity(planePtr);
	Ogre::MaterialPtr bumpyMaterial = Ogre::MaterialManager::getSingleton().getByName("OgreLogo");
	//bumpyMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScale(0.03, 0.03);
	bumpyMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureFiltering(Ogre::FO_ANISOTROPIC, Ogre::FO_ANISOTROPIC, Ogre::FO_NONE);
	groundEntity->setMaterial(bumpyMaterial);
	Ogre::SceneNode* groundNode = sceneManager->getRootSceneNode()->createChildSceneNode("groundNode");
	groundNode->attachObject(groundEntity);
	groundNode->yaw(Ogre::Degree(180));

	//// create a cube
	//Ogre::Entity* cubeEntityOne = sceneManager->createEntity("Cube.mesh");
	//Ogre::SceneNode* cubeNodeOne = sceneManager->getRootSceneNode()->createChildSceneNode("cubeNodeOne");
	//cubeNodeOne->scale(1.0, 1.0, 1.0);
	//cubeNodeOne->setPosition(0, 10, 0);
	//cubeNodeOne->attachObject(cubeEntityOne);
	///* cubeNodeOne->yaw(Ogre::Degree(-30)); */
	///* cubeNodeOne->pitch(Ogre::Degree(30)); */

	//// create another cube
	//Ogre::Entity* cubeEntityTwo = sceneManager->createEntity("Cube.mesh");
	//Ogre::SceneNode* cubeNodeTwo = sceneManager->getRootSceneNode()->createChildSceneNode("cubeNodeTwo");
	//cubeNodeTwo->setPosition(0, 13, 0);
	//cubeNodeTwo->scale(.5, .5, .5);
	//cubeNodeTwo->attachObject(cubeEntityTwo);
	///* cubeNodeTwo->yaw(Ogre::Degree(30)); */
	///* cubeNodeTwo->pitch(Ogre::Degree(30)); */

	// sky
	sceneManager->setSkyBox(true, "TrippySkyBox", 99, false);

	// create the plane entity to the physics engine, and attach it to the node
	physics.addGround(groundNode);

	// simulate air friction with a damping factor
	//physics.addBox(cubeNodeOne, btVector3(1.0f, 1.0f, 1.0f), btScalar(0.2f), btVector3(0.0f, 2.0f, 0.0f), 0.4f);
	//physics.addBox(cubeNodeTwo, btVector3(0.5f, 0.5f, 0.5f), btScalar(0.1f), btVector3(0.0f, 1.0f, 0.0f), 0.2f);

	// keys
	KeyHandler keyHandler;
	ctx.addInputListener(&keyHandler);

	// Ogre loop
	ctx.getRoot()->startRendering();

	// free Ogre objects
	delete physFrameListener;
	delete logManager;
	return 0;
}
