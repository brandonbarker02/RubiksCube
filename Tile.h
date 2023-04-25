//
// Created by Aidan on 3/16/2023.
//

#ifndef RUBIKSMODEL_TILE_H
#define RUBIKSMODEL_TILE_H


#include <string>
#include <vector>
#include <iostream>

class Tile {
private:
    // private fields which characterize a given tile (sticker)
    char color;
    int face, row, column;
    std::vector<Tile*> tileVector;
public:
    // ------------------- METHODS TO ACCESS / SET A TILE'S ATTRIBUTES --------------------------------------
    void setFace(int face); // sets the face that the given tile is on (used during construction of the cube)
    int getFace(); // returns the face that the given tile is on
    void setRow(int row); // sets the row that a given tile is on (used during construction)
    int getRow(); // returns the row that a given tile is on
    void setColumn(int column); // sets the column that a given tile is on (used during construction)
    int getColumn(); // returns the column that a given tile is on
    void setColor(char val); // allows the color field of a given tile to be changed
    char getColor(); // returns the color field of a given tile
    void displayLocation(); // prints a tile's location information
    // ------------------------------------------------------------------------------------------------------

    // ------------------------------- METHODS RELATING TO A TILE'S POINTERS (ADJACENT TILES) ------------------------------------------------------------------------------------
    void expandTV(int num); // resizes the tile pointer vector to fit adjacent tiles, when necessary (saves space from unnecessary allocation)
    bool setTV(int position, Tile* ptr); // sets a given tile in the pointer vector during declaration or rotation
    Tile* getTV(int position); // returns a given tile in the pointer vector for access
    int getTVSize(); // returns the size of a given tile's vector of pointers | use in identifying a tile, i.e., center piece size = 12; corner piece size = 2; edge piece size = 1
    void printTV(); // prints a center tile's tile vector for debugging
    // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
};

#endif //RUBIKSMODEL_TILE_H