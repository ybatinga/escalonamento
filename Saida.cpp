/*
 * Saida.cpp
 *
 * Cada objeto Saida representa uma linha do arquivo de saida
 *
 */


#include "Saida.h"
#include<string> // for string class
using namespace std;

Saida::Saida() {
	// TODO Auto-generated constructor stub
}

Saida::Saida(int id, string txs, string sr, string vs){
	this->id = id;
	this->txs = txs;
	this->sr = sr;
	this->vs = vs;
}

int Saida::getId() {
	return id;
}

void Saida::setId(int id) {
	this->id = id;
}

string Saida::getSr() {
	return sr;
}

void Saida::setSr(string sr) {
	this->sr = sr;
}

string Saida::getTxs() {
	return txs;
}

void Saida::setTxs(string txs) {
	this->txs = txs;
}

string Saida::getVs() {
	return vs;
}

void Saida::setVs(string vs) {
	this->vs = vs;
}

