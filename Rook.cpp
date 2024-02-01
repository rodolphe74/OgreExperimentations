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



Ogre::Vector3 ArbitraryRotate(Ogre::Vector3 p, Ogre::Real theta, Ogre::Vector3 p1, Ogre::Vector3 p2)
{
	// Rotate a point p by angle theta around an arbitrary line segment p1-p2
	// Return the rotated point.
	// Positive angles are anticlockwise looking down the axis
	// towards the origin.
	// Assume right hand coordinate system.
	// https://paulbourke.net/geometry/rotate/

	Ogre::Vector3 q(0.0, 0.0, 0.0);
	Ogre::Real costheta, sintheta;
	Ogre::Vector3 r;

	r.x = p2.x - p1.x;
	r.y = p2.y - p1.y;
	r.z = p2.z - p1.z;
	p.x -= p1.x;
	p.y -= p1.y;
	p.z -= p1.z;
	r.normalise();

	costheta = std::cos(theta);
	sintheta = std::sin(theta);

	q.x += (costheta + (1 - costheta) * r.x * r.x) * p.x;
	q.x += ((1 - costheta) * r.x * r.y - r.z * sintheta) * p.y;
	q.x += ((1 - costheta) * r.x * r.z + r.y * sintheta) * p.z;

	q.y += ((1 - costheta) * r.x * r.y + r.z * sintheta) * p.x;
	q.y += (costheta + (1 - costheta) * r.y * r.y) * p.y;
	q.y += ((1 - costheta) * r.y * r.z - r.x * sintheta) * p.z;

	q.z += ((1 - costheta) * r.x * r.z - r.y * sintheta) * p.x;
	q.z += ((1 - costheta) * r.y * r.z + r.x * sintheta) * p.y;
	q.z += (costheta + (1 - costheta) * r.z * r.z) * p.z;

	q.x += p1.x;
	q.y += p1.y;
	q.z += p1.z;
	return(q);
}


void rotateAroundLookAtPointThenTranslateZ()
{
	Ogre::Vector3 camPosition = CAM_POSITION - LOOK_AT_POINT;

	// Rotate X
	camPosition = ArbitraryRotate(camPosition, Ogre::Real(xRotation), Ogre::Vector3(0, 0, 0), Ogre::Vector3::UNIT_Y);

	// Rotate Y in new frame
	Ogre::Vector3 frameX = ArbitraryRotate(Ogre::Vector3::UNIT_X, Ogre::Real(xRotation), Ogre::Vector3(0, 0, 0), Ogre::Vector3::UNIT_Y);
	camPosition = ArbitraryRotate(camPosition, Ogre::Real(yRotation), Ogre::Vector3(0, 0, 0), frameX);

	camNode->setPosition(camPosition + LOOK_AT_POINT);
	camNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);

	// translate Z in new frame
	Ogre::Vector3 cameraZ = LOOK_AT_POINT - camNode->getPosition();
	cameraZ.normalise();
	camNode->translate(cameraZ * zTranslate, Ogre::Node::TS_WORLD);

	camNode->lookAt(LOOK_AT_POINT, Ogre::Node::TS_WORLD);
}


class KeyHandler : public OgreBites::InputListener
{
	int leftMouseButtonPressed = 0;
	int pressX = 0, pressY = 0;
	uint32_t width, height;
	Ogre::SceneManager *sceneManager;
	Ogre::RaySceneQuery *raySceneQuery;

	public:
		KeyHandler(Ogre::SceneManager &sm, Ogre::RaySceneQuery &rsq, uint32_t w, uint32_t h) 
			: sceneManager(&sm), raySceneQuery(&rsq), width(w), height(h) {};

