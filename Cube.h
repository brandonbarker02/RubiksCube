//
// Created by Aidan on 3/16/2023.
// Class which models a Rubik's cube as a 3-dimensional (6 x 3 x 3) array, modeling the 6 faces of the cube, each with 3 rows and 3 columns.
// Faces are identified in 3 ways: by integer, color, or direction:
// 1. Integer: White = 0; Red = 1; Blue = 2; Orange = 3; Green = 4; Yellow = 5
// 2. Color: White = 'W'; Red = 'R'; Blue = 'B'; Orange = 'O'; Green = 'G'; Yellow = 'Y'
// 3. Direction: Configured using the orientCube() function, which maps: Front = 'F'; Left = 'L'; Back = 'B'; Right = 'R'; Up = 'U'; Down = 'D' to the faces specified for easier application of algorithms (Used for most methods)
//

#ifndef RUBIKSMODEL_CUBE_H
#define RUBIKSMODEL_CUBE_H
#include <vector>
#include <unistd.h>
#include <iostream>
#include <unordered_map>
#include <set>
#include <utility>
#include <stdlib.h>
#include <string>
#include "Tile.h"
#include "Algorithms.h"

class Cube {
private:
    Tile tileArray[6][3][3]; // the cube itself
    int numRotations; // incremented after each rotation is called
    char frontFace; //keeps track of front
    char topFace; //keeps track of top
    std::unordered_map<char,int> rotationMap = {}; // maps directional identifiers (i.e. 'F' = front) to some face of the cube (arbitrary)
    std::unordered_map<int,char> colorBoard = {{0, 'W'}, {1, 'R'}, {2, 'B'}, {3, 'O'}, {4, 'G'}, {5, 'Y'}}; // maps color identifiers to their face of the cube (objective)
    std::unordered_map<char, char> opposite = {{'O', 'R'}, {'R', 'O'}, {'Y', 'W'}, {'W', 'Y'}, {'B', 'G'}, {'G', 'B'}}; //get the color opposite a given color
    const std::set<std::vector<char>> allCorners = {{'W', 'G', 'R'}, {'W', 'R', 'B'}, {'W', 'B', 'O'}, {'W', 'O', 'G'},
                                                    {'Y', 'G', 'R'}, {'Y', 'R', 'B'}, {'Y', 'B', 'O'}, {'Y', 'O', 'G'}};
    const std::set<std::pair<char, char>> allEdges = {{'W', 'G'}, {'W', 'O'}, {'W', 'B'}, {'W', 'R'},
                                                      {'Y', 'G'}, {'Y', 'O'}, {'Y', 'B'}, {'Y', 'R'},
                                                      {'R', 'B'},  {'O', 'B'},  {'O', 'G'},  {'R', 'G'}}; //set of all edges
    void linkCorner(Tile* tile1, Tile* tile2, Tile* tile3); // called to link three tiles' pointers together where they form a corner
    void linkTiles(Tile* tile1, Tile* tile2, int pos1, int pos2); // called to link two tiles' pointers together where they form an edge
    //vector that keeps track of all moves done
    std::vector<std::string> movesDone;
public:

    Cube(); // constructor for the cube

    // ------------------------------------------------- METHODS TO CHANGE THE CUBE -----------------------------------------------------------
    std::vector<std::string> findCommon(std::vector<std::vector<char>> charValues, int offset); // used in printing the cube
    void orientCube(char topFace, char frontFace); // orients the cube (configures the rotationMap) so that 'topFace' is facing up and 'frontFace' is facing frontward (takes a COLOR identifier as parameter)
    void clockwiseRotation(char side); // rotates a given face of the cube clockwise (uses a DIRECTIONAL identifier as parameter)
    void counterClockwiseRotation(char side); // rotates a given face counterclockwise (uses a DIRECTIONAL identifier as parameter)
    void manualConfigure(int side, int A, int B, char mc); // allows manual configuration of the color of a given sticker for debugging (uses an INTEGER identifier as parameter)
    void manualConfigure(Tile *tile, char mc); // allows manual configuration of the color of a given sticker for debugging (uses a TILE as parameter)
    void setNumRotations(int rot); //allows manual setting of numRotations
    // -----------------------------------------------------------------------------------------------------------------------------------------

    // ------------------------------------------------- METHODS TO CHECK THE STATE OF THE CUBE ---------------------------------------------------------------------------------------------------------------------------------------------------
    bool isSolved(); // checks if a cube is solved
    Tile* findEdge(char sideA, char sideB); // returns an edge given the two colors that form it | the tile it returns is the sideA tile, i.e., passing in (red, blue) will return the red tile of the red-blue edge, and vice versa. (Takes COLOR parameters)
    Tile* findCorner(char sideA, char sideB, char sideC); // returns a corner given the three colors that form it | the returned tile is the sideA tile, i.e., passing in (red, blue, white) will return the red tile of the red-blue-white corner. (Takes COLOR parameters)
    void displayColors(); // "prints" the given cube for debugging*
    /* the format of the printed cube is like a net. So imagine folding all the sides inward, together, to form the cube, to visualize where each tile is
     * for example, the top left white tile is the white tile on the red-green-white corner, etc. */
    int getNumRotations(); // returns the numRotations value to analyze effectiveness of algorithm
    // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // ------------------------Scrambling/Solving----------------------
    std::string scramble(int numMoves);
    void solveCross();
    void solveCrossEdge(char sideA, char sideB);
    void solveBottomCorners();
    void solveBottomCorner(char sideA, char sideB, char sideC);
    void solveMiddleEdges();
    void solveMiddleEdge(char sideA, char sideB);
    void orientTopEdges();
    void solveTopEdges();
    void permuteTopCorners();
    bool correctCornerPerumtation(Tile* corner);
    void solveTopCorners();
    void beginnersMethodSolve();
    void algorithm(const std::vector<std::string> &alg);
    void reverseAlgorithm(const std::vector<std::string> &alg);
    void wideD();
    void counterWideD();
    void wideL();
    void counterWideL();
    std::set<std::pair<char, char>> getUnsolvedEdges();
    void oldPochmannEdges();
    std::set<std::vector<char>> getUnsolvedCorners();
    bool cornerContains(char color, std::vector<char> &corner);
    void oldPochmannCorners();
    void oldPochmannSolve();
    std::vector<std::string> getMovesDone();
};


#endif //RUBIKSMODEL_CUBE_H
