#include <OgreCommon.h>
#include <OgreRenderSystem.h>
#include <string>
#include <vector>
#include <map>
#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <iostream>

#ifdef _WINDOWS
#pragma warning( disable : 26495 )
#endif // _WINDOWS


// Globals
constexpr int OBJECT_LIFE_TIME = 300;
constexpr int WAIT_FRAMES = 0;
constexpr float RADIUS = 2.0f;
constexpr float FAR_CLIP = 500.0f;
constexpr int REFRESH_RATE = 75;

int tick = 0;
int waitedFrames = 0;
float angleIncrement = 0;

Ogre::Entity *rookEntity;
Ogre::Entity *rookEntityTwo;
Ogre::SceneNode *rookNode;
Ogre::SceneNode *rookNodeTwo;


class KeyHandler : public OgreBites::InputListener
{
	bool keyPressed(const OgreBites::KeyboardEvent &evt) override
	{
		if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
		{
			Ogre::Root::getSingleton().queueEndRendering();
		}
		return true;
	}
};


class RookFrameListener : public Ogre::FrameListener
{
private:
	Ogre::SceneManager *sceneManager;

public:
	RookFrameListener(Ogre::SceneManager &sm)
		:sceneManager(&sm) {};

	bool frameStarted(const Ogre::FrameEvent &evt)
	{

		angleIncrement+=.01f;

		rookNode->setPosition(
			-1 + std::cos(angleIncrement) * RADIUS, 
			rookNode->getPosition()[1], 
			10 + std::sin(angleIncrement) * RADIUS);

		rookNodeTwo->setPosition(
		1 - std::cos(angleIncrement) * RADIUS, 
		rookNodeTwo->getPosition()[1], 
		10 - std::sin(angleIncrement) * RADIUS);

		return true;
	}

	bool frameEnded(const Ogre::FrameEvent &evt)
	{
		tick++;
		return true;
	}
};



