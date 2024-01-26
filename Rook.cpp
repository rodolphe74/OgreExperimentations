#include <OgreCommon.h>
#include <OgreRenderSystem.h>
#include <string>
#include <vector>
#include <map>
#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <iostream>
#include <OBJ_Loader.h>

#ifdef _WINDOWS
#pragma warning( disable : 26495 )
#endif // _WINDOWS


// Globals
constexpr int OBJECT_LIFE_TIME = 300;
constexpr int WAIT_FRAMES = 0;
constexpr float CAM_Y = 10.0f;
constexpr float CAM_Z = 30.0f;
constexpr float FAR_CLIP = 500.0f;
constexpr int REFRESH_RATE = 75;

int tick = 0;
int waitedFrames = 0;
float cameraIncrement = 0;




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
		return true;
	}

	bool frameEnded(const Ogre::FrameEvent &evt)
	{
		tick++;
		return true;
	}
};



class ObjLoader {
public:
	static std::vector<Ogre::ManualObject *> loadObjTriangle(std::string filename, Ogre::SceneManager &sceneManager, Ogre::MaterialPtr &defaultMaterial) {
		std::vector<Ogre::ManualObject *> list;

		objl::Loader Loader;
		Loader.LoadFile(filename);
		for (int i = 0; i < Loader.LoadedMeshes.size(); i++) {
			objl::Mesh curMesh = Loader.LoadedMeshes[i];

			Ogre::ManualObject *manualObject = sceneManager.createManualObject(curMesh.MeshName);
			manualObject->begin("Blau", Ogre::RenderOperation::OT_TRIANGLE_LIST);

			for (int j = 0; j < curMesh.Vertices.size(); j++) {
				for (int j = 0; j < curMesh.Indices.size(); j += 3) {
					manualObject->triangle(curMesh.Indices[j], curMesh.Indices[j + 1], curMesh.Indices[j + 2]);
				}
			}
			
			// manualObject->setMaterial(0, defaultMaterial);
			//manualObject->setMaterialName(0, "Blau");
			manualObject->end();
			list.push_back(manualObject);
		}

		return list;
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
	camNode->setPosition(0, 3, 20);
	// camNode->setPosition(std::cos(cameraIncrement) * CAM_Z, CAM_Y, std::sin(cameraIncrement) * CAM_Z);
	// and tell it to render into the main window
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

	Ogre::Entity *rookEntityTwo = sceneManager->createEntity("rook.mesh");
	Ogre::MaterialPtr blauMaterial = Ogre::MaterialManager::getSingleton().getByName("Blau");
	rookEntityTwo->setMaterial(blauMaterial);
	Ogre::SceneNode *rookNodeTwo = sceneManager->getRootSceneNode()->createChildSceneNode("RookTwo");
	rookNodeTwo->setPosition(-4, 2, 0);
	rookNodeTwo->attachObject(rookEntityTwo);

	Ogre::Entity *bishop = sceneManager->createEntity("bishop.mesh");
	Ogre::MaterialPtr gelbMaterial = Ogre::MaterialManager::getSingleton().getByName("Gelb");
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
