/*
 * Saida.h
 *
 * Cada objeto Saida representa uma linha do arquivo de saida
 *
 */

#ifndef SAIDA_H_
#define SAIDA_H_
#include<string> // for string class
using namespace std;

class Saida {
public:
	Saida();
	Saida(int id, string txs, string sr, string vs);

	int getId();
	void setId(int id);
	string getSr();
	void setSr(string sr);
	string getTxs();
	void setTxs(string txs);
	string getVs();
	void setVs(string vs);

private:
	int id; //  identificador do escalonamento
	string txs; // lista de transações
	string sr; // resultado do algoritmo da garantia da seriabilidade, onde SS e NS significam respectivamente serial (SS) ou não serial (NS)
	string vs; // resultado do algoritmo de teste de equivalência de visão, onde SV e NV significam respectivamente equivalente (SV) ou não equivalente (NV)
};

#endif /* SAIDA_H_ */
