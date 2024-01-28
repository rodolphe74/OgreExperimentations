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

void createBoard(Ogre::SceneManager *sceneManager, Ogre::MaterialPtr &monochromeOrderedDitherMaterial, Ogre::MaterialPtr &darkMonochromeOrderedDitherMaterial);
void setPieceOnBoard(PIECE_ENUM piece, int x, int y, Ogre::SceneManager *sceneManager, Ogre::MaterialPtr &whiteMaterialPtr, Ogre::MaterialPtr &blackMaterialPtr);
