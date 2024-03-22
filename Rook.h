#pragma once

#define M_PI 3.14159265358979323846

// Globals
constexpr int OBJECT_LIFE_TIME = 300;
constexpr int WAIT_FRAMES = 0;
constexpr float RADIUS = 2.0f;
constexpr float FAR_CLIP = 500.0f;
constexpr int REFRESH_RATE = 75;
constexpr float BOARD_THICKNESS = .2f;
constexpr float SQUARE_WIDTH = 2.0f;
constexpr float SQUARE_HEIGHT = 2.0f;
constexpr int X_START = -14;
constexpr int Y_START = 0;
constexpr float X_CENTER = 0.0f;
constexpr float Y_CENTER = -14.0f;


int tick = 0;
int waitedFrames = 0;
float dancersAngleIncrement = 0.0f;

const float ZOOM_INCREMENT = 1.0f;
const float ZOOM_MAX = 15.0f;
const float ZOOM_MIN = -5.0f;
const float CAM_HEIGHT = 6.0f;
const float Y_ROTATION_MAX = 1.72f;
const float Y_ROTATION_MIN = 0.04f;
float xRotation = 0.0f;
float yRotation = 0.0f;
float zTranslate = 0.0f;
float X_ROTATION_INCREMENT = .05f;
float Y_ROTATION_INCREMENT = .05f;

//uint32_t width = 0;
//uint32_t height = 0;

Ogre::Vector3 LOOK_AT_POINT = Ogre::Vector3(X_CENTER, CAM_HEIGHT, Y_CENTER);
Ogre::Vector3 CAM_POSITION = Ogre::Vector3(0, 6, 26);

Ogre::Root *root;
Ogre::SceneManager *sceneManager;
Ogre::RaySceneQuery *raySceneQuery;
Ogre::Entity *kingEntity;
Ogre::Entity *queenEntityTwo;
Ogre::SceneNode *kingNode;
Ogre::SceneNode *queenNodeTwo;
Ogre::SceneNode *camNode;
Ogre::Camera *cam;
Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

enum PIECE_ENUM { 
	WHITE_PAWN = 0, WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING,
	BLACK_PAWN, BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING,
	EMPTY_SQUARE, UNKNOWN
};

struct PieceStruct {
	PIECE_ENUM piece;
	Ogre::Entity *entity;
	Ogre::SceneNode *node;
	int x, y;
};
typedef PieceStruct Piece;

std::map<PIECE_ENUM, Piece> piecesMap;
Ogre::Vector3 arbitraryRotate(Ogre::Vector3 p, Ogre::Real theta, Ogre::Vector3 p1, Ogre::Vector3 p2);
void createBoard(Ogre::SceneManager *sceneManager, Ogre::MaterialPtr &whiteMaterialPtr, Ogre::MaterialPtr &blackMaterialPtr);
void setPieceOnBoard(PIECE_ENUM piece, int x, int y, Ogre::SceneManager *sceneManager, Ogre::MaterialPtr &whiteMaterialPtr, Ogre::MaterialPtr &blackMaterialPtr);
void rotateAroundLookAtPointThenTranslateZ();
