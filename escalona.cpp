#include<stdio.h>
#include<string.h>
#include<string>
using namespace std;
#include "Tx.h"
#include "Graph.h"
#include "EvTx.h"
#include <vector>
#include <algorithm> // std::find
#include "Saida.h"
#include "Esc.h"

#define ARQUIVO_ENTRADA "teste.in" // nome do arquivo de entrada
#define ARQUIVO_SAIDA "teste.sol" // nome do arquivo de saída
#define NAO_SERIAL "NS"
#define SIM_SERIAL "SS"
#define NAO_VISAO "NV"
#define SIM_VISAO "SV"
#define READ_WRITE "RW" // regra de equivalencia de visao: - para cada r(x) de Ti, se o valor de x lido for escrito por w(x) de Tj, o mesmo deve permanecer para r(x) de Ti em S'
#define WRITE_WRITE "WW"
#define WRITE_READ "WR"
#define C "C" // commit
#define A "A" // abort
#define W "W" // write
#define R "R" // read


vector<Tx> txList; // lista de transacoes
vector<Esc> escListList; //
int cEsc; // conta quantidade de escalonamento
vector<EvTx> evTxList; // lista de conjunto de operacoes que satisfazem regra de teste de equivalencia de visao:
bool isSerial;
bool isEV;
vector<Saida> saidaList;

/*
 * função para criação de arquivo de entrada com dados fornecidos
 */
void criarArquivoEntrada();
/*
 * carrega transacoes de arquivo de entrada
 */
void carregaArquivoEntrada();
/*
 * triagem grupo de transacoes de escalonamento
 */
void triagemEscalonamento();
/*
 * insere transacoes em grafo de acordo com regras de conflito de serializacao para verificacao de seriabilidade quanto ao conflito
 */
void testeSeriabilidadeConflito(vector<Tx> escList);
/*
 * insere transacoes em grafo de acordo com regras de equivalencia de visao para verificacao de equivalencia de visao
 */
void testeEquivalenciaVisao(unsigned i, vector<Tx> escList);
/*
 * salva um resultado em lista para ser processado em arquivo de saida
 */
void salvaSaida(int index);
/*
 * combinacao fatorial de transacoes de um escalonamento
 * Fonte algoritimo: https://stackoverflow.com/questions/32678349/all-permutations-c-with-vectorint-and-backtracking
*/
void combinacaoFatorialEscalonamento(const std::vector<int>& v, vector<Tx> escList);
/*
 * gera permutacoes de escalonamento
 */
void permutaEscalonamento(unsigned i, vector<Tx> escList);
/*
 * agrupa conjunto de transacoes que fazem parte de regra de equivalencia de visao
 */
void agrupaTransacoesEquivalencia(vector<Tx> escList);
/*
 * salva resultados em arquivo de saida
 */
void processaSaida();
/*
 * triagem de ids para lista de escalonamento
 */
void triagemIdsEsc();
void ordenaEsc();
int main()
{
//	criarArquivoEntrada();
	carregaArquivoEntrada();
	triagemIdsEsc();
        triagemEscalonamento();
        ordenaEsc();
	for (unsigned i = 0; i < escListList.size(); i++){
		testeSeriabilidadeConflito(escListList.at(i).GetEscList());
		permutaEscalonamento(i, escListList.at(i).GetEscList());
		agrupaTransacoesEquivalencia(escListList.at(i).GetEscList());
		testeEquivalenciaVisao(i, escListList.at(i).GetEscList());
		salvaSaida(i);
		isSerial = false;
		isEV = false;
		evTxList.clear();
	}
	processaSaida();
	return 0;
}
/*
 * ordena escalonamento de acordo com ordem de chegada
 */
void ordenaEsc(){
    for (unsigned k = 0; k < escListList.size(); k++){
        vector<Tx> escList = escListList.at(k).GetEscList();
        int temp;
	for (unsigned j = 0; j < escList.size() - 1; ++j) {
            Tx min = escList.at(j);
            temp = j;
            for (unsigned i = j+1; i < escList.size(); ++i) {
                if (min.getTc() > escList.at(i).getTc()) {
                    min = escList.at(i);  
                    temp = i;
                    }
                }  
		swap(escList.at(j), escList.at(temp));
	}
        escListList.at(k).SetEscList(escList);
    }
}
/*
 * triagem grupo de transacoes de escalonamento
 */
