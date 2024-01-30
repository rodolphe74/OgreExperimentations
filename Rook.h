#pragma once

// Globals
constexpr int OBJECT_LIFE_TIME = 300;
constexpr int WAIT_FRAMES = 0;
constexpr float RADIUS = 2.0f;
constexpr float FAR_CLIP = 500.0f;
constexpr int REFRESH_RATE = 75;
constexpr float BOARD_THICKNESS = .2f;
constexpr float SQUARE_WIDTH = 2.0f;
constexpr float SQUARE_HEIGHT = 2.0f;
constexpr int X_START = -13;
constexpr int Y_START = 0;
// constexpr int X_CENTER = (X_START + 2.0f * 8) / 2.0f;
// constexpr int Y_CENTER = (Y_START - 2.0f * 8) / 2.0f;
constexpr float X_CENTER = 1.5f;
constexpr float Y_CENTER = -14.0f;

int tick = 0;
int waitedFrames = 0;
float dancersAngleIncrement = 0.0f;

const float ZOOM_INCREMENT = 1.0f;
const float ZOOM_MAX = 35.0f;
const float ZOOM_MIN = 20.0f;
const float CAM_HEIGHT = 6.0f;
float CAM_X_RADIUS = 35.0f;
float xRotation = 0.0f;
float X_ROTATION_INCREMENT = .05f;

Ogre::Entity *rookEntity;
Ogre::Entity *rookEntityTwo;
Ogre::SceneNode *rookNode;
Ogre::SceneNode *rookNodeTwo;
Ogre::SceneNode *camNode;


enum PIECE_ENUM { 
	WHITE_PAWN = 0, WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING,
	BLACK_PAWN, BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING
};

struct PieceStruct {
	PIECE_ENUM piece;
	Ogre::Entity *entity;
	Ogre::SceneNode *node;
};
typedef PieceStruct Piece;

std::map<PIECE_ENUM, Piece> piecesMap;
void createBoard(Ogre::SceneManager *sceneManager, Ogre::MaterialPtr &whiteMaterialPtr, Ogre::MaterialPtr &blackMaterialPtr);
void setPieceOnBoard(PIECE_ENUM piece, int x, int y, Ogre::SceneManager *sceneManager, Ogre::MaterialPtr &whiteMaterialPtr, Ogre::MaterialPtr &blackMaterialPtr);
