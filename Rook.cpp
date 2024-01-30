#include <OgreCommon.h>
#include <OgreRenderSystem.h>
#include <string>
#include <vector>
#include <map>
#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <iostream>
#include "Rook.h"

#ifdef _WINDOWS
#pragma warning( disable : 26495 )
#endif // _WINDOWS

void rotateAroundLookAtPoint(bool x, float increment)
{
	Ogre::Vector3 camPosition = camNode->getPosition();
	std::cout << "camPosition:" << camPosition << " - " <<   camNode->convertWorldToLocalPosition(camPosition) <<  std::endl;

	Ogre::Vector3 originTranslation = Ogre::Vector3(0, 0, 0) - LOOK_AT_POINT;
	std::cout << "originTranslation:" << originTranslation << std::endl;

	camPosition = camPosition + originTranslation;

	std::cout << "translated cam position:" << camPosition << std::endl;

	// camNode->convertWorldToLocalDirection

	if (x) {
		Ogre::Quaternion xRotQ(Ogre::Radian(increment), Ogre::Vector3::UNIT_Y);
		Ogre::Matrix4 xRotM(xRotQ);
		Ogre::Vector4f xRotC(camPosition[0], camPosition[1], camPosition[2], 1);
		Ogre::Vector4f xRotNC = xRotM * xRotC;
		camNode->setPosition(xRotNC[0] - originTranslation[0], xRotNC[1] - originTranslation[1], xRotNC[2] - originTranslation[2]);
	}
	else {
		Ogre::Quaternion yRotQ(Ogre::Radian(increment), Ogre::Vector3::UNIT_X);
		Ogre::Matrix4 yRotM(yRotQ);
		Ogre::Vector4f yRotC(camPosition[0], camPosition[1], camPosition[2], 1);
		Ogre::Vector4f yRotNC = yRotM * yRotC;
		camNode->setPosition(yRotNC[0] - originTranslation[0], yRotNC[1] - originTranslation[1], yRotNC[2] - originTranslation[2]);
	}

	// camNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
	camNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
	camNode->lookAt(LOOK_AT_POINT, Ogre::Node::TS_WORLD);
}


// void rotateAroundLookAtPoint(bool x, float increment)
// {
// 	Ogre::Vector3 camPosition = camNode->getPosition();
// 	std::cout << "camPosition:" << camPosition << " - " <<   camNode->convertWorldToLocalPosition(camPosition) <<  std::endl;

// 	Ogre::Vector3 lookAtPointInCameraSpace = camNode->convertWorldToLocalPosition(LOOK_AT_POINT);
// 	std::cout << "lookAtCam:" << lookAtPointInCameraSpace <<  std::endl;


// 	Ogre::Vector3 originTranslation = Ogre::Vector3(0, 0, 0) - lookAtPointInCameraSpace;
// 	std::cout << "originTranslation:" << originTranslation << std::endl;

// 	camPosition = camPosition + originTranslation;

// 	std::cout << "translated cam position:" << camPosition << std::endl;

// 	camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

// 	if (x) {
// 		Ogre::Quaternion xRotQ(Ogre::Radian(increment), Ogre::Vector3::UNIT_Y);
// 		Ogre::Matrix4 xRotM(xRotQ);
// 		Ogre::Vector4f xRotC(camPosition[0], camPosition[1], camPosition[2], 1);
// 		Ogre::Vector4f xRotNC = xRotM * xRotC;
// 		camNode->setPosition(xRotNC[0] - originTranslation[0], xRotNC[1] - originTranslation[1], xRotNC[2] - originTranslation[2]);
// 	}
// 	else {
// 		Ogre::Quaternion xRotQ(Ogre::Radian(increment), Ogre::Vector3::UNIT_X);
// 		Ogre::Matrix4 xRotM(xRotQ);
// 		Ogre::Vector4f xRotC(camPosition[0], camPosition[1], camPosition[2], 1);
// 		Ogre::Vector4f xRotNC = xRotM * xRotC;
// 		camNode->setPosition(xRotNC[0] - originTranslation[0], xRotNC[1] - originTranslation[1], xRotNC[2] - originTranslation[2]);
// 	}