void triagemEscalonamento(){
    for (unsigned i = 0; i < escListList.size(); i++){
        Esc esc = escListList.at(i);
        vector<Tx> escList;
        for (unsigned j = 0; j < esc.GetIdList().size(); j++){
//            for (int k = 0; k < esc.GetIdList().size(); k++){
                int id = esc.GetIdList().at(j);
                for (unsigned l = 0; l < txList.size();l++){
                    Tx tx = txList.at(l);
                    if (tx.getId() == id){
                        tx.setIdG(j);
                        escList.push_back(tx);
                    }
                }
//            }
            
        }
        escListList.at(i).SetEscList(escList);
    }
}
/*
 * triagem de ids para lista de escalonamento
 */
void triagemIdsEsc(){
    string s ; // recebe resultado se NS ou SS
    unsigned i,j;  // counter
    vector<int> idList; // lista com identificadores unicos de transacoes de um grupo de escalonamento
    
    // percorre itens de transacoes da lista
    for(i = 0; i < txList.size(); i++) {
//        Graph* g = new Graph(txList.size());
        // fixa o for em transacao i para teste de conflito de seriazabilidade
        Tx txi = txList.at(i);
        int k = 0; // contador para iterador de lista de ids
        for(j = i; j < txList.size(); j++){
            // fixa o for em transacao j para teste de conflito de seriazabilidade
            Tx txj = txList.at(j);

            // entra no if se tempo de chegada de transacoes
            // estao em ordem crescente,
            // o id das transacoes sao diferentes
            // e os atributos sao os mesmos
            if (txi.getTc() < txj.getTc()
                        &&
                txi.getId() != txj.getId()
                        &&
                txi.getAt() == txj.getAt()
            ){
                if (
                    // aresta Ti -> Tj para cada w(x) em Tj depois de r(x) em Ti
                    (txi.getOp() == R && txj.getOp() == W)
                    ||
                    // aresta Ti -> Tj para cada r(x) em Tj depois de w(x) em Ti
                    (txi.getOp() == W && txj.getOp() == R)
                    ||
                    // aresta Ti -> Tj para cada w(x) em Tj depois de w(x) em Ti
                    (txi.getOp() == W && txj.getOp() == W)
                ){
                    // fonte para algoritmo 'find': http://www.cplusplus.com/reference/algorithm/find/
                    // usado para contar quantidade de vertices em escalonamento
                    vector<int>::iterator it;
                    it = find (idList.begin(), idList.end(), txi.getId());
                    if (it == idList.end()){
                        idList.push_back(txi.getId());
                    }
                    // adiciona aresta em grafo
//                    g->addEdge(txi.getId(), txj.getId());
                    k++;
                }
                if (
                    // aresta Ti -> Tj para cada r(x) em Tj depois de r(x) em Ti
                    // essa regra nao se aplica a regra de teste de seriabilidade,
                    // se aplica somente para esta selecao de triagem de transacoes de escalonamento
                    (txi.getOp() == R && txj.getOp() == R)
                ){
                    // fonte para algoritmo 'find': http://www.cplusplus.com/reference/algorithm/find/
                    // usado para contar quantidade de vertices em escalonamento
                    vector<int>::iterator it;
                    it = find (idList.begin(), idList.end(), txi.getId());
                    if (it == idList.end()){
                        idList.push_back(txi.getId());
                        idList.push_back(txj.getId());
                    }
                    k++;
                }
            }
            
            // se houver transacao com commit,
            // finaliza-se a triagem de agrupamento de transacoes de escalonamento
            if (
                (txi.getOp() == C)
            ){
//                i = j; // salva checkpoint para novo escalonamento
                for (unsigned i = 0; i < idList.size(); i++){
                    if (idList.at(0) == txi.getId()){
                        Esc esc(idList);             
//                        // aponta se existe ciclo no escalonamento
//                        if(g->isCyclic()){
//                                esc.SetSerial(false);
//                        }else{
//                                esc.SetSerial(true);
//                        }
//                        delete g;
                        escListList.push_back(esc);
                        idList.clear();
                        
                        
                    }
                }
            }
        }
        
    }
}

/*
 * insere transacoes em grafo de acordo com regras de equivalencia de visao para verificacao de equivalencia de visao
 */
