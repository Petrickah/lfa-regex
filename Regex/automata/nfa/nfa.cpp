#include "nfa.h"

template<typename Type>
bool vectorContains(std::vector<Type>& vec, Type& x) {
	for (Type elem : vec)
		if (elem == x) return true;
	return false;
}

void NFA::setStareInitiala(int _stare)
{
	if (stariNFA[_stare] != nullptr) {
		if (stareInitiala != _stare) stareInitiala = _stare;
	}
	else {
		stariNFA[_stare] = new Stare(_stare);
		stareInitiala = _stare;
	}
}

void NFA::insertStareFinala(int _stare) {
	if (!vectorContains(stariFinale, _stare))
	{
		if(stariNFA[_stare] == nullptr)
			createStare(_stare);
		stariFinale.push_back(_stare);
	}
	else std::cout << "Starea noua este deja stare finala!" << std::endl;;
}

void NFA::addLitera(char litera) {
	if(!vectorContains(alfabet, litera))
		alfabet.push_back(litera);
	else std::cout << "Automatul contine deja litera " << litera << std::endl;
}

void NFA::createStare(int _id, std::vector<int>* _subStari) {
	if (stariNFA[_id] == nullptr) {
		Stare* newStare = new Stare(_id);
		if (_subStari != nullptr) {
			for (int stare : *_subStari) {
				newStare->addSubStare(stare);
			}
		}
		stariNFA[_id] = newStare;
	}
}

void NFA::insertTranzitie(int _stare, char _litera, int _next)
{
	if (stariNFA[_stare] != nullptr) {
		tabelTranzitii[_stare][_litera].push_back(_next);
		if (stariNFA[_next] == nullptr) createStare(_next);
		
		//std::cout << _stare << "--" << _litera << "-->" << tabelTranzitii[_stare][_litera]<<std::endl;
	}
	else std::cout << "Stare de pornire nu exista in automat!"<<std::endl;
}

NFA & NFA::operator>>(int nr) {

	TipTabelTranzitiiNFA& tabel = this->tabelTranzitii;
	TipStare& stari = this->stariNFA;
	this->stareInitiala += nr;
	std::vector<int>& stariFinale = this->stariFinale;
	for (int i = 0; i<stariFinale.size(); i++)
		stariFinale[i] += nr;
	for (TipStare::reverse_iterator it = stari.rbegin(); it != stari.rend(); it ++) {
		int nrStare = (*it).first;
		tabel[nrStare + nr] = tabel[nrStare];
		for (char litera : this->alfabet) {
			for (int i = 0; i < tabel[nrStare][litera].size(); i++)
				tabel[nrStare + nr][litera][i] = tabel[nrStare + nr][litera][i] + nr;
		}
		for (int i = 0; i < tabel[nrStare][LAMBDA].size(); i++)
			tabel[nrStare + nr][LAMBDA][i] = tabel[nrStare + nr][LAMBDA][i] + nr;
		if(stari[nrStare] != nullptr)
			stari[nrStare]->setID(nrStare + nr);
		stari[nrStare + nr] = stari[nrStare];
	}
	for (int i = 0; i < nr; i++) {
		stari[i] = nullptr;
		for (char litera : this->alfabet)
			tabel[i][litera].clear();
	}
	return *this;
}