// 	// camNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
// 	camNode->lookAt(LOOK_AT_POINT, Ogre::Node::TS_WORLD);
// }



// void adaptCamera()
// {
// 	Ogre::Vector3 camPosition = camNode->getPosition();
// 	std::cout << "camPosition:" << camPosition << std::endl;

// 	Ogre::Vector3 lap = Ogre::Vector3(X_CENTER, CAM_HEIGHT, Y_CENTER);
// 	camNode->lookAt(lap, Ogre::Node::TS_WORLD);
// 	camNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);


// 	Ogre::Vector3 originTranslation =  Ogre::Vector3(0, 0, 0) - lap;
// 	std::cout << "originTranslation:" << originTranslation << std::endl;

// 	camPosition = camPosition + originTranslation;

// 	std::cout << "translated cam position:" << camPosition << std::endl;

// 	Ogre::Quaternion xRotQ(Ogre::Radian(X_ROTATION_INCREMENT), Ogre::Vector3::UNIT_Y);
// 	Ogre::Matrix4 xRotM(xRotQ);
// 	Ogre::Vector4f xRotC(camPosition[0], camPosition[1], camPosition[2], 1);
// 	Ogre::Vector4f xRotNC = xRotM * xRotC;
// 	// Ogre::Vector3 newCamPosition(xRotC[0] - originTranslation[0], xRotC[1] - originTranslation[1], xRotC[2] - originTranslation[2]);
// 	camNode->setPosition(xRotNC[0] - originTranslation[0], xRotNC[1] - originTranslation[1], xRotNC[2] - originTranslation[2]);

// 	/*
// 	Ogre::Vector3 ocp = camNode->getPosition();
// 	camNode->setPosition(-X_CENTER, CAM_HEIGHT, -Y_CENTER);
// 	Ogre::Vector3 cp = camNode->getPosition();

// 	Ogre::Vector3 tr = cp - ocp;
// 	std::cout << ocp << cp << tr << std::endl;
	
// 	// X
// 	Ogre::Quaternion xRotQ(Ogre::Radian(xRotation), Ogre::Vector3::UNIT_Y);
// 	Ogre::Matrix4 xRotM(xRotQ);
// 	Ogre::Matrix4 xRotT;
// 	// xRotT.makeTrans(X_CENTER, 0, Y_CENTER);
// 	// std::cout << xRotT << std::endl;
// 	// Ogre::Matrix4 xRotTI;
// 	// xRotTI.makeTrans(-X_CENTER, 0, -Y_CENTER);

// 	std::cout << xRotM << std::endl;
// 	Ogre::Vector4f xRotC(cp[0], cp[1], cp[2], 1);
// 	std::cout << xRotM * xRotC << std::endl;
// 	Ogre::Vector4f xRotNC = xRotM * xRotC;
// 	camNode->setPosition(xRotNC[0], xRotNC[1], xRotNC[2]);

// 	Ogre::Vector3 ncp = camNode->getPosition();
// 	camNode->setPosition(ncp[0] + X_CENTER, CAM_HEIGHT, ncp[2] + Y_CENTER);

// 	// X-AXIS
// 	//camNode->setPosition(X_CENTER + std::cos(xRotation) * CAM_X_RADIUS, CAM_HEIGHT, Y_CENTER + std::sin(xRotation) * CAM_X_RADIUS);

// 	// Y-AXIS
// 	// Ogre::Vector3 cp = camNode->getPosition();
// 	// camNode->setPosition(cp[0], cp[1] + std::cos(yRotation) * CAM_Y_RADIUS, cp[2] + std::sin(yRotation) * CAM_Y_RADIUS);
// 	// camNode->setPosition(X_CENTER, CAM_HEIGHT + std::cos(yRotation) * CAM_Y_RADIUS, Y_CENTER + std::sin(yRotation) * CAM_Y_RADIUS);

// 	Ogre::Vector3 lap = Ogre::Vector3(X_CENTER, CAM_HEIGHT, Y_CENTER);
// 	camNode->lookAt(lap, Ogre::Node::TS_WORLD);
// 	camNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
// 	*/
// }


