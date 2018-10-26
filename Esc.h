/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Esc.h
 * Author: ubuntu
 *
 * Created on October 6, 2018, 4:26 PM
 */


#ifndef ESC_H
#define ESC_H
#include <vector>
#include <list>
#include "Tx.h"
using namespace std;
class Esc {
public:
    Esc();
    Esc(vector<int> idList);
    Esc(vector<Tx> escList, vector<int> idList);
    virtual ~Esc();
    void SetEscList(vector<Tx> escList);
    vector<Tx> GetEscList();
    void SetIdList(vector<int> idList);
    vector<int> GetIdList();
    void SetVisList(vector<Tx> visList);
    vector<Tx> GetVisList();

private:
    vector<Tx> escList; // transacoes de um escalonamento
    vector<Tx> visList; //  Lista com lista de permutacoes de escalonamentos para teste de equivalencia de visao
    vector<int> idList; // ids unicos de transacoes de escalonamento

};

#endif /* ESC_H */

