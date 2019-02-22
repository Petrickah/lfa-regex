#pragma once

#ifndef _NFA_HELPER_H
#define _NFA_HELPER_H

#include "../Stare.h"
#include <stack>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
typedef std::vector<char> TipAlfabet;
typedef std::map<int, Stare*> TipStare;
typedef std::map<int, std::map<char, std::vector<int>>> TipTabelTranzitiiNFA;
#define LAMBDA '~'
using namespace std;

namespace Helper {
	vector<int> computeEClosure(int stare, const TipTabelTranzitiiNFA& tabel);
	map<char, vector<int>> computeTransitions(vector<int> eclosure, const TipAlfabet& alph, const TipTabelTranzitiiNFA& tabel);
	TipTabelTranzitiiNFA computeNonLambdaNFATable(int stareInitiala, const TipAlfabet& alph, const TipTabelTranzitiiNFA& tabel);
	TipStare computeStariNonLambdaNFA(const TipTabelTranzitiiNFA& tabel);
	bool isStareFinala(int stare, const std::vector<int>& stariFinale, const TipTabelTranzitiiNFA& tabel);
	std::vector<int> nextStari(char litera, std::vector<int>& currStari, const TipTabelTranzitiiNFA& tabel);
}
#endif // !_NFA_HELPER_H