class KeyHandler : public OgreBites::InputListener
{
	int leftMouseButtonPressed = 0;
	int pressX = 0, pressY = 0;
	uint32_t width, height;

	public:
	KeyHandler(uint32_t w, uint32_t h) : width(w), height(h) {};


	bool keyPressed(const OgreBites::KeyboardEvent &evt) override
	{

		if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
		{
			Ogre::Root::getSingleton().queueEndRendering();
		}

		if (evt.keysym.sym == OgreBites::SDLK_LEFT)
		{
			// xRotation += X_ROTATION_INCREMENT;
			// std::cout << "xRotation:" << xRotation << " yRotation:" << yRotation << std::endl;
			rotateAroundLookAtPoint(true, X_ROTATION_INCREMENT);
		}

		if (evt.keysym.sym == OgreBites::SDLK_RIGHT)
		{
			// xRotation -= X_ROTATION_INCREMENT;
			// std::cout << "xRotation:" << xRotation << " yRotation:" << yRotation << std::endl;
			rotateAroundLookAtPoint(true, -X_ROTATION_INCREMENT);
		}

		if (evt.keysym.sym == OgreBites::SDLK_DOWN)
		{
			rotateAroundLookAtPoint(false, Y_ROTATION_INCREMENT);
		}

		if (evt.keysym.sym == OgreBites::SDLK_UP)
		{
			rotateAroundLookAtPoint(false, -Y_ROTATION_INCREMENT);
		}

		

		return true;
	}


	bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt) {
		if (evt.y == -1 && CAM_X_RADIUS < ZOOM_MAX) {
			CAM_X_RADIUS += ZOOM_INCREMENT;
		} else if (evt.y == 1 && CAM_X_RADIUS > ZOOM_MIN) {
			CAM_X_RADIUS -= ZOOM_INCREMENT;
		}
		std::cout << "zoom:" << CAM_X_RADIUS << std::endl;
		// adaptCamera();
		return true;
	}

	virtual bool mousePressed(const OgreBites::MouseButtonEvent& evt) {
		if (evt.button == 1) {
			leftMouseButtonPressed = 1;
			pressX = evt.x;
			pressY = evt.y;
		}
		return true;
	}

    virtual bool mouseReleased(const OgreBites::MouseButtonEvent& evt) {
		if (evt.button == 1) {
			leftMouseButtonPressed = 0;
		}
		return true;
	}
	
	virtual bool mouseMoved(const OgreBites::MouseMotionEvent& evt) { 
		if (leftMouseButtonPressed) {
			if (evt.x > pressX) {
				//float speed = (pressX - evt.x) / 1000.f;
				//xRotation += X_ROTATION_INCREMENT;
				return true;
			} 
			
			if (evt.x < pressX) {
				// float speed = (evt.x - pressX) / 1000.f;
				//xRotation -= X_ROTATION_INCREMENT;
				return true;
			} 

			if (evt.y > pressY) {
				//float speed = (pressY - evt.y) / 1000.f;
			} else {
				//float speed = (evt.y - pressY) / 1000.f;
			}
		}
		return true;
	}
};


class RookFrameListener : public Ogre::FrameListener
{
private:
	Ogre::SceneManager *sceneManager;
	uint32_t width, height;

public:
	RookFrameListener(Ogre::SceneManager &sm, uint32_t w, uint32_t h)
		:sceneManager(&sm), width(w), height(h) {};

	bool frameStarted(const Ogre::FrameEvent &evt)
	{
		dancersAngleIncrement+=.01f;
		rookNode->setPosition(
			-1 + std::cos(dancersAngleIncrement) * RADIUS, 
			rookNode->getPosition()[1], 
			10 + std::sin(dancersAngleIncrement) * RADIUS);

		rookNodeTwo->setPosition(
		1 - std::cos(dancersAngleIncrement) * RADIUS, 
		rookNodeTwo->getPosition()[1], 
		10 - std::sin(dancersAngleIncrement) * RADIUS);

		// refresh cam
		// std::cout << "zoom:" << zoom << std::endl;
		//camNode->setPosition(camNode->getPosition()[0], camNode->getPosition()[1], camNode->getPosition()[2] + zoom);
		/*zoom = 0;*/

		//if (yaw > 0.1f || yaw < -0.1f)
		//	camNode->yaw(Ogre::Degree(yaw));
		//
		//if (pitch > 0.1f || pitch < -0.1f)
		//	camNode->pitch(Ogre::Degree(pitch));


		//Ogre::Vector3 cp = camNode->getPosition();
		//Ogre::Vector3 lap = Ogre::Vector3(X_CENTER, cp[1], Y_CENTER);
		//Ogre::Vector3 dir = lap - cp;

		//camNode->setDirection(dir);
		//camNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
		//camNode->setPosition(X_CENTER + std::cos(xRotation) * zoom, cp[1], Y_CENTER + std::sin(xRotation) * zoom);
		//zoom = 0;

		return true;
	}