int main()
{
	//Ogre::LogManager *logManager = new Ogre::LogManager();
	//logManager->createLog("ogre.log", true, false, false);

	OgreBites::ApplicationContext ctx("Ogre-Bullet");
	ctx.initApp();

	ctx.getRenderWindow()->setVSyncEnabled(true);

	Ogre::Root *root = ctx.getRoot();
	Ogre::SceneManager *sceneManager = root->createSceneManager();

	RookFrameListener *physFrameListener = new RookFrameListener(/*physics, */*sceneManager);
	root->addFrameListener(physFrameListener);

	// Shader init
	Ogre::RTShader::ShaderGenerator *shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(sceneManager);

	// create light
	sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Light *light = sceneManager->createLight("MainLight");
	Ogre::SceneNode *lightNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);
	lightNode->setPosition(20, 80, 50);

	// create the camera
	Ogre::SceneNode *camNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	Ogre::Camera *cam = sceneManager->createCamera("myCam");
	cam->setNearClipDistance(Ogre::Real(0.1));
	cam->setFarClipDistance(FAR_CLIP);
	cam->setAutoAspectRatio(true);
	camNode->attachObject(cam);
	Ogre::Vector3 cp = camNode->getPosition();
	Ogre::Vector3 lap = Ogre::Vector3(0, 0, 0);
	Ogre::Vector3 dir = lap - cp;
	camNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
	camNode->setPosition(0, 6, 26);
	ctx.getRenderWindow()->addViewport(cam);


	// create a plane
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshPtr planePtr = Ogre::MeshManager::getSingleton().createPlane("ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
		1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity *groundEntity = sceneManager->createEntity(planePtr);
	Ogre::MaterialPtr bumpyMaterial = Ogre::MaterialManager::getSingleton().getByName("OgreLogo");
	//bumpyMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScale(0.03, 0.03);
	bumpyMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureFiltering(Ogre::FO_ANISOTROPIC, Ogre::FO_ANISOTROPIC, Ogre::FO_NONE);
	groundEntity->setMaterial(bumpyMaterial);
	Ogre::SceneNode *groundNode = sceneManager->getRootSceneNode()->createChildSceneNode("groundNode");
	groundNode->attachObject(groundEntity);
	groundNode->yaw(Ogre::Degree(180));

	

	// mesh
	//Ogre::Entity *rookEntity = sceneManager->createEntity("rook.mesh");
	//Ogre::MaterialPtr shaderMaterial = Ogre::MaterialManager::getSingleton().getByName("SimpleRedFragment");
	//rookEntity->setMaterial(shaderMaterial);
	//Ogre::SceneNode *rookNode = sceneManager->getRootSceneNode()->createChildSceneNode("Rook");
	//rookNode->setPosition(-2, 1.5, 0);

	//rookNode->scale(.2, .2, .2);
	//rookNode->attachObject(rookEntity);
	//rookNode->pitch(Ogre::Degree(-90));

	Ogre::MaterialPtr shaderMaterial = Ogre::MaterialManager::getSingleton().getByName("FlatFragment");
	Ogre::MaterialPtr monochromeOrderedDitherMaterial = Ogre::MaterialManager::getSingleton().getByName("MonochromeOrderedFragment");

	rookEntity = sceneManager->createEntity("rook.mesh");
	rookEntity->setMaterial(shaderMaterial);
	rookNode = sceneManager->getRootSceneNode()->createChildSceneNode("Rook");
	rookNode->setPosition(-1, 2, 10);
	rookNode->attachObject(rookEntity);

	rookEntityTwo = sceneManager->createEntity("rook.mesh");
	rookEntityTwo->setMaterial(monochromeOrderedDitherMaterial);
	rookNodeTwo = sceneManager->getRootSceneNode()->createChildSceneNode("RookTwo");
	rookNodeTwo->setPosition(1, 2, 10);
	rookNodeTwo->attachObject(rookEntityTwo);

	
	Ogre::MaterialPtr gelbMaterial = Ogre::MaterialManager::getSingleton().getByName("Gelb");
	Ogre::MaterialPtr blauMaterial = Ogre::MaterialManager::getSingleton().getByName("Blau");

	Ogre::Entity *bishop = sceneManager->createEntity("bishop.mesh");
	bishop->setMaterial(gelbMaterial);
	Ogre::SceneNode *bishopNode = sceneManager->getRootSceneNode()->createChildSceneNode("bishop");
	bishopNode->setPosition(-2, 2, 0);
	bishopNode->attachObject(bishop);

	Ogre::Entity *knight = sceneManager->createEntity("knight.mesh");
	knight->setMaterial(gelbMaterial);
	Ogre::SceneNode *knightNode = sceneManager->getRootSceneNode()->createChildSceneNode("knight");
	knightNode->setPosition(0, 2, 0);
	knightNode->attachObject(knight);

	Ogre::Entity *queen = sceneManager->createEntity("queen.mesh");
	queen->setMaterial(blauMaterial);
	Ogre::SceneNode *queenNode = sceneManager->getRootSceneNode()->createChildSceneNode("queen");
	queenNode->setPosition(2, 2, 0);
	queenNode->attachObject(queen);

	Ogre::Entity *king = sceneManager->createEntity("king.mesh");
	king->setMaterial(blauMaterial);
	Ogre::SceneNode *kingNode = sceneManager->getRootSceneNode()->createChildSceneNode("king");
	kingNode->setPosition(4, 2, 0);
	kingNode->attachObject(king);

	Ogre::Entity *pawn = sceneManager->createEntity("pawn.mesh");
	pawn->setMaterial(gelbMaterial);
	Ogre::SceneNode *pawnNode = sceneManager->getRootSceneNode()->createChildSceneNode("pawn");
	pawnNode->setPosition(6, 2, 0);
	pawnNode->attachObject(pawn);


	//ObjLoader::loadObjTriangle("resources/Chessboard.obj", *sceneManager, blauMaterial);

	// sky
	sceneManager->setSkyBox(true, "TrippySkyBox", 100, false);

	// keys
	KeyHandler keyHandler;
	ctx.addInputListener(&keyHandler);

	// Ogre loop
	ctx.getRoot()->startRendering();

	// free Ogre objects
	delete physFrameListener;
	//delete logManager;
	return 0;
}