void testeEquivalenciaVisao(unsigned m, vector<Tx> escList){

	unsigned h, i, j, k, l;  // counter
	bool isBreak = false;;

	// percorre itens de transacoes da lista
	// inicia por "h = 1", pois "h = 0" representa a imagem de escalonamento S
	for(h = 1; h < escListList.at(m).GetVisList().size(); h++) {
		vector<Tx> txPList = escListList.at(m).GetVisList(); // lista de transacoes permutadas
//		vector<EvTx> evTxListAux = evTxList; // imagem auxiliar de

		// percorre itens de transacoes da lista
		for(i = 0; i < txPList.size(); i++) {

			Tx txi = txPList.at(i);

			for(j = i+1; j < txPList.size(); j++){
				// fixa o for em transacao j para teste de conflito de seriazabilidade
				Tx txj = txPList.at(j);

				// entra no if se tempo de chegada de transacoes
				// estao em ordem crescente,
				// o id das transacoes sao diferentes
				// e os atributos sao os mesmos
				if (
					txi.getTc() < txj.getTc()
						&&
					txi.getId() != txj.getId()
						&&
					txi.getAt() == txj.getAt()
						&&
					txi.getOp() == "R"
						&&
					txj.getOp() == "W"
				){
					string idu = to_string(txi.getId()).append(to_string(txj.getId()));
					EvTx evTx(txi.getId(), txj.getId(), READ_WRITE, txi.getAt(), idu);

					std::vector<EvTx>::iterator it;
					it = std::find(evTxList.begin(), evTxList.end(), evTx);
					if(it != evTxList.end()){
						EvTx evTxSub = evTx;
						evTxSub.setE(true);
						// salva na lista as variaveis que entram na regra:
						// para cada r(x) de Ti, se o valor de x lido for escrito por w(x) de Tj, o mesmo deve permanecer para r(x) de Ti em S'
						replace (evTxList.begin(), evTxList.end(), evTx, evTxSub);
					}
				}else
//				if(
//					txi.getTc() < txj.getTc()
//						&&
//					txi.getId() != txj.getId()
//						&&
//					txi.getAt() == txj.getAt()
//						&&
//					txi.getOp() == "W"
//						&&
//					txj.getOp() == "R"
//				){
//					string idu = to_string(txi.getId()).append(to_string(txj.getId()));
//					EvTx evTx(txi.getId(), txj.getId(), WRITE_READ, txi.getAt(), idu);
//					std::vector<EvTx>::iterator it;
//					it = std::find(evTxList.begin(), evTxList.end(), evTx);
//					if(it != evTxList.end()){
//						EvTx evTxSub = evTx;
//						evTxSub.setE(true);
//						// salva na lista as variaveis que entram na regra:
//						// para cada r(x) de Ti, se o valor de x lido for escrito por w(x) de Tj, o mesmo deve permanecer para r(x) de Ti em S'
//						replace (evTxList.begin(), evTxList.end(), evTx, evTxSub);
//					}
//				}
				if (
					txj.getOp() == "W"
						&&
					j == escList.size()-1
				){
					string idu = to_string(txi.getId()).append(to_string(txj.getId()));
					EvTx evTx(txi.getId(), txj.getId(), W, txi.getAt(), idu);

					std::vector<EvTx>::iterator it;
					it = std::find(evTxList.begin(), evTxList.end(), evTx);
					if(it != evTxList.end()){
						EvTx evTxSub = evTx;
						evTxSub.setE(true);
						// salva na lista as variaveis que entram na regra:
						// para cada r(x) de Ti, se o valor de x lido for escrito por w(x) de Tj, o mesmo deve permanecer para r(x) de Ti em S'
						replace (evTxList.begin(), evTxList.end(), evTx, evTxSub);
					}
				} 
//                                if(
//						txi.getTc() < txj.getTc()
//							&&
//						txi.getId() != txj.getId()
//							&&
//						txi.getAt() == txj.getAt()
//							&&
//						txi.getOp() == "W"
//							&&
//						txj.getOp() == "W"
//				){
//					string idu = to_string(txi.getId()).append(to_string(txj.getId()));
//					EvTx evTx(txi.getId(), txj.getId(), WRITE_WRITE, txi.getAt(), idu);
//
//					std::vector<EvTx>::iterator it;
//					it = std::find(evTxList.begin(), evTxList.end(), evTx);
//					if(it != evTxList.end()){
//						EvTx evTxSub = evTx;
//						evTxSub.setE(true);
//						// salva na lista as variaveis que entram na regra:
//						// para cada r(x) de Ti, se o valor de x lido for escrito por w(x) de Tj, o mesmo deve permanecer para r(x) de Ti em S'
//						replace (evTxList.begin(), evTxList.end(), evTx, evTxSub);
//					}
//				}
			}
			l=0;
			for (k = 0; k < evTxList.size(); k++){
				if(evTxList.at(k).isE() == true){
					l = l+1;
				}
			}
			if (l == evTxList.size()){
				isEV = true;
				isBreak = true;
			}

			for (EvTx evTx : evTxList){
				std::vector<EvTx>::iterator it;
				it = std::find(evTxList.begin(), evTxList.end(), evTx);
				if(it != evTxList.end()){
					EvTx evTxSub = evTx;
					evTxSub.setE(false);
					// salva na lista as variaveis que entram na regra:
					// para cada r(x) de Ti, se o valor de x lido for escrito por w(x) de Tj, o mesmo deve permanecer para r(x) de Ti em S'
					replace (evTxList.begin(), evTxList.end(), evTx, evTxSub);
				}
			}
			if (isBreak) break;
		}
		if (isBreak) break;
	}

}