	bool frameEnded(const Ogre::FrameEvent &evt)
	{
		tick++;
		//xRotation += X_ROTATION_INCREMENT;
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

	RookFrameListener *physFrameListener = new RookFrameListener(*sceneManager, ctx.getRenderWindow()->getWidth(), ctx.getRenderWindow()->getHeight());
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
	camNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	Ogre::Camera *cam = sceneManager->createCamera("myCam");
	cam->setNearClipDistance(Ogre::Real(0.1));
	cam->setFarClipDistance(FAR_CLIP);
	cam->setAutoAspectRatio(true);
	camNode->attachObject(cam);
	
	//Ogre::Vector3 cp = camNode->getPosition();
	//Ogre::Vector3 lap = Ogre::Vector3(0, 0, 0);
	//Ogre::Vector3 dir = lap - cp;
	camNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
	camNode->setPosition(0, 6, 26);

	// camNode->setPosition(36, 6, -14);
	camNode->lookAt(LOOK_AT_POINT, Ogre::Node::TS_WORLD);
	// adaptCamera();
	ctx.getRenderWindow()->addViewport(cam);
	// std::cout << " W= " << ctx.getRenderWindow()->getWidth() << std::endl;


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

	Ogre::MaterialPtr shaderMaterial = Ogre::MaterialManager::getSingleton().getByName("FlatFragment");
	Ogre::MaterialPtr monochromeOrderedDitherMaterial = Ogre::MaterialManager::getSingleton().getByName("MonochromeOrderedFragment");
	Ogre::MaterialPtr darkMonochromeOrderedDitherMaterial = Ogre::MaterialManager::getSingleton().getByName("DarkMonochromeOrderedFragment");
	Ogre::MaterialPtr blackMonochromeOrderedDitherMaterial = Ogre::MaterialManager::getSingleton().getByName("BlackMonochromeOrderedFragment");

	rookEntity = sceneManager->createEntity("king.mesh");
	rookEntity->setMaterial(darkMonochromeOrderedDitherMaterial);
	rookNode = sceneManager->getRootSceneNode()->createChildSceneNode("DitheredKing");
	rookNode->setPosition(-1, 0, 10);
	rookNode->attachObject(rookEntity);

	rookEntityTwo = sceneManager->createEntity("queen.mesh");
	rookEntityTwo->setMaterial(monochromeOrderedDitherMaterial);
	rookNodeTwo = sceneManager->getRootSceneNode()->createChildSceneNode("DitheredQueen");
	rookNodeTwo->setPosition(1, 0, 10);
	rookNodeTwo->attachObject(rookEntityTwo);

	
	Ogre::MaterialPtr gelbMaterial = Ogre::MaterialManager::getSingleton().getByName("Gelb");
	Ogre::MaterialPtr blauMaterial = Ogre::MaterialManager::getSingleton().getByName("Blau");

	Ogre::Entity *pawn = sceneManager->createEntity("pawn.mesh");
	pawn->setMaterial(gelbMaterial);
	Ogre::SceneNode *pawnNode = sceneManager->getRootSceneNode()->createChildSceneNode("pawn");
	pawnNode->setPosition(X_CENTER, 0, Y_CENTER);
	pawnNode->attachObject(pawn);

    createBoard(sceneManager, monochromeOrderedDitherMaterial, blackMonochromeOrderedDitherMaterial);
	setPieceOnBoard(WHITE_ROOK, 0, 0, sceneManager, monochromeOrderedDitherMaterial, darkMonochromeOrderedDitherMaterial);
	setPieceOnBoard(BLACK_KNIGHT, 1, 0, sceneManager, monochromeOrderedDitherMaterial, darkMonochromeOrderedDitherMaterial);
	setPieceOnBoard(BLACK_BISHOP, 2, 0, sceneManager, monochromeOrderedDitherMaterial, darkMonochromeOrderedDitherMaterial);

    // sky
	sceneManager->setSkyBox(true, "TrippySkyBox", 100, false);

	// keys
	KeyHandler keyHandler(ctx.getRenderWindow()->getWidth(), ctx.getRenderWindow()->getHeight());
	ctx.addInputListener(&keyHandler);

	// Ogre loop
	ctx.getRoot()->startRendering();

	// free Ogre objects
	delete physFrameListener;
	//delete logManager;
	return 0;
}

void createBoard(Ogre::SceneManager *sceneManager, Ogre::MaterialPtr &whiteMaterialPtr, Ogre::MaterialPtr &blackMaterialPtr)
{
    bool black = true;

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            std::string nodeName = std::string(1, 'a' + x).append(std::to_string(y + 1));
            Ogre::Entity *square = sceneManager->createEntity("cube.mesh");

            if (!black)
                square->setMaterial(whiteMaterialPtr);
            else
                square->setMaterial(blackMaterialPtr);
            black = !black;

            Ogre::SceneNode *squareNode = sceneManager->getRootSceneNode()->createChildSceneNode(nodeName);
            squareNode->setPosition(X_START + x * SQUARE_WIDTH * 2, 0, Y_START - y * SQUARE_HEIGHT * 2);
            squareNode->setScale(SQUARE_WIDTH, 0 + BOARD_THICKNESS, SQUARE_HEIGHT);
            squareNode->attachObject(square);
        }
        black = !black;
    }
}

