//

// Created by Aidan on 3/16/2023.
//

#include "Cube.h"

Cube::Cube(){
    srand (time(NULL));
    numRotations = 0;
    for (int i = 0; i < 6; i++){ // 6 faces of the cube - white, red, blue, orange, green, yellow
        for (int j = 0; j < 3; j++){ // 3 rows of each face
            for (int k = 0; k < 3; k++){ // 3 columns of each face
                tileArray[i][j][k].setColor(colorBoard[i]); // setting each tile's fields
                tileArray[i][j][k].setFace(i);
                tileArray[i][j][k].setRow(j);
                tileArray[i][j][k].setColumn(k);
                if (j == 1 && k == 1) // center piece
                    tileArray[i][j][k].expandTV(12);
                else if ((j == 0 && k == 1) || (j == 1 && k == 0) || (j == 1 && k == 2) || (j == 2 && k == 1)) // edge piece
                    tileArray[i][j][k].expandTV(1);
                else if ((j == 0 && k == 0) || (j == 0 && k == 2) || (j == 2 && k == 0) || (j == 2 && k == 2)) // corner piece
                    tileArray[i][j][k].expandTV(2);
            }
        }
    }

    /* mapping the pieces to each other using pointers
     * this allows for individual pieces to be tracked, identified, and located */

    // map edge tiles to each other using pointers
    // white face's edges
    linkTiles(&tileArray[0][0][1], &tileArray[4][0][1], 0, 0);
    linkTiles(&tileArray[0][1][0], &tileArray[1][0][1], 0, 0);
    linkTiles(&tileArray[0][1][2], &tileArray[3][0][1], 0, 0);
    linkTiles(&tileArray[0][2][1], &tileArray[2][0][1], 0, 0);

    // yellow face's edges
    linkTiles(&tileArray[5][0][1], &tileArray[2][2][1], 0, 0);
    linkTiles(&tileArray[5][1][0], &tileArray[1][2][1], 0, 0);
    linkTiles(&tileArray[5][1][2], &tileArray[3][2][1], 0, 0);
    linkTiles(&tileArray[5][2][1], &tileArray[4][2][1], 0, 0);

    // middle edges
    linkTiles(&tileArray[2][1][0], &tileArray[1][1][2], 0, 0);
    linkTiles(&tileArray[2][1][2], &tileArray[3][1][0], 0, 0);
    linkTiles(&tileArray[4][1][0], &tileArray[3][1][2], 0, 0);
    linkTiles(&tileArray[4][1][2], &tileArray[1][1][0], 0, 0);

    // map corner tiles to each other using pointers
    // white corners
    linkCorner(&tileArray[0][0][0], &tileArray[1][0][0], &tileArray[4][0][2]);
    linkCorner(&tileArray[0][0][2], &tileArray[3][0][2], &tileArray[4][0][0]);
    linkCorner(&tileArray[0][2][0], &tileArray[2][0][0], &tileArray[1][0][2]);
    linkCorner(&tileArray[0][2][2], &tileArray[3][0][0], &tileArray[2][0][2]);

    // yellow corners
    linkCorner(&tileArray[5][0][0], &tileArray[2][2][0], &tileArray[1][2][2]);
    linkCorner(&tileArray[5][0][2], &tileArray[3][2][0], &tileArray[2][2][2]);
    linkCorner(&tileArray[5][2][0], &tileArray[1][2][0], &tileArray[4][2][2]);
    linkCorner(&tileArray[5][2][2], &tileArray[3][2][2], &tileArray[4][2][0]);

    // map center tiles to the tiles bordering the face
    for (int i = 4; i >= 1; i--){
        for (int j = 0; j < 3; j++){
            tileArray[0][1][1].setTV(12 - (i * 3) + j, &tileArray[i][0][j]);
        }
    }
    for (int i = 1; i <= 4; i++){
        for (int j = 0; j < 3; j++){
            tileArray[5][1][1].setTV((i-1) * 3 + j, &tileArray[i][2][j]);
        }
    }
    for (int i = 1; i <= 4; i++){
        int left, right;
        left = i-1;
        right = i+1;
        if (left == 0)
            left = 4;
        if (right == 5)
            right = 1;
        // left and right now refer to those non-white and non-yellow faces relative to our current face i (with the white face oriented upward), specifically its center piece [i][j][k], or i[1][1].

        // creates tile vector of pointers to the 12 stickers surrounding current face i, storing it in the center tile of face i.
        // this is useful to track the edges which are being rotated in a given rotation
        std::vector<std::pair<int,int>> whiteMap = {{0,0}, {1,0}, {2,0}, {2,0}, {2,1}, {2,2}, {2,2}, {1,2}, {0,2}, {0,2}, {0,1}, {0,0}};
        std::vector<std::pair<int,int>> yellowMap = {{0,0}, {1,0}, {2,0}, {0,2}, {0,1}, {0,0}, {2,2}, {1,2}, {0,2}, {2,0}, {2,1}, {2,2}};
        for (int l = 0; l < 3; l++){
            tileArray[i][1][1].setTV(l, &tileArray[left][2-l][2]); // left edges
            tileArray[i][1][1].setTV(l+3, &tileArray[0][whiteMap.at((i-1)*3 + l).first][whiteMap.at((i-1)*3 + l).second]); // white edges
            tileArray[i][1][1].setTV(l+6, &tileArray[right][l][0]); // right edges
            tileArray[i][1][1].setTV(l+9, &tileArray[5][yellowMap.at((i-1)*3 + l).first][yellowMap.at((i-1)*3 + l).second]); // yellow edges
        }
        // this structure storing the 12 edges around a given face sequentially means any rotation only needs to rotate these previously stored pointers.
    }
    // default orientation is white face up, blue face frontward
    this->orientCube('W','B');
}

void Cube::orientCube(char topFace, char frontFace){
    //add it to moves vector
    std::string s;
    s.push_back(topFace);
    s.push_back(frontFace);
    movesDone.push_back(s);

    std::unordered_map<char, int> colorToInt = {{'W',0}, {'R',1}, {'B',2}, {'O',3}, {'G',4}, {'Y',5}};
    this->frontFace = frontFace;
    this->topFace = topFace;
    int top = colorToInt[topFace];
    int front = colorToInt[frontFace];

    // we need to derive down, back, left, right, based on the parameters
    std::unordered_map<int,int> reverseSide = {{0,5}, {1,3}, {2,4}, {3,1}, {4,2}, {5,0}};
    rotationMap['F'] = front;
    rotationMap['B'] = reverseSide[front];
    rotationMap['U'] = top;
    rotationMap['D'] = reverseSide[top];

    // maps a given face to a clockwise-oriented vector of its adjacent sides | for any of these vectors, the number prior to the topFace will be the leftFace
    std::unordered_map<int,std::vector<int>> adjMap = {{0, {4, 3, 2, 1}}, {1, {4, 0, 2, 5}}, {2, {1, 0, 3, 5}}, {3, {2, 0, 4, 5}}, {4, {3, 0, 1, 5}}, {5, {1, 2, 3, 4}}};
    int leftFace;
    for (int i = 0; i < adjMap[front].size(); i++){
        if (adjMap[front].at(i) == top)
            leftFace = i-1;
    }
    if (leftFace == -1)
        leftFace = 3;
    if (leftFace == 4)
        leftFace = 0;
    leftFace = adjMap[front].at(leftFace);

    rotationMap['L'] = leftFace;
    rotationMap['R'] = reverseSide[leftFace];
}

