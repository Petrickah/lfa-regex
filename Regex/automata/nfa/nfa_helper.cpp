#include "nfa_helper.h"

std::vector<int> Helper::computeEClosure(int stare, const TipTabelTranzitiiNFA & tabel)
{
	int _stare = stare;
	std::vector<int> result;
	std::stack<int> myStack;

	Start:
		std::vector<int> eclosure = ((TipTabelTranzitiiNFA &)tabel)[_stare][LAMBDA];
		if (!eclosure.empty()) {
			for (int item : eclosure) {
				if(find(result.begin(), result.end(), item) == result.end()) //*
					myStack.push(item);
			}
			while (!myStack.empty()) {
				_stare = myStack.top(); myStack.pop();
				if(find(result.begin(), result.end(), _stare) == result.end()) //*
					result.push_back(_stare); 
				goto Start;
			}
		}
		else {
			if (!myStack.empty()) {
				_stare = myStack.top(); myStack.pop();
				if (find(result.begin(), result.end(), _stare) == result.end()) //*
					result.push_back(_stare);
				goto Start;
			}
		}
	End:
		result.push_back(stare);
		return result;
}

std::map<char, std::vector<int>> Helper::computeTransitions(std::vector<int> eclosure, const TipAlfabet & alph, const TipTabelTranzitiiNFA & tabel)
{
	std::map<char, std::vector<int>> result;
	for (char litera : alph) {
		result[litera] = std::vector<int>();
		for (int stare : eclosure) {
			auto trans = ((TipTabelTranzitiiNFA&)tabel)[stare][litera];
			for (int nextStare : trans)
				if (std::find(result[litera].begin(), result[litera].end(), nextStare) == result[litera].end())
					result[litera].push_back(nextStare);
		}
	}
	return result;
}

TipTabelTranzitiiNFA Helper::computeNonLambdaNFATable(int stareInitiala, const TipAlfabet & alph, const TipTabelTranzitiiNFA & tabel)
{
	Declarari:
		TipTabelTranzitiiNFA result;
		std::stack<int> myStack;
		std::vector<int> oldStack;
		int stare;

	Step_1:
		auto trans = Helper::computeTransitions(Helper::computeEClosure(stareInitiala, tabel), alph, tabel); //*
		result[stareInitiala] = trans;
	Step_2:
		for (char litera : alph) {
			for (int stare : trans[litera])
				if(std::find(oldStack.begin(), oldStack.end(), stare) == oldStack.end())
					myStack.push(stare);
		}
		if (myStack.empty()) goto Step_3;
		stare = myStack.top(); myStack.pop();
		trans = Helper::computeTransitions(Helper::computeEClosure(stare, tabel), alph, tabel);
		result[stare] = trans;
		oldStack.push_back(stare);
		goto Step_2;
	Step_3:
		return result;
}

TipStare Helper::computeStariNonLambdaNFA(const TipTabelTranzitiiNFA & tabel)
{
	TipStare stariNFA;
	for (auto pair : tabel) {
		stariNFA[pair.first] = new Stare(pair.first);
	}
	return stariNFA;
}

bool Helper::isStareFinala(int stare, const std::vector<int>& stariFinale, const TipTabelTranzitiiNFA & tabel)
{
	bool ok = false;
	auto eclosure = computeEClosure(stare, tabel);
	for (int estare : eclosure) {
		if (std::find(stariFinale.begin(), stariFinale.end(), estare) != stariFinale.end())
			ok = true;
	}
	return ok;
}

std::vector<int> Helper::nextStari(char litera, std::vector<int>& currStari, const TipTabelTranzitiiNFA & tabel)
{
	std::vector<int> result;
	for (int stare : currStari) {
		auto stari = ((TipTabelTranzitiiNFA&)tabel)[stare][litera];
		for (int i = 0; i < stari.size(); i++)
			if (find(result.begin(), result.end(), stari[i]) == result.end())
				result.push_back(stari[i]);
	}
	return result;
}
