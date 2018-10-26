/*
 * Tx.h
 *
 * Tx (transacao) representa uma linha do arquivo de entrada
 *
 */

#ifndef TX_H_
#define TX_H_
#include<string> // for string class
using namespace std;

class Tx {
public:
	Tx();
	Tx(int tc, int id, string op, string at);

	int getTc();
	void setTc(int tc);
	int getId();
	void setId(int id);
	string getOp();
	void setOp(string op);
	string getAt();
	void setAt(string at);

private:
	int tc; // tempo de chegada
	int id; // identificador da transação
	string op; // operação (R=read, W=write, C=commit)
	string at; // atributo lido/escrito
};

#endif /* TX_H_ */