void Cube::clockwiseRotation(char side){
    //add it to moves vector
    std::string s = "";
    s.push_back(side);
    movesDone.push_back(s);

    // derive the face based on our rotationMap
    int face = rotationMap[side];

    numRotations++;
    // remap the stickers on the given face AND the stickers on the adjacent row of the four neighboring faces (four of these rows in total)

    // 1. remap tileArray[face]

    // first iteration rotates the corner stickers
    // second iteration rotates the edge stickers
    for (int i = 0; i < 2; i++){
        char temp = tileArray[face][0][i].getColor();
        tileArray[face][0][i].setColor(tileArray[face][2-i][0].getColor());
        tileArray[face][2-i][0].setColor(tileArray[face][2][2-i].getColor());
        tileArray[face][2][2-i].setColor(tileArray[face][i][2].getColor());
        tileArray[face][i][2].setColor(temp);
    }

    // 2. remap the adjacent rows

    // rotate this vector according to the clockwise rotation of the face
    char temp1 = tileArray[face][1][1].getTV(9)->getColor();
    char temp2 = tileArray[face][1][1].getTV(10)->getColor();
    char temp3 = tileArray[face][1][1].getTV(11)->getColor();

    for (int i = 3; i > 0; i--){
        tileArray[face][1][1].getTV(i * 3)->setColor(tileArray[face][1][1].getTV((i-1)*3)->getColor());
        tileArray[face][1][1].getTV(i * 3 + 1)->setColor(tileArray[face][1][1].getTV((i-1)*3 + 1)->getColor());
        tileArray[face][1][1].getTV(i * 3 + 2)->setColor(tileArray[face][1][1].getTV((i-1)*3 + 2)->getColor());
    }
    tileArray[face][1][1].getTV(0)->setColor(temp1);
    tileArray[face][1][1].getTV(1)->setColor(temp2);
    tileArray[face][1][1].getTV(2)->setColor(temp3);

    //for debugging
    //std::cout << /*std::endl <<*/ "rotation: " << side << std::endl;
    //displayColors();
}

void Cube::counterClockwiseRotation(char side){
    //add it to moves vector
    std::string s = "";
    s.push_back(side);
    s.push_back('\'');
    movesDone.push_back(s);

    // derive the face based on our rotationMap
    int face = rotationMap[side];

    numRotations++;
    // remap the stickers on the given face AND the stickers on the adjacent row of the four neighboring faces (four of these rows in total)

    // 1. remap tileArray[face]

    // first iteration rotates the corner stickers
    // second iteration rotates the edge stickers
    for (int i = 0; i < 2; i++){
        char temp = tileArray[face][0][i].getColor();
        tileArray[face][0][i].setColor(tileArray[face][i][2].getColor());
        tileArray[face][i][2].setColor(tileArray[face][2][2-i].getColor());
        tileArray[face][2][2-i].setColor(tileArray[face][2-i][0].getColor());
        tileArray[face][2-i][0].setColor(temp);
    }

    // 2. remap the adjacent rows

    // rotate this vector according to the clockwise rotation of the face
    char temp1 = tileArray[face][1][1].getTV(0)->getColor();
    char temp2 = tileArray[face][1][1].getTV(1)->getColor();
    char temp3 = tileArray[face][1][1].getTV(2)->getColor();
    for (int i = 0; i < 3; i++){
        tileArray[face][1][1].getTV(i * 3)->setColor(tileArray[face][1][1].getTV((i+1)*3)->getColor());
        tileArray[face][1][1].getTV(i * 3 + 1)->setColor(tileArray[face][1][1].getTV((i+1)*3 + 1)->getColor());
        tileArray[face][1][1].getTV(i * 3 + 2)->setColor(tileArray[face][1][1].getTV((i+1)*3 + 2)->getColor());
    }
    tileArray[face][1][1].getTV(9)->setColor(temp1);
    tileArray[face][1][1].getTV(10)->setColor(temp2);
    tileArray[face][1][1].getTV(11)->setColor(temp3);

    //for debugging
    //std::cout << /*std::endl <<*/ "rotation: " << side << "\'" << std::endl;
    //displayColors();
}

void Cube::manualConfigure(int side, int A, int B, char mc){
    tileArray[side][A][B].setColor(mc);
}

void Cube::manualConfigure(Tile *tile, char mc){
    tile->setColor(mc);
}

bool Cube::isSolved(){
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 3; j++){
            for (int k = 0; k < 3; k++){
                if (tileArray[i][j][k].getColor() != colorBoard[i])
                    return false;
            }
        }
    }
    return true;
}

Tile* Cube::findEdge(char sideA, char sideB){
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 3; j++){
            for (int k = 0; k < 3; k++){
                if (tileArray[i][j][k].getColor() == sideA && tileArray[i][j][k].getTVSize() == 1 && tileArray[i][j][k].getTV(0)->getColor() == sideB)
                    return &tileArray[i][j][k];
            }
        }
    }
    return nullptr;
}

Tile* Cube::findCorner(char sideA, char sideB, char sideC){
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 3; j++){
            for (int k = 0; k < 3; k++){
                if (tileArray[i][j][k].getColor() == sideA && tileArray[i][j][k].getTVSize() == 2 && ((tileArray[i][j][k].getTV(0)->getColor() == sideB && tileArray[i][j][k].getTV(1)->getColor() == sideC) ||
                                                                                                      (tileArray[i][j][k].getTV(0)->getColor() == sideC && tileArray[i][j][k].getTV(1)->getColor() == sideB))) // .at(0) and .at(1) need to be sideB and sideC
                    return &tileArray[i][j][k];
            }
        }
    }
    return nullptr;
}

void Cube::displayColors(){
    findCommon({{'U', 'B', 'L', 'R'}}, 4); // prints 'U' face
    findCommon({{'L','U','B','F'},{'F','U','L','R'},{'R','U','F','B'},{'B','U','L','R'}}, 0); // prints middle 4 faces
    findCommon({{'D','F','L','R'}}, 4); // prints 'D' face
}