void agrupaTransacoesEquivalencia(vector<Tx> escList){
	unsigned i, j;  // counter

	// percorre itens de transacoes da lista
	for(i = 0; i < escList.size(); i++) {

		Tx txi = escList.at(i);

		for(j = i+1; j < escList.size(); j++){
			// fixa o for em transacao j para teste de conflito de seriazabilidade
			Tx txj = escList.at(j);

			// entra no if se tempo de chegada de transacoes
			// estao em ordem crescente,
			// o id das transacoes sao diferentes
			// e os atributos sao os mesmos
			if(
				txi.getTc() < txj.getTc()
					&&
				txi.getId() != txj.getId()
					&&
				txi.getAt() == txj.getAt()
					&&
				txi.getOp() == "R"
					&&
				txj.getOp() == "W"
			){
				string idu = to_string(txi.getId()).append(to_string(txj.getId()));
				EvTx evTx(txi.getId(), txj.getId(), READ_WRITE, txi.getAt(), idu);
				evTx.setE(false);
				// salva na lista as variaveis que entram na regra:
				// para cada r(x) de Ti, se o valor de x lido for escrito por w(x) de Tj, o mesmo deve permanecer para r(x) de Ti em S'
				evTxList.push_back(evTx);
			}else
//			if(
//				txi.getTc() < txj.getTc()
//					&&
//				txi.getId() != txj.getId()
//					&&
//				txi.getAt() == txj.getAt()
//					&&
//				txi.getOp() == "W"
//					&&
//				txj.getOp() == "R"
//			){
//				string idu = to_string(txi.getId()).append(to_string(txj.getId()));
//				EvTx evTx(txi.getId(), txj.getId(), WRITE_READ, txi.getAt(), idu);
//				evTx.setE(false);
//				// salva na lista as variaveis que entram na regra:
//				// aresta Ti -> Tj para cada r(x) em Tj depois de w(x) em Ti
//				evTxList.push_back(evTx);
//			}
			if (
				txj.getOp() == "W"
					&&
				j == escList.size()-1
			){
				string idu = to_string(txi.getId()).append(to_string(txj.getId()));
				EvTx evTx(txi.getId(), txj.getId(), W, txi.getAt(), idu);
				evTx.setE(false);
				// salva na lista as variaveis que entram na regra:
				// Se o operador w(y) em Tk é a ultima escrita de y em S, então w(y) em Tk deve ser a última escrita em S'
				evTxList.push_back(evTx);
			}
//			if(
//					txi.getTc() < txj.getTc()
//						&&
//					txi.getId() != txj.getId()
//						&&
//					txi.getAt() == txj.getAt()
//						&&
//					txi.getOp() == "W"
//						&&
//					txj.getOp() == "W"
//				){
//					string idu = to_string(txi.getId()).append(to_string(txj.getId()));
//					EvTx evTx(txi.getId(), txj.getId(), WRITE_WRITE, txi.getAt(), idu);
//					evTx.setE(false);
//					// salva na lista as variaveis que entram na regra:
//					// aresta Ti -> Tj para cada w(x) em Tj depois de w(x) em Ti
//					evTxList.push_back(evTx);
//			}
		}
	}
}