NFA & NFA::operator*()
{
	(*this) >> 1;
	this->tabelTranzitii[0].clear();

	TipTabelTranzitiiNFA& tabel = this->tabelTranzitii;
	TipStare& stari = this->stariNFA;

	int newStareInitiala = this->stareInitiala - 1;
	int oldStareInitiala = this->stareInitiala;
	int newStareFinala = *std::max_element(this->stariFinale.begin(), this->stariFinale.end()) + 1;
	stari[newStareInitiala] = new Stare(newStareInitiala);

	this->insertTranzitie(newStareInitiala, LAMBDA, oldStareInitiala);
	this->insertTranzitie(newStareInitiala, LAMBDA, newStareFinala);
	if (stariFinale.size() > 1) {
		for (int stareFinala : this->stariFinale) {
			this->insertTranzitie(stareFinala, LAMBDA, newStareFinala);
		}
		this->insertTranzitie(newStareFinala, LAMBDA, oldStareInitiala);
		this->insertTranzitie(newStareFinala, LAMBDA, newStareFinala + 1);
		newStareFinala++;
		this->tabelTranzitii[0][LAMBDA][1] = newStareFinala;
	}
	else {
		this->insertTranzitie(stariFinale[0], LAMBDA, oldStareInitiala);
		this->insertTranzitie(stariFinale[0], LAMBDA, newStareFinala);
	}

	this->setStareInitiala(newStareInitiala);
	this->stariFinale.clear(); this->setStariFinale({ newStareFinala });
	return (*this);
}

NFA & NFA::operator^(const NFA& myNFA)
{
	NFA* newNFA = new NFA(myNFA);
	(*this)>>(newNFA->nrStari+1);
	for (auto stare : this->stariNFA) {
		if (stare.second != nullptr) {
			newNFA->createStare(stare.second->getID());
			newNFA->tabelTranzitii[stare.second->getID()] = this->tabelTranzitii[stare.second->getID()];
		}
	}
	for (char litera : this->alfabet) {
		if (!vectorContains(newNFA->alfabet, litera)) {
			newNFA->alfabet.push_back(litera);
		}
	}
	for (auto stare : newNFA->stariFinale)
	{
		newNFA->insertTranzitie(stare, LAMBDA, this->stareInitiala);
	}
	newNFA->stariFinale.clear();
	for(auto stare: this->stariFinale)
		newNFA->stariFinale.push_back(stare);


	return *newNFA;
}

NFA & NFA::operator|(const NFA &myNFA)
{
	NFA* newNFA = new NFA(myNFA);
	(*newNFA) >> 1; (*this) >> (newNFA->nrStari + 2);
	for (auto stare : this->stariNFA) {
		if (stare.second != nullptr) {
			newNFA->createStare(stare.second->getID());
			newNFA->tabelTranzitii[stare.second->getID()] = this->tabelTranzitii[stare.second->getID()];
		}
	}
	for (char litera : this->alfabet) {
		if (!vectorContains(newNFA->alfabet, litera)) {
			newNFA->alfabet.push_back(litera);
		}
	}

	newNFA->createStare(0);
	newNFA->insertTranzitie(0, LAMBDA, newNFA->stareInitiala);
	newNFA->insertTranzitie(0, LAMBDA, this->stareInitiala);
	newNFA->stareInitiala = 0;

	for (int stare : this->stariFinale)
		newNFA->insertStareFinala(stare);
	int max = newNFA->stariFinale[0];
	for (int stare : newNFA->stariFinale)
		if (stare > max) max = stare;

	newNFA->createStare(max+1);
	for (int stareFinala : newNFA->stariFinale)
		newNFA->insertTranzitie(stareFinala, LAMBDA, max + 1);
	newNFA->stariFinale.clear(); newNFA->setStariFinale({ max + 1 });
	return *newNFA;
}