int Cube::getNumRotations(){
    return numRotations;
}

void Cube::linkCorner(Tile* tile1, Tile* tile2, Tile* tile3){
    linkTiles(tile1, tile2, 0, 0);
    linkTiles(tile1, tile3, 1, 0);
    linkTiles(tile2, tile3, 1, 1);
}

void Cube::linkTiles(Tile* tile1, Tile* tile2, int pos1, int pos2){
    tile1->setTV(pos1, tile2);
    tile2->setTV(pos2, tile1);
}

std::vector<std::string> Cube::findCommon(std::vector<std::vector<char>> charValues, int offset){

    //string to return
    std::string returnString = "";
    // the order that we print a given face's array depends on that face's orientation | layoutMap will be used to determine which printing approach will be used for a given face
    std::unordered_map<int, std::pair<int,int>> layoutMap;
    for (int x = 0; x < charValues.size(); x++) { // iterates thru each face we have passed in
        Tile *tile1;
        Tile *tile2;
        char givenEdge = charValues.at(x)[0];
        char commonEdge = charValues.at(x)[1];
        char edge1 = charValues.at(x)[2];
        char edge2 = charValues.at(x)[3];
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 12; j++) {
                if (tileArray[rotationMap[edge1]][1][1].getTV(i) == tileArray[rotationMap[commonEdge]][1][1].getTV(j) && (tileArray[rotationMap[edge1]][1][1].getTV(i)->getFace() == rotationMap[givenEdge])) {
                    tile1 = tileArray[rotationMap[edge1]][1][1].getTV(i); // stores the 'top left' tile of the given face
                }
            }
        }
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 12; j++) {
                if (tileArray[rotationMap[edge2]][1][1].getTV(i) == tileArray[rotationMap[commonEdge]][1][1].getTV(j) && (tileArray[rotationMap[edge2]][1][1].getTV(i)->getFace() == rotationMap[givenEdge])) {
                    tile2 = tileArray[rotationMap[edge2]][1][1].getTV(i); // stores the 'top right' tile of the given face
                }
            }
        }

        // using the tiles we have derived, find their orientation relative to the face's array so we can apply proper printing approach
        std::pair<int, int> pr;
        if (tile1->getRow() == tile2->getRow()) { // the tiles are in the same row
            pr = {tile1->getRow(), 3};
        } else { // the tiles are in the same column
            pr = {3, tile1->getColumn()};
        }
        layoutMap[charValues.at(x).at(0)] = pr; // store the printing approach for each given face
    }

    // used for indentation of a printed face (formatting)
    std::string whitespace = "";
    for (int i = 0; i < offset; i++){
        whitespace += " ";
    }

    // based on a face's layout (stored in layoutMap), print that face
    for (int i = 0; i <= 2; i++) { // iterate through each row of the face(s) we are printing
        for (int x = 0; x < charValues.size(); x++) { // iterate through each face
            //std::cout << whitespace;
            for (int j = 0; j <= 2; j++) { // iterate through the columns
                if (layoutMap[charValues.at(x).at(0)].first == 3 && layoutMap[charValues.at(x).at(0)].second == 0) { // the "top row" is the 0th column in the tileArray
                    //std::cout << tileArray[rotationMap[charValues.at(x).at(0)]][2 - j][i].getColor();
                    returnString.push_back(tileArray[rotationMap[charValues.at(x).at(0)]][2 - j][i].getColor());
                }
                if (layoutMap[charValues.at(x).at(0)].first == 3 && layoutMap[charValues.at(x).at(0)].second == 2) { // the "top row" is the 2nd column in the tileArray
                    //std::cout << tileArray[rotationMap[charValues.at(x).at(0)]][j][2 - i].getColor();
                    returnString.push_back(tileArray[rotationMap[charValues.at(x).at(0)]][j][2 - i].getColor());
                }
                if (layoutMap[charValues.at(x).at(0)].first == 0 && layoutMap[charValues.at(x).at(0)].second == 3) { // the "top row" is the 0th row in the face in the tileArray
                    //std::cout << tileArray[rotationMap[charValues.at(x).at(0)]][i][j].getColor();
                    returnString.push_back(tileArray[rotationMap[charValues.at(x).at(0)]][i][j].getColor());
                }
                if (layoutMap[charValues.at(x).at(0)].first == 2 && layoutMap[charValues.at(x).at(0)].second == 3) { // the "top row" is the 2nd row in the face in the tileArray
                    //std::cout << tileArray[rotationMap[charValues.at(x).at(0)]][2 - i][2 - j].getColor();
                    returnString.push_back(tileArray[rotationMap[charValues.at(x).at(0)]][2 - i][2 - j].getColor());
                }
            }
            //std::cout << " ";
        }
        //std::cout << std::endl;
    }
    std::string current = "";
    std::vector<std::string> returnVector;
    for (int i = 0; i < returnString.length(); ++i) {
        current.push_back(returnString[i]);
        if (i % 3 == 2) {
            returnVector.push_back(current);
            //std::cout << "returning string: " << current << std::endl;
            current = "";
        }
    }
    return returnVector;
}

std::string Cube::scramble(int numMoves) {
    numRotations = 0;
    movesDone.clear();
    std::unordered_map<int, char> faces = {{0, 'F'}, {1, 'B'},
                                           {2, 'R'}, {3, 'L'},
                                           {4, 'U'}, {5, 'D'}};
    std::string scrambleChars = "";

    for (int i = 0; i < numMoves; i++) {
        char element = rand() % 6;
        if (rand() & 2) {
            clockwiseRotation(faces[element]);
            scrambleChars.push_back(faces[element]);
        }
        else {
            counterClockwiseRotation(faces[element]);
            scrambleChars.push_back(faces[element]);
            scrambleChars.push_back('\'');
        }
        scrambleChars.push_back(' ');

        //std::cout << std::endl << "move: " << faces[element] << std::endl;
        //displayColors();
    }
    return scrambleChars;
}

void Cube::solveCross() {
    solveCrossEdge('G', 'W');
    solveCrossEdge('O', 'W');
    solveCrossEdge('B', 'W');
    solveCrossEdge('R', 'W');
}