/*
 * combinacao fatorial de transacoes de um escalonamento
 * Fonte algoritimo: https://stackoverflow.com/questions/32678349/all-permutations-c-with-vectorint-and-backtracking
*/
void combinacaoFatorialEscalonamento(unsigned i, const std::vector<int>& v, vector<Tx> escList)
{
	vector<Tx> txL;
    for (int e : v) {
        for (Tx tx : escList){
        	if (tx.getTc() == e){
        		txL.push_back(tx);
				break;
        	}
        }
    }
//    visList.push_back(txL);
    escListList.at(i).SetVisList(txL);
}

/*
 * gera permutacoes de escalonamento
 */
void permutaEscalonamento(unsigned i, vector<Tx> escList){
	// executa combinacao fatorial de transacoes de um escalonamento
	std::vector<int> v;
	for (Tx tx : escList){
		v.push_back(tx.getTc());
	}
	do {
		combinacaoFatorialEscalonamento(i, v, escList);
	} while (std::next_permutation(v.begin(), v.end()));
}

///*
// * triagem grupo de transacoes de escalonamento
// */
//void triagemEscalonamento(){
//    unsigned i,j;  // counter
//    vector<Tx> escList; // lista de lista de grupo de transacoes de escalonamento
//    vector<int> idList; // lista com identificadores unicos de transacoes de um grupo de escalonamento
//    Tx txAux;
//
//    // percorre itens de transacoes da lista
//    for(j = 0; j < txList.size(); j++) {
//        int k = 0; // contador para iterador de lista de ids
//        // percorre itens de transacoes da lista
//        for(i = j; i < txList.size(); i++) {
//
//            // usa-se txAux para comparar item anterior com atual na iteracao
//            if (i != 0) {
//                txAux = txList.at(i-1);
//            }
//
//            // fixa o for em transacao i para teste de conflito de seriazabilidade
//            Tx txi = txList.at(i);
//
//            if (txi.getOp() != "C")
//            // adiciona transacao em escalonamento
//            escList.push_back(txi);
//
//            // fonte para algoritmo 'find': http://www.cplusplus.com/reference/algorithm/find/
//            // usado para contar quantidade de vertices em escalonamento
//            vector<int>::iterator it;
//            it = find (idList.begin(), idList.end(), txi.getId());
//            if (it == idList.end()){
//                idList.push_back(escList.at(k).getId());
//            }
//            k++;
//            // se duas transacoes seguidas tiverem commit,
//            // finaliza-se a triagem de agrupamento de transacoes de esconamento
//            if (
//                    (txAux.getId() != 0)
//                    &&
//                    (txAux.getOp() == "C")
//                    &&
//                    (txi.getOp() == "C")
//            ){
//                j = i; // salva checkpoint para novo escalonamento
//                break;
//            }
//        }
//        Esc esc(escList, idList);
//        escListList.push_back(esc);
//        escList.clear();
//        idList.clear();
//    }
//}

/*
 * insere transacoes em grafo de acordo com regras de conflito de serializacao para verificacao de seriabilidade quanto ao conflito
 */
void testeSeriabilidadeConflito(vector<Tx> escList){
    
	string s ; // recebe resultado se NS ou SS
	unsigned i,j;  // counter
//	for (vector<Tx> escList : escListList){
		Graph g(escList.size());

		// percorre itens de transacoes da lista
		for(i = 0; i < escList.size(); i++) {
			// fixa o for em transacao i para teste de conflito de seriazabilidade
			Tx txi = escList.at(i);
			for(j = i; j < escList.size(); j++){
				// fixa o for em transacao j para teste de conflito de seriazabilidade
				Tx txj = escList.at(j);

				// entra no if se tempo de chegada de transacoes
				// estao em ordem crescente,
				// o id das transacoes sao diferentes
				// e os atributos sao os mesmos
				if (txi.getTc() < txj.getTc()
						&&
					txi.getId() != txj.getId()
						&&
					txi.getAt() == txj.getAt()
				){
					if (
						// aresta Ti -> Tj para cada w(x) em Tj depois de r(x) em Ti
						(txi.getOp() == "R" && txj.getOp() == "W")
						||
						// aresta Ti -> Tj para cada r(x) em Tj depois de w(x) em Ti
						(txi.getOp() == "W" && txj.getOp() == "R")
						||
						// aresta Ti -> Tj para cada w(x) em Tj depois de w(x) em Ti
						(txi.getOp() == "W" && txj.getOp() == "W")
					){
						// adiciona aresta em grafo
						g.addEdge(txi.getIdG(), txj.getIdG());
					}
				}
			}
		}

		// aponta se existe ciclo no escalonamento
		if(g.isCyclic()){
			isSerial = false;
		}else{
			isSerial = true;
		}
//	}

}

