/*
 * evTx.h
 *
 * Representa uma transacao da regra de teste de equivalencia de visao:
 * - para cada r(x) de Ti, se o valor de x lido for escrito por w(x) de Tj, o mesmo deve permanecer para r(x) de Ti em S'
 * - se o operador w(y) em Tk é a ultima escrita de y em S, então w(y) em Tk deve ser a última escrita em S'
 */

#ifndef EVTX_H_
#define EVTX_H_
#include<string> // for string class
using namespace std;
class EvTx {
public:
	EvTx();
	EvTx(int idA, int idB, string op, string at, string idu);

	string getAt();
	void setAt(string at);
	int getIdA();
	void setIdA(int idA);
	int getIdB();
	void setIdB(int idB);
	string getOp();
	void setOp(string op);
	bool isE();
	void setE(bool e);
	string getIdu();
	void setIdu(string idu);

	bool operator==(const EvTx & obj2) const
	{
		if(this->op.compare(obj2.op) == 0)
			return true;
		else
			return false;
	}

private:
	int idA; // identificador da transação A
	int idB; // identificador da transação B
	string op; // operação (READ_WRITE, WRITE, WRITE_WRITE, WRITE_READ)
	string at; // atributo lido/escrito
	string idu; // identificador unico do objeto que representa a concatenacao de idA e idB
	bool e; // booleano que sinaliza equivalencia de transação com escalonamento S’
};

#endif /* EVTX_H_ */