void Cube::solveCrossEdge(char sideA, char sideB) {
    //orient correctly
    orientCube(opposite[sideB], sideA);

    //solve green edge
    Tile* side = findEdge(sideA, sideB);
    Tile* bottom = findEdge(sideB, sideA);

    //if on the bottom
    if (colorBoard[side->getFace()] == sideB) {
        //if on front
        if (bottom->getFace() == rotationMap['F']) {
            clockwiseRotation('F');
            clockwiseRotation('F');
            counterClockwiseRotation('U');
        }
            //if on left
        else if (bottom->getFace() == rotationMap['L']) {
            clockwiseRotation('L');
            clockwiseRotation('L');
            clockwiseRotation('U');
            clockwiseRotation('U');
        }
            //if on right
        else if (bottom->getFace() == rotationMap['R']) {
            clockwiseRotation('R');
            clockwiseRotation('R');
        }
            //if in back
        else if (bottom->getFace() == rotationMap['B']) {
            clockwiseRotation('B');
            clockwiseRotation('B');
            clockwiseRotation('U');
        }

        //solve piece
        counterClockwiseRotation('R');
        clockwiseRotation('F');
        clockwiseRotation('R');
        return;
    }
    //if already in top layer
    if (colorBoard[side->getFace()] == opposite[sideB]) {
        //rotate to right
        //if on left, rotate twice
        if (bottom->getFace() == rotationMap['L']) {
            clockwiseRotation('U');
            clockwiseRotation('U');
        }
            //if on front
        else if (bottom->getFace() == rotationMap['F']){
            counterClockwiseRotation('U');
        }
            //if on back
        else if (bottom->getFace() == rotationMap['B']) {
            clockwiseRotation('U');
        }
        //solve piece
        counterClockwiseRotation('R');
        clockwiseRotation('F');
        clockwiseRotation('R');
        return;
    }
    //if easy solve on bottom
    if (colorBoard[bottom->getFace()] == sideB) {
        //if already solved
        if (colorBoard[side->getFace()] == side->getColor()) {
            return;
        }
        //if on left side
        if (side->getFace() == rotationMap['L']) {
            clockwiseRotation('L');
            clockwiseRotation('L');
        }
            //if on back side
        else if (side->getFace() == rotationMap['B']) {
            clockwiseRotation('B');
            clockwiseRotation('B');
        }
            //if on right side
        else if (side->getFace() == rotationMap['R']) {
            clockwiseRotation('R');
            clockwiseRotation('R');
        }
        //refresh positions
        side = findEdge(sideA, sideB);
        bottom = findEdge(sideB, sideA);

    }

    //if in easy solve position
    if (colorBoard[bottom->getFace()] == opposite[sideB]) {
        //rotate to front
        //if on left
        if (side->getFace() == rotationMap['L']) {
            counterClockwiseRotation('U');
        }
            //if on back
        else if (side->getFace() == rotationMap['B']) {
            clockwiseRotation('U');
            clockwiseRotation('U');
        }
            //if on right
        else if (side->getFace() == rotationMap['R']) {
            clockwiseRotation('U');
        }
        //solve piece
        clockwiseRotation('F');
        clockwiseRotation('F');
        return;
    }
    //program should only get to here if piece is not yet solved, meaning it must be in the middle layer
    //if color part is on back
    if (colorBoard[side->getFace()] == opposite[sideA]) {
        //setup moves
        clockwiseRotation('D');
        clockwiseRotation('D');
        //if on right
        if (bottom->getFace() == rotationMap['L']) {
            clockwiseRotation('B');
        }
            //if on left
        else {
            counterClockwiseRotation('B');
        }
        //reverse setup moves
        clockwiseRotation('D');
        clockwiseRotation('D');
        return;
    }
    //if color part is on front
    if (colorBoard[side->getFace()] == sideA) {
        //if on right
        if (bottom->getFace() == rotationMap['R']) {
            clockwiseRotation('F');
        }
            //if on left
        else {
            counterClockwiseRotation('F');
        }
        return;
    }
    //if in back area
    if (colorBoard[bottom->getFace()] == opposite[sideA]) {
        //if on left face
        if (side->getFace() == rotationMap['L']) {
            //setup move
            counterClockwiseRotation('D');
            //solve piece
            counterClockwiseRotation('L');
            //reverse setup move
            clockwiseRotation('D');
            return;
        }
        //if on right
        //setup move
        clockwiseRotation('D');
        //solve piece
        clockwiseRotation('R');
        //reverse setup move
        counterClockwiseRotation('D');
        return;
    }
    //if it gets here the white part must be on front
    //if on left
    if (side->getFace() == rotationMap['L']) {
        //setup move
        counterClockwiseRotation('D');
        //solve piece
        clockwiseRotation('L');
        //reverse setup move
        clockwiseRotation('D');
        return;
    }
    //if it gets here it must be on right
    //setup move
    clockwiseRotation('D');
    //solve piece
    counterClockwiseRotation('R');
    //reverse setup move
    counterClockwiseRotation('D');
    return;
}

void Cube::solveBottomCorners() {
    solveBottomCorner('W', 'G', 'O');
    solveBottomCorner('W', 'O', 'B');
    solveBottomCorner('W', 'B', 'R');
    solveBottomCorner('W', 'R', 'G');
}

//ex sideA: white, sideB: Green, sideC: Orange
void Cube::solveBottomCorner(char sideA, char sideB, char sideC) {
    orientCube(opposite[sideA], sideB);
    Tile* bottom = findCorner(sideA, sideB, sideC);
    Tile* face = findCorner(sideB, sideA, sideC);
    Tile* side = findCorner(sideC, sideB, sideA);
    bool top = true;
    Tile* flushFace = nullptr;
    if (bottom->getFace() == rotationMap['D']) {
        top = false;
        flushFace = bottom;
        orientCube(opposite[sideA], colorBoard[face->getFace()]);
    }
    else if (side->getFace() == rotationMap['D']) {
        top = false;
        flushFace = side;
        orientCube(opposite[sideA], colorBoard[bottom->getFace()]);
    }
    else if (face->getFace() == rotationMap['D']) {
        top = false;
        flushFace = face;
        orientCube(opposite[sideA], colorBoard[side->getFace()]);
    }
    //if on bottom
    if (!top) {
        //return if already solved
        if (flushFace->getColor() == sideA && face->getFace() == colorBoard[sideB]) {
            return;
        }
        clockwiseRotation('R');
        clockwiseRotation('U');
        counterClockwiseRotation('R');

        //re-orient
        orientCube(opposite[sideA], sideB);

        //update pointers
        bottom = findCorner(sideA, sideB, sideC);
        face = findCorner(sideB, sideA, sideC);
        side = findCorner(sideC, sideB, sideA);
    }

    //now should be on top
    //re-find flush face
    //ex: if "white" is on top
    if (bottom->getFace() == rotationMap['U']) {
        //if to left
        if (side->getFace() == rotationMap['L']) {
            counterClockwiseRotation('U');
        }
            //if to right
        else if (side->getFace() == rotationMap['R']) {
            clockwiseRotation('U');
        }
            //if opposite
        else if (side->getFace() == rotationMap['B']) {
            clockwiseRotation('U');
            clockwiseRotation('U');
        }
        //solve piece
        clockwiseRotation('R');
        clockwiseRotation('U');
        clockwiseRotation('U');
        counterClockwiseRotation('R');
        counterClockwiseRotation('U');
        clockwiseRotation('R');
        clockwiseRotation('U');
        counterClockwiseRotation('R');
    }
        //ex: if "green" is on top
    else if (face->getFace() == rotationMap['U']) {
        //if to left
        if (bottom->getFace() == rotationMap['L']) {
            counterClockwiseRotation('U');
        }
            //if to right
        else if (bottom->getFace() == rotationMap['R']) {
            clockwiseRotation('U');
        }
            //if opposite
        else if (bottom->getFace() == rotationMap['B']) {
            clockwiseRotation('U');
            clockwiseRotation('U');
        }
        //solve piece
        counterClockwiseRotation('F');
        counterClockwiseRotation('U');
        clockwiseRotation('F');

    }
        //ex: if "orange" is on top
    else if (side->getFace() == rotationMap['U']) {
        //if to left
        if (face->getFace() == rotationMap['L']) {
            counterClockwiseRotation('U');
        }
            //if to right
        else if (face->getFace() == rotationMap['R']) {
            clockwiseRotation('U');
        }
            //if opposite
        else if (face->getFace() == rotationMap['B']) {
            clockwiseRotation('U');
            clockwiseRotation('U');
        }
        //solve piece
        clockwiseRotation('R');
        clockwiseRotation('U');
        counterClockwiseRotation('R');
    }
}