/*
 * salva um resultado em lista para ser processado em arquivo de saida
 */
void salvaSaida(int index){

	unsigned i;

	vector<int> l = escListList.at(index).GetIdList();
	string lt; // lista de transacoes
	for (i = 0; i < l.size(); i++){
		if (i != 0)
			lt.append(",");

		lt.append(to_string(l.at(i)));
	}

	Saida saida = Saida(index+1, lt.c_str(), isSerial? SIM_SERIAL : NAO_SERIAL, isEV? SIM_VISAO : NAO_VISAO);
	saidaList.push_back(saida);

}

/*
 * salva resultados em arquivo de saida
 */
void processaSaida(){
	FILE *fptr = fopen(ARQUIVO_SAIDA, "w");
	unsigned i;
	for (i = 0; i < saidaList.size(); i++){
		fprintf(fptr, "%d %s %s %s\n",
				saidaList.at(i).getId(),
				saidaList.at(i).getTxs().c_str(),
				saidaList.at(i).getSr().c_str(),
				saidaList.at(i).getVs().c_str());
	}
	fclose(fptr);
}

/*
 * carrega transacoes de arquivo de entrada
 */
void carregaArquivoEntrada(){
	FILE *fptr = fopen(ARQUIVO_ENTRADA, "r");
        char buf[20];
        char tc[5]; // tempo de chegada
        char id[5]; // identificador da transação
        char op[5]; // operação (R=read, W=write, C=commit)
        char at[5]; // atributo lido/escrito

//	fonte: https://support.microsoft.com/en-hk/help/60336/the-fscanf-function-does-not-read-consecutive-lines-as-expected
//	while (fscanf(stdin, "%d %d %[^ ] %[^\n]\n", &tc, &id, op, at) != EOF) // carrega arquivo pela linha de comando no terminal
//	while (fscanf(fptr, "%d %d %[^ ] %[^\n]\n", &tc, &id, op, at) != EOF)

        // carrega cada linha do arquivo
	while (fgets(buf,sizeof(buf), fptr) != NULL)        
//        while (fgets(buf,sizeof(buf), stdin) != NULL)
	{
            buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
            // inicia variaveis com vazio-nulo
            tc[0] = '\0';
            id[0] = '\0';
            op[0] = '\0';
            at[0] = '\0';
            
            // le cada linha
            sscanf(buf, "%s %s %s %s", tc, id, op, at);
            string a;
            a += tc;
            string b;
            b += id;
            
            // carrega cada linha de arquivo de entrada em objeto Tx
            Tx tx (atoi(a.c_str()), atoi(b.c_str()), op, at);
            // insere objeto Tx em lista
            txList.push_back(tx);
	}
}

/*
 * função para criação de arquivo de entrada com dados fornecidos
 */
void criarArquivoEntrada(){
	FILE *fptr = fopen(ARQUIVO_ENTRADA, "w");

	fprintf(fptr, "%d %d %s %s\n", 1, 1, "R", "X");
	fprintf(fptr, "%d %d %s %s\n", 2, 2, "R", "X");
	fprintf(fptr, "%d %d %s %s\n", 3, 2, "W", "X");
	fprintf(fptr, "%d %d %s %s\n", 4, 1, "W", "X");
	fprintf(fptr, "%d %d %s %s\n", 5, 2, "C", "-");
	fprintf(fptr, "%d %d %s %s\n", 6, 1, "C", "-");
	fprintf(fptr, "%d %d %s %s\n", 7, 3, "R", "X");
	fprintf(fptr, "%d %d %s %s\n", 8, 3, "R", "Y");
	fprintf(fptr, "%d %d %s %s\n", 9, 4, "R", "X");
	fprintf(fptr, "%d %d %s %s\n", 10, 3, "W", "Y");
	fprintf(fptr, "%d %d %s %s\n", 11, 4, "C", "-");
	fprintf(fptr, "%d %d %s %s", 12, 3, "C", "-");

	fclose(fptr);
}
