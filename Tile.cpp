//
// Created by Aidan on 3/16/2023.
//

#include "Tile.h"

void Tile::setFace(int face){
    this->face = face;
}

int Tile::getFace(){
    return this->face;
}

void Tile::setRow(int row){
    this->row = row;
}

int Tile::getRow(){
    return this->row;
}

void Tile::setColumn(int column){
    this->column = column;
}

int Tile::getColumn(){
    return this->column;
}

void Tile::setColor(char val){
    this->color = val;
}

char Tile::getColor(){
    return this->color;
}
void Tile::displayLocation(){
    std::cout << "(" << this->getFace() << this->getRow() << this->getColumn() << ") ";
}

void Tile::expandTV(int num){
    tileVector.resize(num);
}

bool Tile::setTV(int position, Tile* ptr){
    if (position > 11 || position < 0)
        return false;
    tileVector.at(position) = ptr;
    return true;
}

Tile* Tile::getTV(int position){
    if (position > 11 || position < 0)
        return nullptr;
    return tileVector.at(position);
}

int Tile::getTVSize(){
    return this->tileVector.size();
}

void Tile::printTV(){
    for (int i = 0; i < 12; i++){
        std::cout << this->tileVector.at(i)->getColor();
    }
}