void Cube::solveMiddleEdges() {
    //std::cout << "prior to G O" <<std::endl;
    //displayColors();
    solveMiddleEdge('G', 'O');
    //std::cout << "prior to O B" <<std::endl;
    //displayColors();
    solveMiddleEdge('O', 'B');
    //std::cout << "prior to B R" <<std::endl;
    //displayColors();
    solveMiddleEdge('B', 'R');
    //std::cout << "prior to R G" <<std::endl;
    //displayColors();
    solveMiddleEdge('R', 'G');
    //std::cout << std::endl;
}

void Cube::solveMiddleEdge(char sideA, char sideB) {
    orientCube('Y', sideA);

    Tile* face = findEdge(sideA, sideB);
    Tile* side = findEdge(sideB, sideA);

    //if in middle pop up
    if (face->getFace() != rotationMap['U'] && side->getFace() != rotationMap['U']) {
        //if already solved
        if (face->getFace() == sideA && side->getFace() == sideB) {
            return;
        }
        //put edge on front
        orientCube('Y', colorBoard[face->getFace()]);

        //if on left
        if (side->getFace() == rotationMap['L']) {
            algorithm(middleEdgeLeft);
        }
            //if on right
        else {
            algorithm(middleEdgeRight);
        }

        //update pointers
        face = findEdge(sideA, sideB);
        side = findEdge(sideB, sideA);
    }
    //if side is on top
    if (side->getFace() == rotationMap['U']) {
        //face face
        orientCube('Y', sideA);
        //rotate to face
        //if on left
        if (face->getFace() == rotationMap['L']) {
            counterClockwiseRotation('U');
        }
            //if on right
        else if (face->getFace() == rotationMap['R']) {
            clockwiseRotation('U');
        }
            //if in back
        else if (face->getFace() == rotationMap['B']) {
            clockwiseRotation('U');
            clockwiseRotation('U');
        }
        //solve right
        algorithm(middleEdgeRight);
    }
        //if face is on top
    else {
        //face side
        orientCube('Y', sideB);
        //rotate to face
        //if on left
        if (side->getFace() == rotationMap['L']) {
            counterClockwiseRotation('U');
        }
            //if on right
        else if (side->getFace() == rotationMap['R']) {
            clockwiseRotation('U');
        }
            //if in back
        else if (side->getFace() == rotationMap['B']) {
            clockwiseRotation('U');
            clockwiseRotation('U');
        }
        //solve left
        algorithm(middleEdgeLeft);
    }
}

void Cube::orientTopEdges() {
    orientCube('Y', 'G');
    std::vector<Tile*> orientedTiles;
    //repeat till top solved
    while (orientedTiles.size() != 4) {
        orientedTiles.clear();
        //check each edge
        Tile* current = findEdge('Y', 'G');
        if (colorBoard[current->getFace()] == 'Y')
            orientedTiles.push_back(current);
        current = findEdge('Y', 'O');
        if (colorBoard[current->getFace()] == 'Y')
            orientedTiles.push_back(current);
        current = findEdge('Y', 'B');
        if (colorBoard[current->getFace()] == 'Y')
            orientedTiles.push_back(current);
        current = findEdge('Y', 'R');
        if (colorBoard[current->getFace()] == 'Y')
            orientedTiles.push_back(current);
        //break if > 4
        if (orientedTiles.size() == 4) {
            break;
        }
        //perform appropriate rotation if num is 2
        if (orientedTiles.size() == 2) {
            //check for line
            if (colorBoard[orientedTiles[0]->getTV(0)->getFace()] == opposite[colorBoard[orientedTiles[0]->getTV(0)->getFace()]]) {
                //if vertical line
                if (orientedTiles[0]->getFace() % 2 == 0) {
                    clockwiseRotation('U');
                }
            }
                //if L shape
            else {
                //determine the counterclockwise-most edge
                Tile* rotateAround = orientedTiles[1]->getTV(0);
                if ((orientedTiles[0]->getTV(0)->getFace() < orientedTiles[1]->getTV(0)->getFace()
                     || (orientedTiles[0]->getTV(0)->getFace() == 4 && orientedTiles[1]->getTV(0)->getFace() == 1))
                    && !(orientedTiles[0]->getTV(0)->getFace() == 1 && orientedTiles[1]->getTV(0)->getFace() == 4)) {
                    rotateAround = orientedTiles[0]->getTV(0);
                }
                //rotate orientation tile to left
                if (rotateAround->getFace() == rotationMap['F'])  {
                    clockwiseRotation('U');
                }
                else if (rotateAround->getFace() == rotationMap['B']) {
                    counterClockwiseRotation('U');
                }
                else if (rotateAround->getFace() == rotationMap['R']) {
                    clockwiseRotation('U');
                    clockwiseRotation('U');
                }
            }
        }
        //perform algorithm
        algorithm(crossOrient);
    }
}

