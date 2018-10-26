/*
 * Tx.cpp
 *
 * Tx (transacao) representa uma linha do arquivo de entrada
 *
 */

#include "Tx.h"
#include<string> // for string class
using namespace std;

Tx::Tx(){
}
Tx::Tx(int tc, int id, string op, string at){
	this->tc = tc;
	this->id = id;
	this->op = op;
	this->at = at;
}

int Tx::getTc() {
	return tc;
}

void Tx::setTc(int tc) {
	this->tc = tc;
}

int Tx::getId() {
	return id;
}

void Tx::setId(int id) {
	this->id = id;
}

string Tx::getOp() {
	return op;
}

void Tx::setOp(string op) {
	this->op = op;
}

string Tx::getAt() {
	return at;
}

void Tx::setAt(string at) {
	this->at = at;
}
