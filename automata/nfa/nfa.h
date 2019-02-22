#pragma once

#ifndef _NFA_H
#define _NFA_H


#include "nfa_helper.h"
#include <iostream>

class NFA
{
	TipStare stariNFA;
	TipAlfabet alfabet;
	TipTabelTranzitiiNFA tabelTranzitii;
	int stareInitiala, nrStari;
	std::vector<int> stariFinale;

public:
	NFA() : stareInitiala(0) { stariNFA[0] = new Stare(0); nrStari = 1; };
	NFA(int _nrStari, TipStare _stari, TipAlfabet _alfabet, int _stareInit, std::vector<int> _stariFinale) :
		stariNFA(_stari), nrStari(_nrStari), alfabet(_alfabet), stareInitiala(_stareInit), stariFinale(_stariFinale) {};
	~NFA() { };
	
	void createStare(int _id, std::vector<int>* _subStari = nullptr); //Creeaza o stare noua in automat
	TipStare getStariNFA() const { return stariNFA; } //Getter
	void setStariNFA(TipStare stari) { this->stariNFA = stari; } //Setter
	Stare& operator[](int i) { return *(stariNFA[i]); } //Extrage o stare din automat

	void setStareInitiala(int _stare); //Setter
	int getStareInitiala() const { return stareInitiala; } //Getter

	void setStariFinale(std::vector<int> _stari) { stariFinale = _stari; }
	std::vector<int> getStariFinale() const { return stariFinale; }
	void insertStareFinala(int _stare); //Inserarea unei noi stari finale in automat

	void setAlfabet(std::vector<char> _alfabet) { alfabet = _alfabet; }
	TipAlfabet getAlfabet() const { return alfabet; }
	void addLitera(char litera); //Adaugarea unei noi litere in alfabet

	void insertTranzitie(int _stare, char _litera, int _next); //Adaugarea unei tranzitii
	TipTabelTranzitiiNFA getTabelTranzitii() const { return tabelTranzitii; }
	void setTabelTranzitii(TipTabelTranzitiiNFA& myTable) { tabelTranzitii = myTable; }

	NFA& operator>>(int nr); //Shiftare stari automat cu nr pozitii la dreapta
	NFA& operator*(); //Klenee star (stelarea unui automat)
	NFA& operator^(const NFA&); //Concatenarea a doua automate
	NFA& operator|(const NFA&); //Reuniunea a doua automate
	NFA& flip(); //Oglindirea unui automat

	friend std::ostream& operator<<(std::ostream& os, const NFA& myNFA); //Afisarea automatului

	NFA& convertToNonLambdaNFA(); //Conversia la non-lambda NFA
};

#endif // !_NFA_H