void Cube::solveTopEdges() {
    orientCube('Y', 'G');
    std::vector<Tile*> solvedEdges;
    //rotate until two or four edges are solved
    while(solvedEdges.size() != 4) {
        //clear out vector
        solvedEdges.clear();
        //individually check if each edge is solved
        Tile* current = findEdge('G', 'Y');
        if (colorBoard[current->getFace()] == 'G')
            solvedEdges.push_back(current);
        current = findEdge('O', 'Y');
        if (colorBoard[current->getFace()] == 'O')
            solvedEdges.push_back(current);
        current = findEdge('B', 'Y');
        if (colorBoard[current->getFace()] == 'B')
            solvedEdges.push_back(current);
        current = findEdge('R', 'Y');
        if (colorBoard[current->getFace()] == 'R')
            solvedEdges.push_back(current);
        //break if all edges solved
        if (solvedEdges.size() == 4)
            break;
        //rotate again if none are solved
        if (solvedEdges.size() < 2)
            clockwiseRotation('U');
            //if two are solved
        else {
            //check for line
            if (colorBoard[solvedEdges[0]->getFace()] == opposite[colorBoard[solvedEdges[1]->getFace()]]) {
                //if vertical line, rotate
                if (solvedEdges[0]->getFace() % 2 == 0) {
                    clockwiseRotation('U');
                }
            }
                //if L shape
            else {
                //determine the counterclockwise-most edge
                Tile* rotateAround = solvedEdges[1];
                if (solvedEdges[0]->getFace() < solvedEdges[0]->getFace()
                    && !(solvedEdges[0]->getFace() == 1 && solvedEdges[0]->getFace() == 4)) {
                    rotateAround = solvedEdges[0];
                }
                //rotate orientation tile to back
                if (rotateAround->getFace() == rotationMap['F'])  {
                    clockwiseRotation('U');
                    clockwiseRotation('U');
                }
                else if (rotateAround->getFace() == rotationMap['R']) {
                    counterClockwiseRotation('U');
                }
                else if (rotateAround->getFace() == rotationMap['L']) {
                    clockwiseRotation('U');
                }
            }
            //perform algorithm
            algorithm(sune);
        }
    }
}

void Cube::permuteTopCorners() {
    orientCube('Y', 'G');
    std::vector<Tile*> permutedCorners;
    //do corner shuffles until all are permuted
    while (permutedCorners.size() != 4) {
        //clear out vector
        permutedCorners.clear();
        //individually check if each corner is permuted
        Tile* current = findCorner('G', 'Y', 'O');
        if (correctCornerPerumtation(current))
            permutedCorners.push_back(current);
        current = findCorner('O', 'Y', 'B');
        if (correctCornerPerumtation(current))
            permutedCorners.push_back(current);
        current = findCorner('B', 'Y', 'R');
        if (correctCornerPerumtation(current))
            permutedCorners.push_back(current);
        current = findCorner('R', 'Y', 'G');
        if (correctCornerPerumtation(current))
            permutedCorners.push_back(current);
        //break if all corners permuted
        if (permutedCorners.size() == 4)
            break;
        //if one is solved, orient to put it in bottom left
        if (permutedCorners.size() == 1) {
            orientCube('Y', permutedCorners[0]->getColor());
        }
        //do 3 corner shuffle
        algorithm(threeCornerShuffle);
    }
}

void Cube::solveTopCorners() {
    orientCube('Y', 'G');
    std::vector<Tile*> unsolvedCorners;
    //do an initial check for solved corners
    Tile* current = findCorner('Y', 'G', 'O');
    if (current->getFace() != rotationMap['U'])
        unsolvedCorners.push_back(current);
    current = findCorner('Y', 'O', 'B');
    if (current->getFace() != rotationMap['U'])
        unsolvedCorners.push_back(current);
    current = findCorner('Y', 'B', 'R');
    if (current->getFace() != rotationMap['U'])
        unsolvedCorners.push_back(current);
    current = findCorner('Y', 'R', 'G');
    if (current->getFace() != rotationMap['U'])
        unsolvedCorners.push_back(current);
    //repeat till all corners are solved
    int rotationsPerformed = 0;
    std::vector<char> originalColors;
    //preserve originalColors
    for (int i = 0; i < unsolvedCorners.size(); i++) {
        originalColors.push_back(unsolvedCorners[i]->getTV(0)->getColor());
        originalColors.push_back(unsolvedCorners[i]->getTV(1)->getColor());
    }
    for (int i = 0; i < unsolvedCorners.size(); i++) {
        //rotate to front right
        char color1 = unsolvedCorners[i]->getTV(0)->getColor();
        char color2 = unsolvedCorners[i]->getTV(1)->getColor();
        //if it's the blue orange corner
        if ((color1 == 'O' || color2 == 'O') && (color1 == 'B' || color2 == 'B')) {
            while (rotationsPerformed < 1) {
                clockwiseRotation('U');
                rotationsPerformed++;
            }
        }
        else if ((color1 == 'R' || color2 == 'R') && (color1 == 'G' || color2 == 'G')) {
            if (rotationsPerformed == 0) {
                counterClockwiseRotation('U');
                rotationsPerformed--;
            }
            else {
                while (rotationsPerformed < 3) {
                    clockwiseRotation('U');
                    rotationsPerformed++;
                }
            }
        }
        else if ((color1 == 'B' || color2 == 'B') && (color1 == 'R' || color2 == 'R')) {
            if (rotationsPerformed < 0) {
                counterClockwiseRotation('U');
                rotationsPerformed--;
            }
            else {
                while (rotationsPerformed < 2) {
                    clockwiseRotation('U');
                    rotationsPerformed++;
                }
            }
        }
        //update pointers
        for (int j = 0; j < unsolvedCorners.size(); j++) {
            unsolvedCorners[j] = findCorner('Y', originalColors[j * 2], originalColors[j * 2 + 1]);
        }
        //do algorithm
        if (unsolvedCorners[i]->getFace() == rotationMap['R']) {
            algorithm(cornerSolve);
        }
        else {
            reverseAlgorithm(cornerSolve);
        }
    }
    //rotate until final layer lines up
    while (rotationsPerformed != 0) {
        if (rotationsPerformed > 0) {
            counterClockwiseRotation('U');
            rotationsPerformed--;
        }
        else {
            clockwiseRotation('U');
            rotationsPerformed++;
        }
    }
}

void Cube::beginnersMethodSolve() {
    numRotations = 0;
    movesDone.clear();
    //std::cout << std::endl << "Cube before cross" << std::endl;
    //displayColors();
    solveCross();
//    std::cout << "Cross Successful" << std::endl;
//    std::cout << std::endl << "Cube before corners" << std::endl;
//    displayColors();
    solveBottomCorners();
//    std::cout << "Bottom Successful" << std::endl;
//    std::cout << std::endl << "Cube before Middle" << std::endl;
//    displayColors();
    solveMiddleEdges();
//    std::cout << "Middle Successful" << std::endl;
//    std::cout << std::endl << "Cube before edges" << std::endl;
//    displayColors();
    orientTopEdges();
    //std::cout << "Top Edge Orientation Successful" << std::endl;
    solveTopEdges();
    //std::cout << "Top Edges Successful" << std::endl;
    permuteTopCorners();
    //std::cout << "Corner Permutation Successful" << std::endl;
    solveTopCorners();
    //std::cout << "Cube solved Successful" << std::endl;
}

