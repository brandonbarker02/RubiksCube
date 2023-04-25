//
// Created by barke on 4/21/2023.
//
#include <vector>
#include <string>

#ifndef RUBIKSCUBE_ALGORITHMS_H
#define RUBIKSCUBE_ALGORITHMS_H

//----------------------------------------------ALGORITHMS-------------------------------------------------------------------
const std::vector<std::string> tPerm = {"R", "U", "R\'", "U\'", "R\'", "F", "R", "R", "U\'", "R\'", "U\'", "R", "U", "R\'", "F\'"};
const std::vector<std::string> diagonal = {"R", "U\'", "R\'", "U\'", "R", "U", "R\'", "F\'", "R", "U", "R\'", "U\'", "R\'", "F", "R"};
const std::vector<std::string> sampleScramble = {"D\'", "R\'", "U", "U\'", "R\'", "B\'", "U\'", "F", "B", "L\'"};
const std::vector<std::string> middleEdgeRight = {"U", "R", "U\'", "R\'", "U\'", "F\'", "U", "F"};
const std::vector<std::string> middleEdgeLeft = {"U\'", "L\'", "U", "L", "U", "F", "U\'", "F\'"};
const std::vector<std::string> crossOrient = {"F", "R", "U", "R\'", "U\'", "F\'"};
const std::vector<std::string> sune = {"R", "U", "R\'", "U", "R", "U", "U", "R\'"};
const std::vector<std::string> threeCornerShuffle = {"U", "R", "U\'", "L\'", "U", "R\'", "U\'", "L"};
const std::vector<std::string> cornerSolve = {"R\'", "D\'", "R", "D", "R\'", "D\'", "R", "D"};

//----------------------------------------------EDGE SETUP PATHS------------------------------------------------------------------
const std::vector<std::string> edgeA = {"l", "l", "D\'", "L", "L"};
const std::vector<std::string> edgeC = {"l", "l", "D", "L", "L"};
const std::vector<std::string> edgeE = {"L", "d\'", "L"};
const std::vector<std::string> edgeF = {"d\'", "L"};
const std::vector<std::string> edgeG = {"L\'", "d\'", "L"};
const std::vector<std::string> edgeH = {"d", "L\'"};
const std::vector<std::string> edgeI = {"l", "D\'", "L", "L"};
const std::vector<std::string> edgeJ = {"d", "d", "L"};
const std::vector<std::string> edgeK = {"l", "D", "L", "L"};
const std::vector<std::string> edgeL = {"L\'"};
const std::vector<std::string> edgeN = {"d", "L"};
const std::vector<std::string> edgeO = {"D", "D", "L\'", "d\'", "L"};
const std::vector<std::string> edgeP = {"d\'", "L\'"};
const std::vector<std::string> edgeQ = {"l\'", "D", "L", "L"};
const std::vector<std::string> edgeR = {"L"};
const std::vector<std::string> edgeS = {"l\'", "D\'", "L", "L"};
const std::vector<std::string> edgeT = {"d", "d", "L\'"};
const std::vector<std::string> edgeU = {"D\'", "L", "L"};
const std::vector<std::string> edgeV = {"D", "D", "L", "L"};
const std::vector<std::string> edgeW = {"D", "L", "L"};
const std::vector<std::string> edgeX = {"L", "L"};

//----------------------------------------------CORNER SETUP PATHS------------------------------------------------------------------
const std::vector<std::string> cornerB = {"R", "R"};
const std::vector<std::string> cornerC = {"F", "F", "D"};
const std::vector<std::string> cornerD = {"F", "F"};
const std::vector<std::string> cornerF = {"F\'", "D"};
const std::vector<std::string> cornerG = {"F\'"};
const std::vector<std::string> cornerH = {"D\'", "R"};
const std::vector<std::string> cornerI = {"F", "R\'"};
const std::vector<std::string> cornerJ = {"R\'"};
const std::vector<std::string> cornerK = {"F\'", "R\'"};
const std::vector<std::string> cornerL = {"F", "F", "R\'"};
const std::vector<std::string> cornerM = {"F"};
const std::vector<std::string> cornerN = {"R\'", "F"};
const std::vector<std::string> cornerO = {"R", "R", "F"};
const std::vector<std::string> cornerP = {"R", "F"};
const std::vector<std::string> cornerQ = {"R", "D\'"};
const std::vector<std::string> cornerS = {"D", "F\'"};
const std::vector<std::string> cornerT = {"R"};
const std::vector<std::string> cornerU = {"D"};
const std::vector<std::string> cornerW = {"D\'"};
const std::vector<std::string> cornerX = {"D", "D"};

#endif //RUBIKSCUBE_ALGORITHMS_H