std::ostream & operator<<(std::ostream & os, const NFA & myNFA)
{
	os << "Stare initiala: " << myNFA.stareInitiala << std::endl;
	os << "Stari finale: ";
	for (int stare : myNFA.stariFinale) {
		os << stare << " ";
	}
	os << std::endl;
	os << "Stari automat: ";
	for (auto stare : myNFA.stariNFA)
		if(stare.second != nullptr)
			os << stare.second->getID() <<" ";
	os << std::endl;
	os << "Alfabet: ";
	for (char litera : myNFA.alfabet) {
		os << litera << " ";
	}
	os << std::endl << std::endl;
	os << "  Stare  ";
	for (char litera : myNFA.alfabet) {
		os <<"\t"<< litera;
	}
	os << "\tLAMBDA";
	os << std::endl;
	TipTabelTranzitiiNFA tabelTranzitii = myNFA.getTabelTranzitii();
	for (auto stare : myNFA.getStariNFA()) {
		if(stare.second != nullptr)
			os <<"    "<< stare.second->getID();
		else os <<"    -";
		os << "\t";
		for (char litera : myNFA.getAlfabet()) {
			os << "\t"; bool ok = true;
			if (stare.second != nullptr)
			{
				for (int next : tabelTranzitii[stare.second->getID()][litera]) {
					if (next != -1)
						os << next << " "; ok = false;
				}
			}
			if (ok) os << "-";
		}
		os << "\t"; bool ok = true;
		if (stare.second != nullptr)
		{
			for (int next : tabelTranzitii[stare.second->getID()][LAMBDA]) {
				if (next != -1)
					os << next << " "; ok = false;
			}
		}
		if (ok) os << "-";
		os << std::endl;
	}
	return os;
}

NFA & NFA::flip()
{
	NFA* newNFA = new NFA;
	newNFA->setAlfabet(this->alfabet);
	newNFA->setStariFinale(this->stariFinale);
	newNFA->setStariNFA(this->stariNFA);
	newNFA->nrStari = this->stariNFA.size();

	for (auto stare : this->stariNFA) {
		if (stare.second != nullptr) {
			for (char litera : this->alfabet) {
				if (stare.second != nullptr)
				{
					auto vStariVecine = this->tabelTranzitii[stare.second->getID()][litera];
					for (int nextStare : vStariVecine) {
						newNFA->insertTranzitie(newNFA->nrStari - nextStare - 1, litera, newNFA->nrStari - stare.second->getID() - 1);
					}
				}
			}

			if (stare.second != nullptr)
			{
				auto vStariVecine = this->tabelTranzitii[newNFA->nrStari - stare.second->getID() - 1][LAMBDA];
				for (int nextStare : vStariVecine) {
					newNFA->insertTranzitie(newNFA->nrStari - nextStare - 1, LAMBDA, newNFA->nrStari - stare.second->getID() - 1);
				}
			}
		}
	}

	for (auto stare : this->stariNFA) {
		if (stare.second != nullptr)
		{
			auto& vStariVecine = newNFA->tabelTranzitii[newNFA->nrStari - stare.second->getID() - 1][LAMBDA];
			for (int i = 0; i < vStariVecine.size(); i++) {
				vStariVecine[i] = newNFA->nrStari - vStariVecine[i] - 1;
			}
		}
	}
	return *newNFA;
}

NFA & NFA::convertToNonLambdaNFA()
{
	NFA* myNFA = new NFA();
	TipTabelTranzitiiNFA _tabelTranzitii = Helper::computeNonLambdaNFATable(this->stareInitiala, this->alfabet, this->tabelTranzitii);
	TipStare _stariNFA = Helper::computeStariNonLambdaNFA(_tabelTranzitii);
	TipAlfabet _alfabet = this->alfabet;
	std::vector<int> _stariFinale;
	for (auto stare : _stariNFA) {
		if (Helper::isStareFinala(stare.second->getID(), this->stariFinale, this->tabelTranzitii))
			_stariFinale.push_back(stare.second->getID());
	}
	int nrStari = _stariNFA.size();

	myNFA->setAlfabet(_alfabet); this->alfabet = _alfabet;
	myNFA->setStareInitiala(this->stareInitiala); this->stareInitiala = this->stareInitiala;
	myNFA->setStariFinale(_stariFinale); this->stariFinale = _stariFinale;
	myNFA->setStariNFA(_stariNFA); this->stariNFA = _stariNFA;
	myNFA->setTabelTranzitii(_tabelTranzitii); this->tabelTranzitii = _tabelTranzitii;
	return *myNFA;
}