void Cube::algorithm(const std::vector<std::string> &alg) {
    for (int i = 0; i < alg.size(); i++) {
        //if wide move
        if (alg[i] == "l") {
            wideL();
        }
        else if (alg[i] == "l\'") {
            counterWideL();
        }
        else if (alg[i] == "d") {
            wideD();
        }
        else if (alg[i] == "d\'") {
            counterWideD();
        }
            //if a face
        else if (alg[i][1] == 'W' || alg[i][1] == 'Y' || alg[i][1] == 'R' || alg[i][1] == 'O' || alg[i][1] == 'G' || alg[i][1] == 'B') {
            orientCube(alg[i][0], alg[i][1]);
        }

            //if normal rotation
        else if (alg[i][1] == '\'') {
            counterClockwiseRotation(alg[i][0]);
            //std::cout << "doing rotation " << alg[i][0] << "\'" << std::endl;
        }
        else {
            clockwiseRotation(alg[i][0]);
            //std::cout << "doing rotation " << alg[i][0] << std::endl;
        }
    }
}

void Cube::reverseAlgorithm(const std::vector<std::string> &alg) {
    for (int i = alg.size() - 1; i >= 0; i--) {
        //if wide move
        if (alg[i] == "l") {
            counterWideL();
        }
        else if (alg[i] == "l\'") {
            wideL();
        }
        else if (alg[i] == "d") {
            counterWideD();
        }
        else if (alg[i] == "d\'") {
            wideD();
        }

            //if not wide move
        else if (alg[i][1] == '\'') {
            clockwiseRotation(alg[i][0]);
            //std::cout << "doing rotation " << alg[i][0] << "\'" << std::endl;
        }
        else {
            counterClockwiseRotation(alg[i][0]);
            //std::cout << "doing rotation " << alg[i][0] << std::endl;
        }
    }
}


void Cube::wideD() {
    clockwiseRotation('U');
    switch (frontFace) {
        case 'B':
            orientCube('W', 'R');
            break;
        case 'O':
            orientCube('W', 'B');
            break;
        case 'G':
            orientCube('W', 'O');
            break;
        case 'R':
            orientCube('W', 'G');
            break;
    }
}

void Cube::counterWideD() {
    for (int i = 0; i < 3; i++) {
        wideD();
    }
}

void Cube::wideL() {
    clockwiseRotation('R');
    switch (frontFace) {
        case 'B':
            orientCube('G', 'W');
            break;
        case 'W':
            orientCube('Y', 'G');
            break;
        case 'G':
            orientCube('B', 'Y');
            break;
        case 'Y':
            orientCube('W', 'B');
            break;
    }
}

void Cube::counterWideL() {
    for (int i = 0; i < 3; i++) {
        wideL();
    }
}

std::set<std::pair<char, char>> Cube::getUnsolvedEdges() {
    std::set<std::pair<char, char>> unsolvedEdges = allEdges;
    for (auto i : allEdges) {
        Tile* current = findEdge(i.first, i.second);
        Tile* current2 = findEdge(i.second, i.first);
        //if edge is solved, erase it from unsolved edges
        if (current->getColor() == colorBoard[current->getFace()] && current2->getColor() == colorBoard[current2->getFace()]) {
            unsolvedEdges.erase(unsolvedEdges.find({i.first, i.second}));
        }
    }
    return unsolvedEdges;
}

bool Cube::correctCornerPerumtation(Tile* corner) {
    std::set<char> faces;
    faces.insert(colorBoard[corner->getFace()]);
    faces.insert(colorBoard[corner->getTV(0)->getFace()]);
    faces.insert(colorBoard[corner->getTV(1)->getFace()]);
    std::set<char> colors;
    colors.insert(corner->getColor());
    colors.insert(corner->getTV(0)->getColor());
    colors.insert(corner->getTV(1)->getColor());
    return (faces == colors);
}

void Cube::setNumRotations(int rot) {
    numRotations = rot;
}

void Cube::oldPochmannEdges() {
    numRotations = 0;
    movesDone.clear();
    std::set<std::pair<char, char>> unsolvedEdges = getUnsolvedEdges();
    //remove buffer from the set if it is contained
    if (unsolvedEdges.find({'W', 'O'}) != unsolvedEdges.end()) {
        unsolvedEdges.erase(unsolvedEdges.find({'W', 'O'}));
    }
    orientCube('W', 'B');
    //set buffer to be top right edge
    Tile* bufferTileTop = &tileArray[0][1][2];
    Tile* bufferTileSide = bufferTileTop->getTV(0);
    //repeat until all edges are solved
    while (unsolvedEdges.size() > 0) {
        std::vector<std::string> setup;
        char top = bufferTileTop->getColor();
        char side = bufferTileSide->getColor();
        //if buffer is in place find next edge that needs to be fixed
        if ((top == 'W' && side == 'O') || (top == 'O' && side == 'W')) {
            top = colorBoard[findEdge(unsolvedEdges.begin()->first, unsolvedEdges.begin()->second)->getFace()];
            side = colorBoard[findEdge(unsolvedEdges.begin()->second, unsolvedEdges.begin()->first)->getFace()];
        }

        //std::cout << std::endl << "switching edge " << top << ", " << side << std::endl;

        //determine appropriate setup moves
        switch(top) {
            case 'W':
                switch (side) {
                    case 'G':
                        setup = edgeA;
                        break;
                    case 'B':
                        setup = edgeC;
                }
                break;
            case 'R':
                switch (side) {
                    case 'W':
                        setup = edgeE;
                        break;
                    case 'B':
                        setup = edgeF;
                        break;
                    case 'Y':
                        setup = edgeG;
                        break;
                    case 'G':
                        setup = edgeH;
                        break;
                }
                break;
            case 'B':
                switch (side) {
                    case 'W':
                        setup = edgeI;
                        break;
                    case 'O':
                        setup = edgeJ;
                        break;
                    case 'Y':
                        setup = edgeK;
                        break;
                    case 'R':
                        setup = edgeL;
                        break;
                }
                break;
            case 'O':
                switch (side) {
                    case 'G':
                        setup = edgeN;
                        break;
                    case 'Y':
                        setup = edgeO;
                        break;
                    case 'B':
                        setup = edgeP;
                        break;
                }
                break;
            case 'G':
                switch (side) {
                    case 'W':
                        setup = edgeQ;
                        break;
                    case 'R':
                        setup = edgeR;
                        break;
                    case 'Y':
                        setup = edgeS;
                        break;
                    case 'O':
                        setup = edgeT;
                        break;
                }
                break;
            case 'Y':
                switch (side) {
                    case 'B':
                        setup = edgeU;
                        break;
                    case 'O':
                        setup = edgeV;
                        break;
                    case 'G':
                        setup = edgeW;
                        break;
                    case 'R':
                        setup = edgeX;
                        break;
                }
                break;
        }
        //now that setup has been chosen, do appropriate moves
        algorithm(setup);
        algorithm(tPerm);
        reverseAlgorithm(setup);

        //refresh unsolved edges
        unsolvedEdges = getUnsolvedEdges();
        //remove buffer from the set if it is contained
        if (unsolvedEdges.find({'W', 'O'}) != unsolvedEdges.end()) {
            unsolvedEdges.erase(unsolvedEdges.find({'W', 'O'}));
        }
    }
}