void setPieceOnBoard(PIECE_ENUM piece, int x, int y, Ogre::SceneManager *sceneManager, Ogre::MaterialPtr &whiteMaterialPtr, Ogre::MaterialPtr &blackMaterialPtr)
{
	Ogre::SceneNode *sceneNode = nullptr;
	if (piecesMap.find(piece) != piecesMap.end()) {
		Piece p = piecesMap[piece];
		sceneNode = p.node;
	}
	else {
		Ogre::Entity *entity = nullptr;
		switch (piece)
		{
		case BLACK_ROOK:
			entity = entity = sceneManager->createEntity("rook.mesh");
			break;
		case BLACK_KNIGHT:
			entity = entity = sceneManager->createEntity("knight.mesh");
			break;
		case BLACK_BISHOP:
			entity = entity = sceneManager->createEntity("bishop.mesh");
			break;
		case BLACK_QUEEN:
			entity = entity = sceneManager->createEntity("queen.mesh");
			break;
		case BLACK_KING:
			entity = entity = sceneManager->createEntity("king.mesh");
			break;
		case WHITE_ROOK:
			entity = entity = sceneManager->createEntity("rook.mesh");
			break;
		case WHITE_KNIGHT:
			entity = entity = sceneManager->createEntity("knight.mesh");
			break;
		case WHITE_BISHOP:
			entity = entity = sceneManager->createEntity("bishop.mesh");
			break;
		case WHITE_QUEEN:
			entity = entity = sceneManager->createEntity("queen.mesh");
			break;
		case WHITE_KING:
			entity = entity = sceneManager->createEntity("king.mesh");
			break;
		default:
			break;
		}
		entity->setMaterial(piece < 6 ? whiteMaterialPtr : blackMaterialPtr);
		sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode(std::to_string(piece));
		sceneNode->setPosition(-2, 0, 0);
		sceneNode->attachObject(entity);
		Piece p = { piece, entity, sceneNode };
		piecesMap.insert(std::pair<PIECE_ENUM, Piece>(piece, p));
	}
	sceneNode->setPosition(X_START + x * SQUARE_WIDTH * 2, 0 + BOARD_THICKNESS, Y_START - y * SQUARE_HEIGHT * 2);
}