	bool keyPressed(const OgreBites::KeyboardEvent &evt) override
	{
		switch (evt.keysym.sym) {
		case OgreBites::SDLK_ESCAPE:
			Ogre::Root::getSingleton().queueEndRendering();
			break;
		case OgreBites::SDLK_LEFT:
			xRotation -= X_ROTATION_INCREMENT;
			rotateAroundLookAtPointThenTranslateZ();
			break;
		case OgreBites::SDLK_RIGHT:
			xRotation += X_ROTATION_INCREMENT;
			rotateAroundLookAtPointThenTranslateZ();
			break;
		case OgreBites::SDLK_DOWN:
			yRotation += Y_ROTATION_INCREMENT;
			rotateAroundLookAtPointThenTranslateZ();
			break;
		case OgreBites::SDLK_UP:
			yRotation -= Y_ROTATION_INCREMENT;
			rotateAroundLookAtPointThenTranslateZ();
			break;
		case 'p':
			zTranslate = (zTranslate < ZOOM_MAX ? ++zTranslate : zTranslate);
			rotateAroundLookAtPointThenTranslateZ();
			break;
		case 'o':
			zTranslate = (zTranslate > ZOOM_MIN ? --zTranslate : zTranslate);
			rotateAroundLookAtPointThenTranslateZ();
			break;

		default:
			break;
		}

		
		return true;
	}

	bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt) {
		if (evt.y == -1) {
			zTranslate = (zTranslate > ZOOM_MIN ? --zTranslate : zTranslate);
			rotateAroundLookAtPointThenTranslateZ();
		} else if (evt.y == 1) {
			zTranslate = (zTranslate < ZOOM_MAX ? ++zTranslate : zTranslate);
			rotateAroundLookAtPointThenTranslateZ();
		}
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
				xRotation -= X_ROTATION_INCREMENT;
				rotateAroundLookAtPointThenTranslateZ();
				pressX = evt.x;
				pressY = evt.y;
				return true;
			} 
			
			if (evt.x < pressX) {
				xRotation += X_ROTATION_INCREMENT;
				rotateAroundLookAtPointThenTranslateZ();
				pressX = evt.x;
				pressY = evt.y;
				return true;
			} 

			if (evt.y > pressY) {
				yRotation -= Y_ROTATION_INCREMENT;
				rotateAroundLookAtPointThenTranslateZ();
				pressX = evt.x;
				pressY = evt.y;
				return true;
			}

			if (evt.y < pressY) {
				yRotation += Y_ROTATION_INCREMENT;
				rotateAroundLookAtPointThenTranslateZ();
				pressX = evt.x;
				pressY = evt.y;
				return true;
			}
		}

		// default selection code
		Ogre::Ray mouseRay = cam->getCameraToViewportRay(evt.x / float(width), evt.y / float(height));
		raySceneQuery->setRay(mouseRay);
		raySceneQuery->setSortByDistance(true);
		Ogre::RaySceneQueryResult rsqr = raySceneQuery->execute();
		if (rsqr.size() > 0) {
			Ogre::RaySceneQueryResult::iterator itr;
			// Get results
			for (itr = rsqr.begin(); itr != rsqr.end(); itr++) {
				if (itr->movable) {
					Ogre::Entity *pointedEntity = sceneManager->getEntity(itr->movable->getName());
					if (pointedEntity) {
						std::string nodeName = pointedEntity->getParentSceneNode()->getName();
						if (nodeName.substr(0, 3) == "_S_") {
							pointedEntity->setMaterialName("Blau");
						}
						else if (nodeName.substr(0, 3) == "_P_") {
							pointedEntity->setMaterialName("Gelb");
						}
					}
					break;
				}
			}
		} // ~selection code

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
		kingNode->setPosition(
			-1 + std::cos(dancersAngleIncrement) * RADIUS, 
			kingNode->getPosition()[1], 
			-8 + std::sin(dancersAngleIncrement) * RADIUS);

		queenNodeTwo->setPosition(
		1 - std::cos(dancersAngleIncrement) * RADIUS, 
		queenNodeTwo->getPosition()[1], 
		-8 - std::sin(dancersAngleIncrement) * RADIUS);

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

	OgreBites::ApplicationContext ctx("Ogre-Rook");
	ctx.initApp();

	ctx.getRenderWindow()->setVSyncEnabled(true);

	root = ctx.getRoot();
	sceneManager = root->createSceneManager();
	
	RookFrameListener *rookFrameListener = new RookFrameListener(*sceneManager, ctx.getRenderWindow()->getWidth(), ctx.getRenderWindow()->getHeight());
	root->addFrameListener(rookFrameListener);

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
	cam = sceneManager->createCamera("myCam");
	cam->setNearClipDistance(Ogre::Real(0.1));
	cam->setFarClipDistance(FAR_CLIP);
	cam->setAutoAspectRatio(true);
	camNode->attachObject(cam);
	camNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
	camNode->setPosition(CAM_POSITION);
	camNode->lookAt(LOOK_AT_POINT, Ogre::Node::TS_WORLD);
	ctx.getRenderWindow()->addViewport(cam);

	// Ray Query
	raySceneQuery = sceneManager->createRayQuery(Ogre::Ray());



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

	kingEntity = sceneManager->createEntity("king.mesh");
	kingEntity->setMaterial(darkMonochromeOrderedDitherMaterial);
	kingNode = sceneManager->getRootSceneNode()->createChildSceneNode("DitheredKing");
	kingNode->setPosition(-1, 0, 10);
	kingNode->attachObject(kingEntity);

	queenEntityTwo = sceneManager->createEntity("queen.mesh");
	queenEntityTwo->setMaterial(monochromeOrderedDitherMaterial);
	queenNodeTwo = sceneManager->getRootSceneNode()->createChildSceneNode("DitheredQueen");
	queenNodeTwo->setPosition(1, 0, 10);
	queenNodeTwo->attachObject(queenEntityTwo);

	
	Ogre::MaterialPtr gelbMaterial = Ogre::MaterialManager::getSingleton().getByName("Gelb");
	Ogre::MaterialPtr blauMaterial = Ogre::MaterialManager::getSingleton().getByName("Blau");


    createBoard(sceneManager, monochromeOrderedDitherMaterial, blackMonochromeOrderedDitherMaterial);
	setPieceOnBoard(WHITE_ROOK, 0, 0, sceneManager, monochromeOrderedDitherMaterial, darkMonochromeOrderedDitherMaterial);
	setPieceOnBoard(BLACK_KNIGHT, 1, 0, sceneManager, monochromeOrderedDitherMaterial, darkMonochromeOrderedDitherMaterial);
	setPieceOnBoard(BLACK_BISHOP, 2, 0, sceneManager, monochromeOrderedDitherMaterial, darkMonochromeOrderedDitherMaterial);


    // sky
	sceneManager->setSkyBox(true, "TrippySkyBox", 100, false);

	// keys
	KeyHandler keyHandler(*sceneManager, *raySceneQuery, ctx.getRenderWindow()->getWidth(), ctx.getRenderWindow()->getHeight());
	ctx.addInputListener(&keyHandler);

	// Ogre loop
	ctx.getRoot()->startRendering();

	// free Ogre objects
	delete rookFrameListener;
	sceneManager->destroyQuery(raySceneQuery);
	root->destroySceneManager(sceneManager);
	
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
            std::string nodeName =  std::string("_S_").append(std::string(1, 'a' + x).append(std::to_string(y + 1)));
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
		sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode(std::string("_P_") + std::to_string(piece));
		sceneNode->setPosition(-2, 0, 0);
		sceneNode->attachObject(entity);
		
		Piece p = { piece, entity, sceneNode };
		piecesMap.insert(std::pair<PIECE_ENUM, Piece>(piece, p));
	}
	sceneNode->setPosition(X_START + x * SQUARE_WIDTH * 2, 0 + BOARD_THICKNESS, Y_START - y * SQUARE_HEIGHT * 2);
}