std::set<std::vector<char>> Cube::getUnsolvedCorners() {
    std::set<std::vector<char>> unsolvedCorners = allCorners;
    for (auto i : allCorners) {
        Tile* current = findCorner(i[0], i[1], i[2]);
        Tile* current2 = findCorner(i[1], i[0], i[2]);
        //if corner is solved, erase it from unsolved corners
        if (current->getColor() == colorBoard[current->getFace()] && current2->getColor() == colorBoard[current2->getFace()]) {
            unsolvedCorners.erase(unsolvedCorners.find({i[0], i[1], i[2]}));
        }
    }
    return unsolvedCorners;
}

void Cube::oldPochmannCorners() {
    std::set<std::vector<char>> unsolvedCorners = getUnsolvedCorners();
    //remove buffer from the set if it is contained
    if (unsolvedCorners.find({'W', 'G', 'R'}) != unsolvedCorners.end()) {
        unsolvedCorners.erase(unsolvedCorners.find({'W', 'G', 'R'}));
    }
    orientCube('W', 'B');
    //set buffer to be back left corner
    Tile* bufferTileTop = &tileArray[1][0][0];
    Tile* bufferTileBack = bufferTileTop->getTV(0);
    Tile* bufferTileSide = bufferTileTop->getTV(1);

    //repeat until all corners are solved
    while (unsolvedCorners.size() > 0) {
        std::vector<std::string> setup;
        std::vector<char> currentCorner;
        currentCorner.push_back(bufferTileTop->getColor());
        currentCorner.push_back(bufferTileSide->getColor());
        currentCorner.push_back(bufferTileBack->getColor());
        //if buffer is in place, find next edge that needs to be fixed
        if (cornerContains('W', currentCorner) && cornerContains('G', currentCorner) && cornerContains('R', currentCorner)) {
            currentCorner[0] = colorBoard[findCorner((*unsolvedCorners.begin())[0], (*unsolvedCorners.begin())[1], (*unsolvedCorners.begin())[2])->getFace()];
            currentCorner[1] = colorBoard[findCorner((*unsolvedCorners.begin())[1], (*unsolvedCorners.begin())[0], (*unsolvedCorners.begin())[2])->getFace()];
            currentCorner[2] = colorBoard[findCorner((*unsolvedCorners.begin())[2], (*unsolvedCorners.begin())[1], (*unsolvedCorners.begin())[0])->getFace()];
        }
        //std::cout << "Switching corner " << currentCorner[0] << ", " << currentCorner[1] << ", " << currentCorner[2] << std::endl;

        //determine appropriate setup moves
        if (currentCorner[0] == 'W') {
            if (cornerContains('O', currentCorner) && cornerContains('G', currentCorner))
                setup = cornerB;
            else if (cornerContains('O', currentCorner) && cornerContains('B', currentCorner))
                setup = cornerC;
            else if (cornerContains('R', currentCorner) && cornerContains('B', currentCorner))
                setup = cornerD;
        }
        else if (currentCorner[0] == 'R') {
            if (cornerContains('W', currentCorner) && cornerContains('B', currentCorner))
                setup = cornerF;
            else if (cornerContains('B', currentCorner) && cornerContains('Y', currentCorner))
                setup = cornerG;
            else if (cornerContains('Y', currentCorner) && cornerContains('G', currentCorner))
                setup = cornerH;
        }
        else if (currentCorner[0] == 'B') {
            if (cornerContains('W', currentCorner) && cornerContains('R', currentCorner))
                setup = cornerI;
            else if (cornerContains('W', currentCorner) && cornerContains('O', currentCorner))
                setup = cornerJ;
            else if (cornerContains('Y', currentCorner) && cornerContains('O', currentCorner))
                setup = cornerK;
            else if (cornerContains('Y', currentCorner) && cornerContains('R', currentCorner))
                setup = cornerL;
        }
        else if (currentCorner[0] == 'O') {
            if (cornerContains('W', currentCorner) && cornerContains('B', currentCorner))
                setup = cornerM;
            else if (cornerContains('W', currentCorner) && cornerContains('G', currentCorner))
                setup = cornerN;
            else if (cornerContains('Y', currentCorner) && cornerContains('G', currentCorner))
                setup = cornerO;
            else if (cornerContains('Y', currentCorner) && cornerContains('B', currentCorner))
                setup = cornerP;
        }
        else if (currentCorner[0] == 'G') {
            if (cornerContains('W', currentCorner) && cornerContains('O', currentCorner))
                setup = cornerQ;
            else if (cornerContains('Y', currentCorner) && cornerContains('R', currentCorner))
                setup = cornerS;
            else if (cornerContains('Y', currentCorner) && cornerContains('O', currentCorner))
                setup = cornerT;
        }
        else if (currentCorner[0] == 'Y') {
            if (cornerContains('B', currentCorner) && cornerContains('R', currentCorner))
                setup = cornerU;
            else if (cornerContains('G', currentCorner) && cornerContains('O', currentCorner))
                setup = cornerW;
            else if (cornerContains('R', currentCorner) && cornerContains('G', currentCorner))
                setup = cornerX;
        }

        //now that setup has been chosen, do appropriate moves
        algorithm(setup);
        algorithm(diagonal);
        reverseAlgorithm(setup);

        //refresh unsolved corners
        unsolvedCorners = getUnsolvedCorners();
        //remove buffer from the set if it is contained
        if (unsolvedCorners.find({'W', 'G', 'R'}) != unsolvedCorners.end()) {
            unsolvedCorners.erase(unsolvedCorners.find({'W', 'G', 'R'}));
        }
    }
}

bool Cube::cornerContains(char color, std::vector<char> &corner) {
    for (auto i : corner) {
        if (i == color) {
            return true;
        }
    }
    return false;
}

void Cube::oldPochmannSolve() {
    oldPochmannEdges();
    oldPochmannCorners();
}

std::vector<std::string> Cube::getMovesDone() {
    return movesDone;
}
