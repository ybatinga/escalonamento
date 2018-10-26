/*
 * EvTx.cpp
 *
 * Representa uma transacao da regra de teste de equivalencia de visao:
 * - para cada r(x) de Ti, se o valor de x lido for escrito por w(x) de Tj, o mesmo deve permanecer para r(x) de Ti em S'
 * - se o operador w(y) em Tk é a ultima escrita de y em S, então w(y) em Tk deve ser a última escrita em S'
 */

#include "EvTx.h"

#include<string> // for string class
using namespace std;
EvTx::EvTx() {
}

EvTx::EvTx(int idA, int idB, string op, string at, string idu){
	this->idA = idA;
	this->idB = idB;
	this->op = op;
	this->at = at;
	this->idu = idu;
}

string EvTx::getAt()  {
	return at;
}

void EvTx::setAt( string at) {
	this->at = at;
}

int EvTx::getIdA() {
	return idA;
}

void EvTx::setIdA(int idA) {
	this->idA = idA;
}

int EvTx::getIdB() {
	return idB;
}

void EvTx::setIdB(int idB) {
	this->idB = idB;
}

string EvTx::getOp() {
	return op;
}

void EvTx::setOp(string op) {
	this->op = op;
}

bool EvTx::isE() {
	return e;
}

void EvTx::setE(bool e) {
	this->e = e;
}

string EvTx::getIdu() {
	return idu;
}

void EvTx::setIdu(string idu) {
	this->idu = idu;
}
