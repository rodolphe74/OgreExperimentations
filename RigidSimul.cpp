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

#ifdef _WINDOWS
#pragma warning( disable : 26495 )
#endif // _WINDOWS


// Globals
constexpr int OBJECT_LIFE_TIME = 300;
constexpr int WAIT_FRAMES = 0;
constexpr float CAM_Y = 10.0f;
constexpr float RADIUS = 30.0f;
constexpr float FAR_CLIP = 500.0f;
constexpr int REFRESH_RATE = 75.0f;

int tick = 0;
int waitedFrames = 0;
float dancersAngleIncrement = 0;


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
	Ogre::SceneNode* camera;

	Physics()
	{
		collisionConfiguration = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfiguration);
		overlappingPairCache = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver();
		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
		dynamicsWorld->setGravity(btVector3(0.0f, -9.80665f, 0.0f)); // earth
		camera = nullptr;
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
		//std::cout << "set mass to " << mass << std::endl;
		//std::cout << "set inertia to " << inertia[0] << "," << inertia[1] << "," << inertia[2] << std::endl;
		body->setMassProps(mass, inertia);
		body->setRestitution(1);
		body->setUserPointer(ogreNode);
		body->setDamping(damping, 0);
		dynamicsWorld->addRigidBody(body);
		// keep reference to allocated body
		int* t = new int;
		*t = OBJECT_LIFE_TIME;
		bodies.insert(std::pair<Ogre::Node*, BodyParameters>(ogreNode, { mass, inertia, motionState, rigidShape, body, ogreNode, t }));
	}

	void addGround(Ogre::SceneNode* ogreNode) {
		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, 0, 0));
		btScalar groundMass(0.); //the mass is 0, because the ground is immovable (static)
		btVector3 localGroundInertia(0, 0, 0);
		btCollisionShape* groundShape = new btBoxShape(btVector3(100, 0, 100));
		btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
		groundShape->calculateLocalInertia(groundMass, localGroundInertia);
		btRigidBody::btRigidBodyConstructionInfo groundRBInfo(groundMass, groundMotionState, groundShape, localGroundInertia);
		btRigidBody* groundBody = new btRigidBody(groundRBInfo);
		dynamicsWorld->addRigidBody(groundBody);
		int* lf = new int;
		*lf = OBJECT_LIFE_TIME;
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
	//Ogre::Real maxHeight = (Ogre::Real)-100.0f;
	//for (std::map<Ogre::Node*, BodyParameters>::iterator i = physics.bodies.begin(); i != physics.bodies.end(); i++) {
	//	if (i->second.rigidBody) {
	//		Ogre::Vector3 p = i->first->getPosition();
	//		Ogre::Real height = p[1];
	//		if (height > maxHeight) {
	//			maxHeight = height;
	//		}
	//	}
	//}
	//std::cout << "MaxHeight" << maxHeight << std::endl;

	//if (maxHeight < 14) {
	if (currentTick % (REFRESH_RATE / 5) == 0) {

		int x = std::rand() % 11 - 5;	// (0 to 10) minus 5
		int s = std::rand() % 50 + 21;
		float sf = s / 100.0f;

		Ogre::Entity* cubeEntity = sceneManager.createEntity("cube.mesh");
		std::string nodeName = "cubeNode" + std::to_string(currentTick);
		Ogre::SceneNode* cubeNode = sceneManager.getRootSceneNode()->createChildSceneNode(nodeName);
		cubeNode->setPosition(Ogre::Real(x), 15, 0);
		//cubeNode->scale(.5, .5, .5);
		cubeNode->scale(sf, sf, sf);
		cubeNode->attachObject(cubeEntity);
		//physics.addBox(cubeNode, btVector3(0.5f, 0.5f, 0.5f), btScalar(0.1f), btVector3(0.0f, 1.0f, 0.0f), 0.2f);
		// simulate air friction with a damping factor
		physics.addBox(cubeNode, btVector3(sf, sf, sf), btScalar(sf / 2), btVector3(0.0f, 1.0f, 0.0f), btScalar(sf / 2));
	}
}



class RookFrameListener : public Ogre::FrameListener
{
private:
	Physics* physics;
	Ogre::SceneManager* sceneManager;

public:
	RookFrameListener(Physics& p, Ogre::SceneManager& sm) 
		:physics(&p), sceneManager(&sm) {};

	bool frameStarted(const Ogre::FrameEvent& evt)
	{
		if (waitedFrames > 0) {
			waitedFrames--;
			return true;
		} else {
			waitedFrames = WAIT_FRAMES;
		}
		
		physics->dynamicsWorld->stepSimulation(1.0f / REFRESH_RATE); //suppose you have 60 frames per second

		//std::cout << "physic objects:" << physics->dynamicsWorld->getCollisionObjectArray().size() << std::endl;
		for (int i = 0; i < physics->dynamicsWorld->getCollisionObjectArray().size(); i++) {
			btCollisionObject* obj = physics->dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);

			if (body && body->isStaticObject()) {
				// it should be the ground
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
						//(*t)--;
					}
					else if (t) {
						//physics->dynamicsWorld->removeRigidBody(body);
						//sceneManager->destroySceneNode(sceneNode);
					}
				}
			}
		}

		dancersAngleIncrement+=.005f;
		Ogre::Vector3 cp = physics->camera->getPosition();
		Ogre::Vector3 lap = Ogre::Vector3(0, 0, 0);
		Ogre::Vector3 dir = lap - cp;
		
		physics->camera->setDirection(dir);
		physics->camera->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
		physics->camera->setPosition(std::cos(dancersAngleIncrement) * RADIUS, cp[1], std::sin(dancersAngleIncrement) * RADIUS);
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

	ctx.getRenderWindow()->setVSyncEnabled(true);

	Ogre::Root* root = ctx.getRoot();
	Ogre::SceneManager* sceneManager = root->createSceneManager();

	RookFrameListener* physFrameListener = new RookFrameListener(physics, *sceneManager);
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
	cam->setFarClipDistance(FAR_CLIP);
	cam->setAutoAspectRatio(true);
	camNode->attachObject(cam);
	Ogre::Vector3 cp = camNode->getPosition();
	Ogre::Vector3 lap = Ogre::Vector3(0, 0, 0);
	Ogre::Vector3 dir = lap - cp;
	camNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
	camNode->setPosition(std::cos(dancersAngleIncrement) * RADIUS, CAM_Y, std::sin(dancersAngleIncrement) * RADIUS);
	physics.camera = camNode;

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

	// sky
	sceneManager->setSkyBox(true, "TrippySkyBox", 100, false);

	// create the plane entity to the physics engine, and attach it to the node
	physics.addGround(groundNode);

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
