/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Esc.cpp
 * Author: ubuntu
 * 
 * Created on October 6, 2018, 4:26 PM
 */

#include "Esc.h"
#include "Tx.h"
#include <vector>
#include <list>
using namespace std;
Esc::Esc() {
}
Esc::Esc(vector<int> idList) {
    this->idList = idList;
    this->isSerial = false;
}
//Esc::Esc(vector<Tx> escList, vector<int> idList){
//    this->escList = escList;
//    this->idList = idList;
//}
Esc::~Esc() {
}

void Esc::SetEscList(vector<Tx> escList) {
    this->escList = escList;
}

vector<Tx> Esc::GetEscList() {
    return escList;
}

void Esc::SetIdList(vector<int> idList) {
    this->idList = idList;
}

vector<int> Esc::GetIdList() {
    return idList;
}

void Esc::SetVisList(vector<Tx> visList) {
    this->visList = visList;
}

vector<Tx> Esc::GetVisList() {
    return visList;
}

void Esc::SetSerial(bool Serial) {
    isSerial = Serial;
}

bool Esc::IsSerial() const {
    return isSerial;
}




